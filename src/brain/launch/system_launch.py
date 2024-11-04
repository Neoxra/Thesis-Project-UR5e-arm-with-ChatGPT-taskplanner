from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
from launch.actions import ExecuteProcess
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

    # # Include Camera Launch (Python)
    # camera_launch = IncludeLaunchDescription(
    #     PythonLaunchDescriptionSource(
    #         os.path.join(get_package_share_directory('openni2_camera'), 'launch', 'camera_only.launch.py')
    #     )
    # )

    # Run the camera launch file in a new terminal
    camera_launch = ExecuteProcess(
        cmd=['gnome-terminal', '--', 'ros2', 'launch', 'openni2_camera', 'camera_only.launch.py'],
        output='screen'
    )

    return LaunchDescription([
        camera_launch,
        brain_launch,
        inventory_launch,
        perception_launch,
    ])
