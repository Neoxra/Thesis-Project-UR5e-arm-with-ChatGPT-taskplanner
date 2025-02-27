#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import TransformStamped
import tf2_ros
import math


class BrainTransformationsNode(Node):
    def __init__(self):
        super().__init__('brain_transformations')

        # Create a static transform broadcaster
        self.static_broadcaster = tf2_ros.StaticTransformBroadcaster(self)

        # Publish the required static transformations
        self.publish_static_transforms()

        self.get_logger().info('Brain Transformation Node has started.')

    def create_transform(self, parent_frame, child_frame, translation, rotation):
        """Helper function to create a TransformStamped message."""
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = parent_frame
        t.child_frame_id = str(child_frame)
        t.transform.translation.x = translation[0]
        t.transform.translation.y = translation[1]
        t.transform.translation.z = translation[2]
        t.transform.rotation.x = rotation[0]
        t.transform.rotation.y = rotation[1]
        t.transform.rotation.z = rotation[2]
        t.transform.rotation.w = rotation[3]
        return t

    def publish_static_transforms(self):
        """Publish the required static transforms for the brain node."""
        transforms = [
            # map -> base_link
            self.create_transform(
                "map", "base_link",
                translation=[1.0, 1.0, 0.0],
                rotation=[0.0, 0.0, 0.0, 1.0]
            ),
            # base_link -> arm_base
            self.create_transform(
                "base_link", "arm_base",
                translation=[0.1, 0.0, 0.2],
                rotation=[0.0, 0.0, 0.0, 1.0]
            ),
            # base_link -> camera_link (rotation π/6 around Y-axis)
            self.create_transform(
                "base_link", "camera_link",
                translation=[-0.5, 0.0, 0.5],
                rotation=[
                    0.0, math.sin(math.pi / 12),  # sin(π/6)/2 for Y rotation
                    0.0, math.cos(math.pi / 12)   # cos(π/6)/2 for W rotation
                ]
            )
        ]

        # Publish all transforms
        self.static_broadcaster.sendTransform(transforms)


def main(args=None):
    rclpy.init(args=args)
    node = BrainTransformationsNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
