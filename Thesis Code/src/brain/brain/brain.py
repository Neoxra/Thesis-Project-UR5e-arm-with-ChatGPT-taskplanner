# ChatGPT was used to aid in the wriing of this code.
import rclpy
from rclpy.node import Node
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from interfaces.srv import Command, Inventory, ArmMovement
from interfaces.msg import BrainStatus
from geometry_msgs.msg import TransformStamped
from tf2_ros import Buffer, TransformListener
import tf2_ros
import math

class BrainNode(Node):
    def __init__(self):
        super().__init__('brain')

        # Define callback groups to avoid deadlocks
        arm_cb_group = MutuallyExclusiveCallbackGroup()
        inventory_cb_group = MutuallyExclusiveCallbackGroup()
        service_cb_group = MutuallyExclusiveCallbackGroup()

        # Service clients with callback groups
        self.arm_client = self.create_client(ArmMovement, 'arm', callback_group=arm_cb_group)
        self.inventory_client = self.create_client(Inventory, 'inventory', callback_group=inventory_cb_group)
        
        # Create the service to receive commands in a separate callback group
        self.command_service = self.create_service(Command, 'command', self.handle_command, callback_group=service_cb_group)

        # Create the publisher for status updates
        self.status_pub = self.create_publisher(BrainStatus, 'brain_status', 10)

        # Create a TF2 buffer and listener to check for transformations
        self.tf_buffer = Buffer()
        self.tf_listener = TransformListener(self.tf_buffer, self)

        # Create a sub-node for spin_until_future_complete
        if not hasattr(self, 'sub_node'):
            self.sub_node = rclpy.create_node('sub_node')
        else:
            self.sub_node.get_logger().warning("Sub-node already exists")

        # Status tracking
        self.busy = False

        self.get_logger().info('Brain node has started.')

    async def handle_command(self, request, response):
        """Handle incoming pick/place commands."""
        if self.busy:
            response.accept = "busy"
            self.publish_status(f"UPDATE: System is busy", log=True)
            return response

        # Set the system to busy
        self.busy = True
        self.publish_status(f"UPDATE: new request {request.command} item {request.item_id} - Status {response.accept}", log=True)

        if request.command == "pick":
            await self.handle_pick(request, response)
        elif request.command == "place":
            await self.handle_place(request, response)
        else:
            response.accept = "malformed command"
            self.publish_status(f"ERROR: malformed command {request.command}", log=True)

        self.busy = False
        return response

    async def handle_pick(self, request, response):
        """Handle the pick command."""
        self.attempts = 0
        self.max_attempts = 5

        # Retry logic for looking up the transform
        await self.retry_lookup(request, response)

    async def retry_lookup(self, request, response):
        """Retry looking up the transform."""
        try:
            transform = self.tf_buffer.lookup_transform(
                'camera_link', str(request.item_id), rclpy.time.Time(), timeout=rclpy.duration.Duration(seconds=5)
            )
            self.get_logger().info(f"Transformation found for item {request.item_id}, proceeding with pick command")
            await self.proceed_with_pick(transform, request, response)
        except tf2_ros.LookupException:
            self.attempts += 1
            if self.attempts < self.max_attempts:
                error_msg = f"ERROR: cannot find transformation to item {request.item_id}"
                self.publish_status(error_msg, log=True)
                await self.retry_lookup(request, response)  # await the retry
            else:
                error_msg = f"ERROR: failed to find transformation to item {request.item_id} after {self.max_attempts} attempts"
                self.publish_status(error_msg, log=True)
                response.accept = "ERROR"

    async def proceed_with_pick(self, transform, request, response):
        """Proceed with the pick operation once the transform is found."""
        distance = math.sqrt(transform.transform.translation.x ** 2 +
                             transform.transform.translation.y ** 2 +
                             transform.transform.translation.z ** 2)
        if distance > 1.0:
            response.accept = "ERROR"
            error_msg = f"ERROR: item {request.item_id} is too far"
            self.publish_status(error_msg, log=True)
            return

        # Call the inventory service to put the item in
        inventory_request = Inventory.Request()
        inventory_request.command = "put_in_inventory"
        inventory_request.item_id = request.item_id
        inventory_response = await self.call_inventory_service(inventory_request)

        if inventory_response.response == -1:
            self.get_logger().error("ERROR: inventory is full")
            response.accept = "ERROR"
            return

        # Call the arm service to move the item
        arm_request = ArmMovement.Request()
        arm_request.command = "pick"
        arm_request.x = transform.transform.translation.x
        arm_request.y = transform.transform.translation.y
        arm_request.z = transform.transform.translation.z
        arm_request.inventory_slot = inventory_response.response
        arm_response = await self.call_arm_service(arm_request)
        
        if arm_response.success:
            response.accept = "accepted"
        else:
            response.accept = "ERROR"
            self.publish_status(f"ERROR: arm movement failed for item {request.item_id}", log=True)

        self.publish_status(f"UPDATE: added item {request.item_id} to inventory slot {inventory_response.response}")
        response.accept = "accepted"

    async def handle_place(self, request, response):
        """Handle the place command."""
        # Set the system to busy
        self.busy = True

        # Call the inventory service to get the item location
        inventory_request = Inventory.Request()
        inventory_request.command = "get_from_inventory"
        inventory_request.item_id = request.item_id
        inventory_response = await self.call_inventory_service(inventory_request)

        if inventory_response is None or inventory_response.response == -1:
            # Item is not in inventory, publish error and reset
            self.publish_status(f"ERROR: item {request.item_id} is not in inventory", log=True)
            response.accept = "ERROR"
            self.busy = False  # Reset the busy status
            return

        # Call the arm service to place the item at the predefined location (0.5, 0.0, 0.0)
        arm_request = ArmMovement.Request()
        arm_request.command = "place"
        arm_request.x = 0.5  # Fixed x position
        arm_request.y = 0.0  # Fixed y position
        arm_request.z = 0.0  # Fixed z position
        arm_request.inventory_slot = inventory_response.response

        arm_response = await self.call_arm_service(arm_request)

        if arm_response.success:
            self.publish_status(f"UPDATE: placed item {request.item_id} from inventory slot {inventory_response.response}", log=True)
            response.accept = "accepted"
        else:
            self.publish_status(f"ERROR: arm movement failed for item {request.item_id}", log=True)
            response.accept = "ERROR"

        # Reset the system to not busy
        self.busy = False


    async def call_arm_service(self, request):
        """Call the arm movement service using ArmMovement.srv."""
        self.publish_status("UPDATE: arm movement request", log=True)

        while not self.arm_client.wait_for_service(timeout_sec=2.0):
            self.get_logger().info('Arm service not available, waiting...')
        self.get_logger().info('Arm service is available.')

        future = self.arm_client.call_async(request)
        rclpy.spin_until_future_complete(self.sub_node, future)

        if future.result() and future.result().success:
            self.get_logger().info(f"UPDATE: arm movement finished")
        else:
            self.get_logger().error(f"ERROR: arm movement failed")

        return future.result()


    async def call_inventory_service(self, request):
        while not self.inventory_client.wait_for_service(timeout_sec=2.0):
            self.get_logger().info('Inventory service not available, waiting...')
        self.get_logger().info('Inventory service is available.')

        future = self.inventory_client.call_async(request)
        rclpy.spin_until_future_complete(self.sub_node, future)

        if future.result() is None:
            self.get_logger().error('Inventory service returned no response (None)')
        else:
            self.get_logger().info(f"Inventory service response: {future.result()}")

        return future.result()


    def publish_status(self, message, log=False):
        """Publish brain status messages and optionally log them."""
        status_msg = BrainStatus()
        status_msg.status = message
        self.status_pub.publish(status_msg)
        if log:
            self.get_logger().info(message)


def main(args=None):
    rclpy.init(args=args)
    brain_node = BrainNode()
    rclpy.spin(brain_node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
