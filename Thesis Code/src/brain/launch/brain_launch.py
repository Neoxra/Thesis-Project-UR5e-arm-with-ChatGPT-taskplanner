from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # Brain package nodes
        Node(
            package='brain',
            executable='brain_transformations',
            name='brain_transformations',
            output='screen'
        ),
        Node(
            package='brain',
            executable='brain',
            name='brain_node',
            output='screen'
        ),
    ])
