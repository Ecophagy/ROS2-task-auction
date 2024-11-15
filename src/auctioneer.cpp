#include "auctioneer.hpp"
#include "constants.hpp"
#include <functional>

Auctioneer::Auctioneer() : Node("auctioneer")
{
    numberOfClients = 1; // TODO: Add a service for clients to "register"
    auctionPublisher = this->create_publisher<task_auction::msg::Task>(Topics::auctions, 10);
    winningBidPublisher = this->create_publisher<task_auction::msg::Bid>(Topics::auctionWinner, 10);

    bidSubscriber = this->create_subscription<task_auction::msg::Bid>(Topics::auctionBids, 10,std::bind(&Auctioneer::BidCallback, this, std::placeholders::_1));
    newTaskSubscriber = this->create_subscription<task_auction::msg::Task>(Topics::newTask, 10, std::bind(&Auctioneer::NewTaskCallback, this, std::placeholders::_1));
}


void Auctioneer::NewTaskCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg)
{
    Auction auction(msg->id);
    auctions[msg->id] = auction;

    // Notify clients of auction
    auto newTaskMsg = task_auction::msg::Task();
    newTaskMsg.id = msg->id;
    newTaskMsg.description = msg->description;
    auctionPublisher->publish(newTaskMsg);
}

void Auctioneer::BidCallback(std::unique_ptr<task_auction::msg::Bid, std::default_delete<task_auction::msg::Bid>> msg)
{
    // Is the auction active?
    if(auctions.find(msg->task_id) != auctions.end())
    {
        auto bid = task_auction::msg::Bid();
        bid.task_id = msg->task_id;
        bid.robot_id = msg->robot_id;
        bid.bid_value = msg->bid_value;
        auctions[msg->task_id].addBid(bid);

        // If we have recevied a bit from all clients, the auction is over
        if(auctions[msg->task_id].getNumberOfBids() == numberOfClients)
        {
            auto winningBid = auctions[msg->task_id].getWinningBid();
            winningBidPublisher->publish(winningBid); // TODO: This should be a service not a topic
            auctions.erase(msg->task_id);
        }
    }
    else
    {
        RCLCPP_WARN(this->get_logger(), "Recevied bid for inactive auction: '%ld' from client %ld", msg->task_id, msg->robot_id);
    }
}
