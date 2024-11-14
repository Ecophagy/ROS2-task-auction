#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/task.hpp"

class Auctioneer : public rclcpp::Node
{
public:
    Auctioneer();

private:
    rclcpp::Subscription<task_auction::msg::Task>::SharedPtr newTaskSubscriber;
};