#include "auctionClient.hpp"
#include "constants.hpp"
#include <functional>


AuctionClient::AuctionClient(int robotId) : Node ("auctionClient"), robotId(robotId)
{
    bidPublisher = this->create_publisher<task_auction::msg::Bid>(Topics::auctionBids, 1);
    auctionSubscriber = this->create_subscription<task_auction::msg::Task>(Topics::auctions, 10, std::bind(&AuctionClient::AuctionCallback, this, std::placeholders::_1));
}

void AuctionClient::AuctionCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg)
{
    auto bid = task_auction::msg::Bid();
    bid.task_id = msg->id;
    bid.robot_id = robotId;
    // TODO: Caluculate and assign bid value
    bidPublisher->publish(bid);
}