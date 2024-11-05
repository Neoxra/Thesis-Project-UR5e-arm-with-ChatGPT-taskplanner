#include <rclcpp/rclcpp.hpp>
#include <visualization_msgs/msg/marker_array.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>
#include <unordered_map>
#include <set>

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

        collision_pub_ = this->create_publisher<moveit_msgs::msg::CollisionObject>("collision_object", 10);

        RCLCPP_INFO(this->get_logger(), "Dynamic Collision Updater Node initiated.");
    }

private:
    rclcpp::Subscription<visualization_msgs::msg::MarkerArray>::SharedPtr marker_sub_;
    tf2_ros::Buffer tf_buffer_;
    tf2_ros::TransformListener tf_listener_;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface_;
    rclcpp::Publisher<moveit_msgs::msg::CollisionObject>::SharedPtr collision_pub_;
    std::unordered_map<int, moveit_msgs::msg::CollisionObject> existing_objects_;  // Stores existing objects

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
                // Create a CollisionObject based on the Marker
                moveit_msgs::msg::CollisionObject collision_object;
                collision_object.id = "collision_object_" + std::to_string(marker.id);
                collision_object.header.frame_id = "base_link"; // Replace with your planning frame
                collision_object.operation = moveit_msgs::msg::CollisionObject::ADD;

                // Set the primitive as a sphere
                shape_msgs::msg::SolidPrimitive primitive;
                primitive.type = shape_msgs::msg::SolidPrimitive::SPHERE;
                primitive.dimensions.resize(1);
                primitive.dimensions[0] = marker.scale.x / 2; // Radius

                // Add the primitive and pose
                auto transformed_pose = transformPose(marker.pose, marker.header.frame_id, "base_link");
                if (transformed_pose == geometry_msgs::msg::Pose()) continue;

                collision_object.primitives.push_back(primitive);
                collision_object.primitive_poses.push_back(transformed_pose);

                // Publish and add to existing_objects_ map
                planning_scene_interface_.applyCollisionObject(collision_object);
                existing_objects_[marker.id] = collision_object;  // Store as CollisionObject instead of Marker
            }
        }

        // Remove obsolete objects
        for (auto it = existing_objects_.begin(); it != existing_objects_.end();)
        {
            if (current_ids.find(it->first) == current_ids.end())
            {
                removeCollisionObject(it->first);
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
        if (existing_objects_.count(marker_id) > 0)
        {
            RCLCPP_INFO(this->get_logger(), "Removing object with ID %d", marker_id);

            moveit_msgs::msg::CollisionObject collision_object;
            collision_object.id = "collision_object_" + std::to_string(marker_id);
            collision_object.header.frame_id = "base_link";  // Replace with your planning frame
            collision_object.operation = moveit_msgs::msg::CollisionObject::REMOVE;

            collision_pub_->publish(collision_object);
            existing_objects_.erase(marker_id);  // Remove from the map
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Tried to remove object with ID %d, but it does not exist.", marker_id);
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
