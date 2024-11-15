#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/task.hpp"
#include "task_auction/msg/bid.hpp"
#include "auction.hpp"
#include <map>
#include <vector>

class Auctioneer : public rclcpp::Node
{
public:
    Auctioneer();

private:
    std::map<int, Auction> auctions;
    int numberOfClients;

    void NewTaskCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg);
    void BidCallback(std::unique_ptr<task_auction::msg::Bid, std::default_delete<task_auction::msg::Bid>> msg);

    rclcpp::Subscription<task_auction::msg::Task>::SharedPtr newTaskSubscriber;
    rclcpp::Publisher<task_auction::msg::Task>::SharedPtr auctionPublisher;
    rclcpp::Subscription<task_auction::msg::Bid>::SharedPtr bidSubscriber;
    rclcpp::Publisher<task_auction::msg::Bid>::SharedPtr winningBidPublisher;
};