# ChatGPT was used to aid in the writing of this code.
import rclpy
from rclpy.node import Node
from interfaces.msg import Camera
from std_msgs.msg import Int32MultiArray
from geometry_msgs.msg import TransformStamped
import tf2_ros


class PerceptionNode(Node):
    def __init__(self):
        super().__init__('perception_node')

        # Create the subscriber for the camera topic
        self.camera_sub = self.create_subscription(Camera, 'camera', self.camera_callback, 10)

        # Create the publisher for the perception status
        self.perception_status_pub = self.create_publisher(Int32MultiArray, 'perception_status', 10)

        # Buffer to store seen item IDs and their timestamps
        self.seen_items = []

        # Timer to periodically publish the perception status at 1Hz
        self.create_timer(1.0, self.publish_perception_status)

        # Create the tf2 broadcaster to send dynamic transforms
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)

        self.get_logger().info('Perception Node has started.')

    def camera_callback(self, msg: Camera):
        # Broadcast dynamic transform based on Camera.msg
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'camera_link'
        t.child_frame_id = str(msg.item_id)
        t.transform.translation.x = msg.x
        t.transform.translation.y = msg.y
        t.transform.translation.z = msg.z
        t.transform.rotation.x = 0.0
        t.transform.rotation.y = 0.0
        t.transform.rotation.z = 0.0
        t.transform.rotation.w = 1.0

        # Send the transform
        self.tf_broadcaster.sendTransform(t)

        # Track the seen item by adding it to the list with a timestamp
        self.seen_items.append((msg.item_id, self.get_clock().now()))

    def publish_perception_status(self):
        # Clean up items older than 5 seconds
        self.cleanup_old_items()

        # Create and publish the Int32MultiArray message
        item_ids = [item_id for item_id, _ in self.seen_items]
        msg = Int32MultiArray()
        msg.data = item_ids
        self.perception_status_pub.publish(msg)

    def cleanup_old_items(self):
        # Remove items older than 5 seconds from the buffer
        current_time = self.get_clock().now()
        self.seen_items = [(item_id, timestamp) for item_id, timestamp in self.seen_items if (current_time - timestamp).nanoseconds <= 5e9]


def main(args=None):
    rclpy.init(args=args)
    node = PerceptionNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
