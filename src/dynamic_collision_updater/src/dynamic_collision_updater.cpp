#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/planning_scene_monitor/planning_scene_monitor.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <moveit/robot_state/robot_state.h>
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
          object_timeout_(std::chrono::milliseconds(250)) // Set timeout to 0.5 seconds
    {
        collision_info_pub_ = this->create_publisher<std_msgs::msg::String>(
            "/yolo/collision_info", 10);
        marker_sub_ = this->create_subscription<visualization_msgs::msg::MarkerArray>(
            "/yolo/dgb_kp_markers", 10,
            std::bind(&DynamicCollisionUpdater::markerCallback, this, std::placeholders::_1));

        collision_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), // Check for timed-out objects every 0.1 seconds
            std::bind(&DynamicCollisionUpdater::checkForExpiredObjects, this));

        planning_scene_interface_ = std::make_shared<moveit::planning_interface::PlanningSceneInterface>();

        // // Initialize PlanningSceneMonitor with necessary arguments
        // planning_scene_monitor_ = std::make_shared<planning_scene_monitor::PlanningSceneMonitor>(
        //     shared_from_this(), "robot_description");

        // if (!planning_scene_monitor_->getPlanningScene())
        // {
        //     RCLCPP_ERROR(this->get_logger(), "Failed to initialize PlanningSceneMonitor.");
        // }
        // else
        // {
        //     planning_scene_monitor_->startSceneMonitor();
        //     planning_scene_monitor_->startStateMonitor();
        //     planning_scene_monitor_->startWorldGeometryMonitor();
        // }

        RCLCPP_INFO(this->get_logger(), "Dynamic Collision Updater Node initiated.");
    }

private:
    rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr marker_sub_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr collision_info_pub_;
    rclcpp::TimerBase::SharedPtr collision_timer_;
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    std::shared_ptr<moveit::planning_interface::PlanningSceneInterface> planning_scene_interface_;
    std::shared_ptr<planning_scene_monitor::PlanningSceneMonitor> planning_scene_monitor_;
    std::unordered_map<int, std::chrono::steady_clock::time_point> existing_objects_;
    std::chrono::milliseconds object_timeout_;

    void markerCallback(const visualization_msgs::msg::MarkerArray::SharedPtr marker_array)
    {
        auto now = std::chrono::steady_clock::now();
        for (const auto &marker : marker_array->markers)
        {
            existing_objects_[marker.id] = now;
            updateCollisionObject(marker);

            // // Check for collisions and publish if detected
            // if (checkCollision())
            // {
            //     std_msgs::msg::String collision_msg;
            //     collision_msg.data = "Collision detected with marker ID: " + std::to_string(marker.id);
            //     collision_info_pub_->publish(collision_msg);
            //     RCLCPP_INFO(this->get_logger(), "Collision detected with marker ID: %d", marker.id);
            // }
        }
    }

    void updateCollisionObject(const visualization_msgs::msg::Marker &marker)
    {
        geometry_msgs::msg::Pose transformed_pose = transformPose(
            marker.pose, marker.header.frame_id, PLANNING_FRAME); // Replace with your planning frame

        if (transformed_pose == geometry_msgs::msg::Pose())
            return;

        moveit_msgs::msg::CollisionObject collision_object;
        collision_object.id = "collision_object_" + std::to_string(marker.id);
        collision_object.header.frame_id = PLANNING_FRAME; // Replace with your planning frame
        collision_object.operation = moveit_msgs::msg::CollisionObject::ADD;

        shape_msgs::msg::SolidPrimitive primitive;
        primitive.type = shape_msgs::msg::SolidPrimitive::SPHERE;
        primitive.dimensions.resize(1);
        primitive.dimensions[0] = marker.scale.x / 2; // Radius

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
                // Object has timed out
                removeCollisionObject(it->first);
                it = existing_objects_.erase(it); // Remove from map and advance iterator
            }
            else
            {
                ++it; // Move to the next object
            }
        }
    }

    void removeCollisionObject(int marker_id)
    {
        moveit_msgs::msg::CollisionObject collision_object;
        collision_object.id = "collision_object_" + std::to_string(marker_id);
        collision_object.header.frame_id = PLANNING_FRAME; // Replace with your planning frame
        collision_object.operation = moveit_msgs::msg::CollisionObject::REMOVE;

        planning_scene_interface_->applyCollisionObject(collision_object);
        RCLCPP_INFO(this->get_logger(), "Removed collision object with ID %d", marker_id);
    }

    bool checkCollision()
    {
        // Using ScopedLock to handle scene read lock
        planning_scene_monitor::LockedPlanningSceneRO planning_scene_lock(planning_scene_monitor_);

        collision_detection::CollisionRequest collision_request;
        collision_detection::CollisionResult collision_result;
        collision_request.contacts = true;
        collision_request.max_contacts = 1;

        auto planning_scene = planning_scene_monitor_->getPlanningScene();
        const moveit::core::RobotState &current_state = planning_scene->getCurrentState();

        planning_scene->checkCollision(collision_request, collision_result, current_state);

        return collision_result.collision;
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
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
