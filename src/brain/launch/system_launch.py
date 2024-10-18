from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Include Brain Launch (Python)
    brain_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('brain'), 'launch', 'brain_launch.py')
        )
    )

    # Include inventory Launch (Python)
    inventory_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('inventory'), 'launch', 'inventory_launch.py')
        )
    )

    # Include Perception Launch (Python)
    perception_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(get_package_share_directory('perception'), 'launch', 'perception_launch.py')
        )
    )

    return LaunchDescription([
        brain_launch,
        inventory_launch,
        perception_launch,
    ])
