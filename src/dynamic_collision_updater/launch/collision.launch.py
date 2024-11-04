import launch
from launch_ros.actions import Node

def generate_launch_description():
    collision_node = Node(
        package="dynamic_collision_updater",
        executable="dynamic_collision_updater",
        name="collision",
        output="screen",
        parameters=[
            # {"planning_frame": "base_link"},
        ],
    )

    return launch.LaunchDescription([collision_node])