from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess

import os

launch_files = [
    'launch/inventory_launch.py',
]


def generate_launch_description():

    return LaunchDescription([
        Node(
            package='arm',  # Replace with your actual package name
            executable='arm',
            name='arm_node',
            output='screen',
        ),
        
    ])