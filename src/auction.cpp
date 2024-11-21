#include "auction.hpp"
#include <stdexcept>

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
    if (not bids.empty())
    {
        auto bestBid = bids.front();
        for (const auto &bid : bids)
        {
            if (bid.bid_value < bestBid.bid_value)
            {
                bestBid = bid;
            }
        }
        return bestBid;
    }
    else
    {
        // No bids made
        throw std::length_error("No bids made");
    }
}

bool Auction::auctionExpired(const rclcpp::Time now)
{
    return now >= auctionStartTime + auctionDuration;
}