#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/task.hpp"
#include "task_auction/msg/bid.hpp"
#include "auction.hpp"
#include <map>
#include <vector>
#include <chrono>

using namespace std::chrono_literals;

class Auctioneer : public rclcpp::Node
{
public:
    Auctioneer();

private:
    const std::chrono::seconds auctionDuration = 60s;
    std::map<int, Auction> auctions;

    void NewTaskCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg);
    void BidCallback(std::unique_ptr<task_auction::msg::Bid, std::default_delete<task_auction::msg::Bid>> msg);
    void BidCompleteTimerCallback(); // TODO: How to link this to the auction?

    rclcpp::Subscription<task_auction::msg::Task>::SharedPtr newTaskSubscriber;
    rclcpp::Publisher<task_auction::msg::Task>::SharedPtr auctionPublisher;
    rclcpp::Subscription<task_auction::msg::Bid>::SharedPtr bidSubscriber;
};