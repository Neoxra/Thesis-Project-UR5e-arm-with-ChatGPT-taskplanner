import rclpy
import cv2
import os
from rclpy.node import Node
from sensor_msgs.msg import Image
from visualization_msgs.msg import Marker, MarkerArray
from cv_bridge import CvBridge
from interfaces.msg import Camera
from ultralytics import YOLO

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
        
        # Camera publisher
        self.camera_publisher = self.create_publisher(Camera, 'camera/objects', 10)

        # Timer to run YOLO inference intermittently (every 5 seconds)
        self.timer = self.create_timer(5.0, self.run_yolo_inference)

        self.bridge = CvBridge()
        self.image = None  # Store the latest image

    def image_callback(self, msg):
        try:
            # Store the latest image for processing by the timer
            self.image = self.bridge.imgmsg_to_cv2(msg, "bgr8")
        except Exception as e:
            self.get_logger().error(f'Error processing image: {str(e)}')

    def run_yolo_inference(self):
        # Check if there is an image available
        if self.image is None:
            self.get_logger().info('No image available for inference.')
            return

        try:
            # Run YOLO detection on the latest image
            results = self.model(self.image)

            # Iterate through results and publish in Camera.msg format
            for i, result in enumerate(results):
                for box in result.boxes:
                    self.get_logger().info(f'YOLO Results: {i}, {box}')
                    # Create a Camera message for each detected object
                    camera_msg = Camera()
                    camera_msg.item_id = i + 1  # 1-based
                    camera_msg.x = (box.xyxy[0] + box.xyxy[2]) / 2  # X center
                    camera_msg.y = (box.xyxy[1] + box.xyxy[3]) / 2  # Y center
                    camera_msg.z = 0.5  # Example depth, adjust as needed

                    # Publish the Camera message
                    self.camera_publisher.publish(camera_msg)

                    # Optionally, log the detection
                    self.get_logger().info(f'Published item {camera_msg.item_id} at '
                                           f'x: {camera_msg.x}, y: {camera_msg.y}, z: {camera_msg.z}')
        except Exception as e:
            self.get_logger().error(f'Error running YOLO inference: {str(e)}')

def main(args=None):
    rclpy.init(args=args)
    node = YoloNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
