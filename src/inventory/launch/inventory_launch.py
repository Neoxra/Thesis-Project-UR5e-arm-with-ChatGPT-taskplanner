from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
          # Inventory package nodes
        Node(
            package='inventory',
            executable='inventory_transformations',
            name='inventory_transformations',
            output='screen'
        ),
        Node(
            package='inventory',
            executable='inventory_manager',
            name='inventory_manager',
            output='screen'
        ),
    ])
