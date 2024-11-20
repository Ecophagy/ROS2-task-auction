#include "auctioneer.hpp"
#include "constants.hpp"
#include <functional>

Auctioneer::Auctioneer() : Node("auctioneer")
{
    auctionPublisher = this->create_publisher<task_auction::msg::Task>(Topics::auctions, 10);
    winningBidPublisher = this->create_publisher<task_auction::msg::Bid>(Topics::auctionWinner, 10);

    bidSubscriber = this->create_subscription<task_auction::msg::Bid>(Topics::auctionBids, 10,std::bind(&Auctioneer::BidCallback, this, std::placeholders::_1));
    newTaskSubscriber = this->create_subscription<task_auction::msg::Task>(Topics::newTask, 10, std::bind(&Auctioneer::NewTaskCallback, this, std::placeholders::_1));
}


void Auctioneer::NewTaskCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg)
{
    RCLCPP_INFO(this->get_logger(), "Creating new auction for task %ld", msg->id);
    Auction auction(msg->id, this->get_clock()->now());
    auctions[msg->id] = auction;

    // Notify clients of auction
    auto newTaskMsg = task_auction::msg::Task();
    newTaskMsg.id = msg->id;
    newTaskMsg.description = msg->description;
    auctionPublisher->publish(newTaskMsg);

    auctionTimer = this->create_wall_timer(auctionTimeoutCheck, std::bind(&Auctioneer::AuctionTimeoutCallback, this));
}

void Auctioneer::BidCallback(std::unique_ptr<task_auction::msg::Bid, std::default_delete<task_auction::msg::Bid>> msg)
{
    // Is the auction active?
    if(auctions.find(msg->task_id) != auctions.end())
    {
        RCLCPP_INFO(this->get_logger(), "Bid for task %ld received from robot #%ld for %ld", msg->task_id, msg->robot_id, msg->bid_value);
        auto bid = task_auction::msg::Bid();
        bid.task_id = msg->task_id;
        bid.robot_id = msg->robot_id;
        bid.bid_value = msg->bid_value;
        auctions[msg->task_id].addBid(bid);
    }
    else
    {
        RCLCPP_WARN(this->get_logger(), "Recevied bid for inactive auction: '%ld' from client %ld", msg->task_id, msg->robot_id);
    }
}

void Auctioneer::AuctionTimeoutCallback()
{
    RCLCPP_INFO(this->get_logger(), "Checking for auction expiry...");
    const auto now = this->get_clock()->now();

    // Check every auction for time expiry
    // We have to use the iterator to allow us to delete a value while iterating over the list
    auto auctionIterator = auctions.begin();
    while (auctionIterator != auctions.end())
    {
        if (auctionIterator->second.auctionExpired(now))
        {
            RCLCPP_INFO(this->get_logger(), "Auction for task %d expired", auctionIterator->first);
            auto winningBid = auctionIterator->second.getWinningBid();
            RCLCPP_INFO(this->get_logger(), "Winning bid for task %d is %ld by robot #%ld", auctionIterator->first, winningBid.bid_value, winningBid.robot_id);
            winningBidPublisher->publish(winningBid); // TODO: This should be a service not a topic
            auctionIterator = auctions.erase(auctionIterator++);
        }
        else
        {
            ++auctionIterator;
        }
    }
}