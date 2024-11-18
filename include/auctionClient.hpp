#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/task.hpp"
#include "task_auction/msg/bid.hpp"

class AuctionClient : public rclcpp::Node
{
public:
    AuctionClient(int robotId);

private:
    const int robotId;
    rclcpp::Subscription<task_auction::msg::Task>::SharedPtr auctionSubscriber;
    rclcpp::Publisher<task_auction::msg::Bid>::SharedPtr bidPublisher;
    void AuctionCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg);
};