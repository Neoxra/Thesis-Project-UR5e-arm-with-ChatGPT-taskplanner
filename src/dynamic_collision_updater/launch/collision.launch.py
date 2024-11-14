import os
import sys
import launch
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory

path = os.path.join(get_package_share_directory('moveit_hybrid_planning'), 'launch')
print("Path to launch folder:", path)
sys.path.append(path)

# Import custom configuration functions
from hybrid_planning_common import (
    get_robot_description,
    get_robot_description_semantic,
)

def generate_launch_description():

    # Define the dynamic_collision_updater node
    collision_node = Node(
        package="dynamic_collision_updater",
        executable="dynamic_collision_updater",
        name="collision",
        output="screen",
        parameters=[
            get_robot_description(),
            get_robot_description_semantic(),
        ],
    )

    # Define the camera_tf_broadcaster node
    camera_tf_broadcaster_node = Node(
        package="dynamic_collision_updater",  # Replace with the actual package name if different
        executable="camera_tf_broadcaster",
        name="camera_tf_broadcaster",
        output="screen"
    )

    # Return the launch description with both nodes
    return launch.LaunchDescription([collision_node])
