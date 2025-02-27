// ChatGPT was used to aid in the wriing of this code.
#include "rclcpp/rclcpp.hpp"
#include "interfaces/srv/inventory.hpp"             // Custom Inventory service
#include "interfaces/msg/inventory_status.hpp"      // InventoryStatus message
#include "visualization_msgs/msg/marker.hpp"        // Marker for RViz visualization
#include "visualization_msgs/msg/marker_array.hpp"   // MarkerArray 


class InventoryManager : public rclcpp::Node
{
public:
  InventoryManager() : Node("inventory_manager")
  {
    // Initialize inventory slots (-1 means empty)
    slots_[0] = -1; slots_[1] = -1; slots_[2] = -1;

    // Create the service
    inventory_service_ = this->create_service<interfaces::srv::Inventory>(
      "inventory", std::bind(&InventoryManager::handle_inventory_request, this, std::placeholders::_1, std::placeholders::_2));

    // Create publisher for InventoryStatus
    status_publisher_ = this->create_publisher<interfaces::msg::InventoryStatus>("inventory_status", 10);

    // Create publisher for RViz visualization (Marker messages)
    marker_publisher_ = this->create_publisher<visualization_msgs::msg::MarkerArray>("inventory_marker", 10);

    // Timer to publish status and markers at 1Hz
    timer_ = this->create_wall_timer(
      std::chrono::seconds(1), std::bind(&InventoryManager::publish_inventory_status, this));

    RCLCPP_INFO(this->get_logger(), "Inventory Manager Node has been started.");
  }

private:
  int slots_[3];  // Array representing 3 inventory slots

  rclcpp::Service<interfaces::srv::Inventory>::SharedPtr inventory_service_;
  rclcpp::Publisher<interfaces::msg::InventoryStatus>::SharedPtr status_publisher_;
  rclcpp::Publisher<visualization_msgs::msg::MarkerArray>::SharedPtr marker_publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  // Handle inventory service requests
  void handle_inventory_request(
      const std::shared_ptr<interfaces::srv::Inventory::Request> request,
      std::shared_ptr<interfaces::srv::Inventory::Response> response)
  {
    RCLCPP_INFO(this->get_logger(), "Inventory request received for command: %s, item: %d", request->command.c_str(), request->item_id);

    if (request->item_id < 1 ) {
      response->response = -2;  // Invalid item ID
    } else {
      if (request->command == "put_in_inventory") {
        response->response = add_to_inventory(request->item_id);
      } else if (request->command == "get_from_inventory") {
        response->response = remove_from_inventory(request->item_id);
      } else {
        response->response = -2;  // Invalid command
      }
    }
   

    RCLCPP_INFO(this->get_logger(), "Request Complete %d: %s for item: %d", response->response, request->command.c_str(), request->item_id);
  }

  // Add item to the inventory
  int add_to_inventory(int item_id)
  {
    for (int i = 0; i < 3; ++i) {
      if (slots_[i] == -1) {  // Empty slot found
        slots_[i] = item_id;
        return i + 1;  // Return slot number (1-based)
      }
    }
    return -1;  // Inventory full
  }

  // Remove item from the inventory
  int remove_from_inventory(int item_id)
  {
    for (int i = 0; i < 3; ++i) {
      if (slots_[i] == item_id) {  // Item found in slot
        slots_[i] = -1;  // Empty the slot
        return i + 1;  // Return slot number (1-based)
      }
    }
    return -1;  // Item not found
  }

  // Publish the current inventory status
  void publish_inventory_status()
  {
    // Publish InventoryStatus message
    auto status_msg = interfaces::msg::InventoryStatus();
    status_msg.slot_1 = slots_[0];
    status_msg.slot_2 = slots_[1];
    status_msg.slot_3 = slots_[2];
    status_publisher_->publish(status_msg);

    publish_marker_status();
  }
  
  // Publish the current inventory status
  void publish_marker_status()
  {
    // CHATGPT: marker array query
    // Publish MarkerArray for RViz visualization
    auto marker_array = visualization_msgs::msg::MarkerArray();
    for (int i = 0; i < 3; ++i) {
      auto marker = visualization_msgs::msg::Marker();
      marker.header.frame_id = "base_link";
      marker.header.stamp = this->now();
      marker.ns = "inventory";
      marker.id = i;
      marker.type = visualization_msgs::msg::Marker::SPHERE;
      marker.action = visualization_msgs::msg::Marker::ADD;

      marker.pose.position.x = 0.25;
      marker.pose.position.y = 0.25 - (i * 0.25);  // Y positions for the slots
      marker.pose.position.z = 0.0;

      marker.scale.x = marker.scale.y = marker.scale.z = 0.1;

      marker.color.a = 1.0;    // Fully opaque
      if (slots_[i] == -1) {
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;  // Green if empty
      } else {
        marker.color.r = 1.0;
        marker.color.g = 0.0;
        marker.color.b = 0.0;  // Red if occupied
      }
      marker_array.markers.push_back(marker);
    }
    marker_publisher_->publish(marker_array);
  }
};


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<InventoryManager>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}