#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/task.hpp"
#include "task_auction/msg/bid.hpp"
#include <map>
#include <vector>

class Auctioneer : public rclcpp::Node
{
public:
    Auctioneer();

private:
    std::map<int, std::vector<task_auction::msg::Bid>> auctions;

    void NewTaskCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg);

    rclcpp::Subscription<task_auction::msg::Task>::SharedPtr newTaskSubscriber;
    rclcpp::Publisher<task_auction::msg::Task>::SharedPtr auctionPublisher;
};