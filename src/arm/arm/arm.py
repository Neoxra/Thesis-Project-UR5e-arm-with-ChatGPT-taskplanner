import rclpy
from rclpy.node import Node
from interfaces.srv import ArmMovement
import random
import time

class ArmService(Node):
    def __init__(self):
        super().__init__('arm_service')
        self.srv = self.create_service(ArmMovement, 'arm', self.handle_arm_movement)
        self.get_logger().info("Arm Movement service is ready.")

    def handle_arm_movement(self, request, response):
        self.get_logger().info("Arm movement requested")

        delay_time = random.uniform(5, 10)
        self.get_logger().info(f'Simulating a movement taking {delay_time:.2f} seconds...')
        time.sleep(delay_time)

        response.success = True
        self.get_logger().info("Simulated arm movement finished")
        return response

def main(args=None):
    rclpy.init(args=args)
    node = ArmService()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
