#include "auction.hpp"

Auction::Auction(int taskId) : taskId(taskId)
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