#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>

class DynamicCollisionUpdater : public rclcpp::Node
{
public:
    DynamicCollisionUpdater()
        : Node("dynamic_collision_updater"),
          tf_buffer_(this->get_clock()),
          tf_listener_(tf_buffer_)
    {
        marker_sub_ = this->create_subscription<visualization_msgs::msg::MarkerArray>(
            "/yolo/dgb_kp_markers", 10,
            std::bind(&DynamicCollisionUpdater::markerCallback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Dynamic Collision Updater Node initiated.");
    }

private:
    void markerCallback(const visualization_msgs::msg::MarkerArray::SharedPtr marker_array)
    {
        std::set<int> current_ids;
        for (const auto &marker : marker_array->markers)
        {
            current_ids.insert(marker.id);
            if (existing_objects_.count(marker.id))
            {
                updateCollisionObject(marker);
            }
            else
            {
                addCollisionObject(marker);
                existing_objects_.insert(marker.id);
            }
        }

        // Remove obsolete objects
        for (auto it = existing_objects_.begin(); it != existing_objects_.end();)
        {
            if (current_ids.find(*it) == current_ids.end())
            {
                removeCollisionObject(*it);
                it = existing_objects_.erase(it);
            }
            else
            {
                ++it;
            }
        }
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

    void addCollisionObject(const visualization_msgs::msg::Marker &marker)
    {
        auto transformed_pose = transformPose(
            marker.pose, marker.header.frame_id, "base_link"); // Replace with your planning frame
        if (transformed_pose == geometry_msgs::msg::Pose())
            return;

        moveit_msgs::msg::CollisionObject collision_object;
        collision_object.id = "collision_object_" + std::to_string(marker.id);
        collision_object.header.frame_id = "base_link"; // Replace with your planning frame
        collision_object.operation = moveit_msgs::msg::CollisionObject::ADD;

        shape_msgs::msg::SolidPrimitive primitive;
        primitive.type = shape_msgs::msg::SolidPrimitive::SPHERE;
        primitive.dimensions.resize(1);
        primitive.dimensions[0] = marker.scale.x / 2; // Radius

        collision_object.primitives.push_back(primitive);
        collision_object.primitive_poses.push_back(transformed_pose);

        planning_scene_interface_.applyCollisionObject(collision_object);
    }

    void updateCollisionObject(const visualization_msgs::msg::Marker &marker)
    {
        auto transformed_pose = transformPose(
            marker.pose, marker.header.frame_id, "base_link"); // Replace with your planning frame
        if (transformed_pose == geometry_msgs::msg::Pose())
            return;

        moveit_msgs::msg::CollisionObject collision_object;
        collision_object.id = "collision_object_" + std::to_string(marker.id);
        collision_object.header.frame_id = "base_link"; // Replace with your planning frame
        collision_object.operation = moveit_msgs::msg::CollisionObject::MOVE;

        collision_object.primitive_poses.push_back(transformed_pose);

        planning_scene_interface_.applyCollisionObject(collision_object);
    }

    void removeCollisionObject(int marker_id)
    {
        planning_scene_interface_.removeCollisionObjects(
            {"collision_object_" + std::to_string(marker_id)});
    }

    rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr marker_sub_;
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;
    std::set<int> existing_objects_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DynamicCollisionUpdater>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
