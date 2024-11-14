#include "auctioneer.hpp"

Auctioneer::Auctioneer() : Node("auctioneer")
{
    auto newTaskCallback =
      [this](task_auction::msg::Task::UniquePtr msg) -> void {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->description.c_str());
      };
   newTaskSubscriber = this->create_subscription<task_auction::msg::Task>("new_tasks", 10, newTaskCallback);
}