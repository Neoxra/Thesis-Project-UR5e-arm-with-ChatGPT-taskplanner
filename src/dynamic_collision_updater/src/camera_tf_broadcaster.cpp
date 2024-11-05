#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <chrono>

class CameraTfBroadcaster : public rclcpp::Node
{
public:
    CameraTfBroadcaster()
        : Node("camera_tf_broadcaster"), tf_broadcaster_(this)
    {
        // Timer to periodically broadcast the transform
        tf_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&CameraTfBroadcaster::broadcastCamera2Transform, this)
        );

        RCLCPP_INFO(this->get_logger(), "Camera TF Broadcaster Node initiated.");
    }

private:
    tf2_ros::TransformBroadcaster tf_broadcaster_;
    rclcpp::TimerBase::SharedPtr tf_timer_;

    void broadcastCamera2Transform()
    {
        geometry_msgs::msg::TransformStamped transformStamped;

        // Define the transformation from base_link to camera2_link
        transformStamped.header.stamp = this->get_clock()->now();
        transformStamped.header.frame_id = "base_link"; // Parent frame
        transformStamped.child_frame_id = "camera2_link"; // New frame

        // Set translation values (example: 1 meter forward, 0.5 meters up)
        transformStamped.transform.translation.x = 1.0; 
        transformStamped.transform.translation.y = 0.0;
        transformStamped.transform.translation.z = 0.5;

        // Set rotation values (identity quaternion, no rotation)
        transformStamped.transform.rotation.x = 0.0;
        transformStamped.transform.rotation.y = 0.0;
        transformStamped.transform.rotation.z = 0.0;
        transformStamped.transform.rotation.w = 1.0;

        // Broadcast the transform
        tf_broadcaster_.sendTransform(transformStamped);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CameraTfBroadcaster>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
