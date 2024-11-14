#include <vector>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/bid.hpp"

class Auction
{
public:
    Auction() = default;
    Auction(int taskId, rclcpp::TimerBase::SharedPtr auctionTimer);
    void addBid(task_auction::msg::Bid);
    task_auction::msg::Bid getWinningBid();

private:
    int taskId;
    std::vector<task_auction::msg::Bid> bids;
    rclcpp::TimerBase::SharedPtr auctionTimer;
};