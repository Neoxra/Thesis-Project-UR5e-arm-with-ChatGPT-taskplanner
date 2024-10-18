// ChatGPT was used to aid in the wriing of this code.
#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>

// Simple Pose class to encapsulate position and orientation
class Pose {
public:
    Pose(double px, double py, double pz, double r, double p, double y)
        : x(px), y(py), z(pz), roll(r), pitch(p), yaw(y) {}

    double x, y, z;  
    double roll, pitch, yaw;  
};

class InventoryTransformations : public rclcpp::Node {
public:
    InventoryTransformations() : Node("inventory_transformations")
    {
        // Initialize the static transform broadcaster
        tf_static_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

        RCLCPP_INFO(this->get_logger(), "Inventory Transformations service is ready.");

        // Publish transforms for each inventory slot
        publish_transform("base_link", "inventory_slot_1", Pose(0.25, 0.25, 0.0, 0.0, 0.0, M_PI / 4));
        publish_transform("base_link", "inventory_slot_2", Pose(0.25, 0.0, 0.0, 0.0, 0.0, 0.0));
        publish_transform("base_link", "inventory_slot_3", Pose(0.25, -0.25, 0.0, 0.0, 0.0, -M_PI / 4));
    }

private:
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_static_broadcaster_;

    // Function to broadcast a static transform between parent and child frames
    void publish_transform(const std::string &parent_frame, const std::string &child_frame, const Pose &pose)
    {
        geometry_msgs::msg::TransformStamped transformStamped_;

        // Set timestamp and frames
        transformStamped_.header.stamp = this->get_clock()->now();
        transformStamped_.header.frame_id = parent_frame;
        transformStamped_.child_frame_id = child_frame;

        // Set position
        transformStamped_.transform.translation.x = pose.x;
        transformStamped_.transform.translation.y = pose.y;
        transformStamped_.transform.translation.z = pose.z;

        // Convert roll, pitch, yaw to quaternion and set orientation
        tf2::Quaternion q;
        q.setRPY(pose.roll, pose.pitch, pose.yaw);
        transformStamped_.transform.rotation.x = q.x();
        transformStamped_.transform.rotation.y = q.y();
        transformStamped_.transform.rotation.z = q.z();
        transformStamped_.transform.rotation.w = q.w();

        // Broadcast the transform
        tf_static_broadcaster_->sendTransform(transformStamped_);
    }
};

// Main function that initializes and spins the node
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<InventoryTransformations>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
