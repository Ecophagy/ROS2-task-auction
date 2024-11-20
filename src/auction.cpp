#include "auction.hpp"

Auction::Auction(int taskId, rclcpp::Time auctionStartTime) : taskId(taskId), auctionStartTime(auctionStartTime)
{
    bids = {};
}

void Auction::addBid(task_auction::msg::Bid bid)
{
    bids.push_back(bid);
}

int Auction::getNumberOfBids()
{
    return bids.size();
}

task_auction::msg::Bid Auction::getWinningBid()
{
    // The lowest bid is the winner
    auto bestBid = bids.front(); // FIXME: Undefined behaviour on empty vector
    for (const auto &bid : bids)
    {
        if (bid.bid_value < bestBid.bid_value)
        {
            bestBid = bid;
        }
    }
    return bestBid;
}

bool Auction::auctionExpired(const rclcpp::Time now)
{
    return now >= auctionStartTime + auctionDuration;
}