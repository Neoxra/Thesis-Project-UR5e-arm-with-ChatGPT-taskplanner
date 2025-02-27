import rclpy
from rclpy.node import Node
from interfaces.srv import Command
from interfaces.msg import Camera
from std_msgs.msg import String

class SystemTestNode(Node):
    def __init__(self):
        super().__init__('system_test_node')

        # Service clients
        self.command_client = self.create_client(Command, 'command')

        # Publisher for Camera topic
        self.camera_pub = self.create_publisher(Camera, 'camera', 10)

        # Subscription for brain status
        self.create_subscription(String, 'brain_status', self.brain_status_callback, 10)

        # Wait for the services to be available
        while not self.command_client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('Waiting for command service...')

        # Start the test
        self.run_test()

    def run_test(self):
        self.get_logger().info("Starting system test...")

        # Publish a test Camera message
        # self.publish_camera_message(item_id=1, x=0.5, y=0.2, z=0.1)

        # Call the "pick" command for the item
        self.call_command_service(command="pick", item_id=1)
        self.get_logger().info('Pickup command sent')

        # Create a timer to wait for 5 seconds before calling the "place" command
        self.timer = self.create_timer(5.0, self.call_place_command)

    def call_place_command(self):
        """Timer callback to call the 'place' command."""
        self.call_command_service(command="place", item_id=1)
        self.get_logger().info('Place command sent')

        if self.timer is not None:
            self.timer.cancel()
            self.timer = None 


    def publish_camera_message(self, item_id, x, y, z):
        """Simulate a camera detection by publishing to the camera topic."""
        msg = Camera()
        msg.item_id = item_id
        msg.x = x
        msg.y = y
        msg.z = z
        self.camera_pub.publish(msg)
        self.get_logger().info(f"Published Camera message: Item ID = {item_id}, Position = ({x}, {y}, {z})")

    def call_command_service(self, command, item_id):
        """Call the command service with a pick/place command."""
        request = Command.Request()
        request.command = command
        request.item_id = item_id
        future = self.command_client.call_async(request)
        future.add_done_callback(self.command_response_callback)

    def command_response_callback(self, future):
        """Handle the response from the command service."""
        try:
            response = future.result()
            if response is not None:
                self.get_logger().info(f"Command response: {response.accept}")
            else:
                self.get_logger().error("Failed to get response from command service.")
        except Exception as e:
            self.get_logger().error(f"Service call failed with exception: {str(e)}")

    def brain_status_callback(self, msg):
        """Handle brain status updates."""
        self.get_logger().info(f"Brain status: {msg.data}")

def main(args=None):
    rclpy.init(args=args)
    node = SystemTestNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
