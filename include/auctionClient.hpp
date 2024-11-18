#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/task.hpp"
#include "task_auction/msg/bid.hpp"
#include <random>

class AuctionClient : public rclcpp::Node
{
public:
    AuctionClient();

private:
    int robotId;
    std::mt19937 generator;
    std::uniform_int_distribution<unsigned int> distribute;
    rclcpp::Subscription<task_auction::msg::Task>::SharedPtr auctionSubscriber;
    rclcpp::Publisher<task_auction::msg::Bid>::SharedPtr bidPublisher;
    void AuctionCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg);
};