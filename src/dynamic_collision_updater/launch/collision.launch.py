import launch
from launch_ros.actions import Node

def generate_launch_description():
    # Define the dynamic_collision_updater node
    collision_node = Node(
        package="dynamic_collision_updater",
        executable="dynamic_collision_updater",
        name="collision",
        output="screen",
        parameters=[
            # {"planning_frame": "base_link"},
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
