import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from visualization_msgs.msg import Marker, MarkerArray
from cv_bridge import CvBridge
import cv2
from ultralytics import YOLO
import os

class YoloNode(Node):
    def __init__(self):
        super().__init__('yolo_inference_node')

        # Declare ROS2 parameter for the model path
        self.declare_parameter('model_path', 'yolov8n.pt')  # Default to YOLOv8n

        # Get the model path from the parameter (either default or custom)
        model_path = self.get_parameter('model_path').get_parameter_value().string_value

        # Check if it's a valid path or use the YOLO model name
        if os.path.exists(model_path):
            self.get_logger().info(f'Loading custom YOLO model from: {model_path}')
        else:
            self.get_logger().info(f'Using pretrained YOLO model: {model_path}')

        # Load the YOLO model
        self.model = YOLO(model_path)

        # Image subscriber
        self.subscription = self.create_subscription(
            Image, 'camera/rgb/image_raw', self.image_callback, 10
        )
        
        # Marker publisher
        self.marker_publisher = self.create_publisher(MarkerArray, 'yolo/markers', 10)

        self.bridge = CvBridge()

    def image_callback(self, msg):
        try:
            # Convert ROS Image message to OpenCV image
            cv_image = self.bridge.imgmsg_to_cv2(msg, "bgr8")

            # Run YOLO inference
            results = self.model(cv_image)
            self.get_logger().info(f'YOLO Results: {results}')

            # Create MarkerArray for bounding boxes
            marker_array = MarkerArray()

            for i, result in enumerate(results):
                for box in result.boxes:
                    marker = Marker()
                    marker.header.frame_id = 'camera_link'
                    marker.type = Marker.CUBE
                    marker.action = Marker.ADD

                    marker.pose.position.x = (box.xyxy[0] + box.xyxy[2]) / 2
                    marker.pose.position.y = (box.xyxy[1] + box.xyxy[3]) / 2
                    marker.pose.position.z = 0.5  # Adjust based on field of view

                    marker.scale.x = box.xyxy[2] - box.xyxy[0]
                    marker.scale.y = box.xyxy[3] - box.xyxy[1]
                    marker.scale.z = 0.1  # Thickness

                    marker.color.a = 0.8
                    marker.color.r = 0.0
                    marker.color.g = 1.0
                    marker.color.b = 0.0

                    marker.id = i

                    marker_array.markers.append(marker)

            # Publish the markers
            self.marker_publisher.publish(marker_array)

        except Exception as e:
            self.get_logger().error(f"Error processing image: {str(e)}")


def main(args=None):
    rclpy.init(args=args)
    node = YoloNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
