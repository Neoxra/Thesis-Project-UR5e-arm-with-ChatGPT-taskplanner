#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <std_msgs/msg/string.hpp>  // For String messages
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <unordered_map>
#include <chrono>

const std::string PLANNING_FRAME = "base_link";

class DynamicCollisionUpdater : public rclcpp::Node
{
public:
    DynamicCollisionUpdater()
        : Node("dynamic_collision_updater"),
          tf_buffer_(this->get_clock()),
          tf_listener_(tf_buffer_),
          planning_scene_interface_(std::make_shared<moveit::planning_interface::PlanningSceneInterface>()),
          object_timeout_(std::chrono::milliseconds(250)) // Set timeout to 0.25 seconds     
    {
        callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);

        rclcpp::SubscriptionOptions subscription_options;
        subscription_options.callback_group = callback_group_;

        collision_info_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/yolo/collision_info", rclcpp::QoS(10));

        marker_sub_ = this->create_subscription<visualization_msgs::msg::MarkerArray>(
            "/yolo/dgb_kp_markers", rclcpp::QoS(10),
            std::bind(&DynamicCollisionUpdater::markerCallback, this, std::placeholders::_1),
            subscription_options);

        collision_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&DynamicCollisionUpdater::checkForExpiredObjects, this),
            callback_group_);

        RCLCPP_INFO(this->get_logger(), "Dynamic Collision Updater Node initiated.");
    }

private:
    rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr marker_sub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr collision_info_pub_;
    rclcpp::TimerBase::SharedPtr collision_timer_;
    rclcpp::CallbackGroup::SharedPtr callback_group_;
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_interface_;
    std::unordered_map<int, std::chrono::steady_clock::time_point> existing_objects_;
    std::chrono::milliseconds object_timeout_;

    void markerCallback(const visualization_msgs::msg::MarkerArray::SharedPtr marker_array)
    {
        auto now = std::chrono::steady_clock::now();
         
        for (const auto &marker : marker_array->markers)
        {
            existing_objects_[marker.id] = now;
            updateCollisionObject(marker);
        }
    }

    void updateCollisionObject(const visualization_msgs::msg::Marker &marker)
    {
        geometry_msgs::msg::Pose transformed_pose = transformPose(
            marker.pose, marker.header.frame_id, PLANNING_FRAME);

        if (transformed_pose == geometry_msgs::msg::Pose())
            return;

        moveit_msgs::msg::CollisionObject collision_object;
        collision_object.id = "collision_object_" + std::to_string(marker.id);
        collision_object.header.frame_id = PLANNING_FRAME;
        collision_object.operation = moveit_msgs::msg::CollisionObject::ADD;

        shape_msgs::msg::SolidPrimitive primitive;
        primitive.type = shape_msgs::msg::SolidPrimitive::SPHERE;
        primitive.dimensions.resize(1);
        primitive.dimensions[0] = marker.scale.x / 2;

        collision_object.primitives.push_back(primitive);
        collision_object.primitive_poses.push_back(transformed_pose);

        planning_scene_interface_->applyCollisionObject(collision_object);
        RCLCPP_INFO(this->get_logger(), "Updated collision object with ID %d", marker.id);
    }

    void checkForExpiredObjects()
    {
        auto now = std::chrono::steady_clock::now();

        for (auto it = existing_objects_.begin(); it != existing_objects_.end();)
        {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - it->second) > object_timeout_)
            {
                // Object has timed out, so remove it
                if (removeCollisionObject(it->first))  // Only erase if successfully removed
                {
                    it = existing_objects_.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            else
            {
                ++it;
            }
        }
    }

    bool removeCollisionObject(int marker_id)
    {
        moveit_msgs::msg::CollisionObject collision_object;
        collision_object.id = "collision_object_" + std::to_string(marker_id);
        collision_object.header.frame_id = PLANNING_FRAME;
        collision_object.operation = moveit_msgs::msg::CollisionObject::REMOVE;

        planning_scene_interface_->applyCollisionObject(collision_object);
        RCLCPP_INFO(this->get_logger(), "Removed collision object with ID %d", marker_id);

        // Return true to confirm the object removal (could add error checks if desired)
        return true;
    }

    geometry_msgs::msg::Pose transformPose(
        const geometry_msgs::msg::Pose &pose,
        const std::string &from_frame,
        const std::string &to_frame)
    {
        geometry_msgs::msg::PoseStamped pose_stamped;
        pose_stamped.header.frame_id = from_frame;
        pose_stamped.pose = pose;

        try
        {
            auto transform = tf_buffer_.lookupTransform(
                to_frame, from_frame, tf2::TimePointZero);
            geometry_msgs::msg::PoseStamped transformed_pose;
            tf2::doTransform(pose_stamped, transformed_pose, transform);
            return transformed_pose.pose;
        }
        catch (const tf2::TransformException &ex)
        {
            RCLCPP_ERROR(this->get_logger(), "Transform failed: %s", ex.what());
            return geometry_msgs::msg::Pose();
        }
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DynamicCollisionUpdater>();

    // Use MultiThreadedExecutor for simultaneous callbacks
    rclcpp::executors::MultiThreadedExecutor executor;
    executor.add_node(node);
    executor.spin();

    rclcpp::shutdown();
    return 0;
}
