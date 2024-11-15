#include <vector>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "task_auction/msg/bid.hpp"

class Auction
{
public:
    Auction() = default;
    Auction(int taskId);
    void addBid(task_auction::msg::Bid);
    int getNumberOfBids();
    task_auction::msg::Bid getWinningBid();

private:
    int taskId;
    std::vector<task_auction::msg::Bid> bids;
};