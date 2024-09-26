from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    openni2_camera_share = get_package_share_directory('openni2_camera')
    camera_launch_file = os.path.join(openni2_camera_share, 'launch', 'camera_only.launch.py')

    return LaunchDescription([
        # Launch the camera node
        # IncludeLaunchDescription(
        #     PythonLaunchDescriptionSource(camera_launch_file)
        # ),

        # Launch YOLO node
        Node(
            package='perception',
            executable='yolo_inference',
            name='yolo_inference',
            output='screen',
        ),
    ])
