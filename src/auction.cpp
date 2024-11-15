#include "auction.hpp"

Auction::Auction(int taskId) : taskId(taskId)
{
    bids = {};
}

void Auction::addBid(task_auction::msg::Bid bid)
{
    bids.push_back(bid);
}

task_auction::msg::Bid Auction::getWinningBid()
{
    // TODO;
}