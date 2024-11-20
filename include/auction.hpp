#include <vector>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/bid.hpp"

class Auction
{
public:
    Auction() = default;
    Auction(int taskId, rclcpp::Time auctionStartTime);
    void addBid(task_auction::msg::Bid);
    int getNumberOfBids();
    task_auction::msg::Bid getWinningBid();
    bool auctionExpired(const rclcpp::Time now);

private:
    int taskId;
    rclcpp::Time auctionStartTime;
    rclcpp::Duration auctionDuration = rclcpp::Duration(5, 0);
    std::vector<task_auction::msg::Bid> bids;
};