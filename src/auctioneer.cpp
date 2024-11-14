#include "auctioneer.hpp"
#include "constants.hpp"
#include <functional>

Auctioneer::Auctioneer() : Node("auctioneer")
{
    auctionPublisher = this->create_publisher<task_auction::msg::Task>(Topics::auctions, 10);

    bidSubscriber = this->create_subscription<task_auction::msg::Bid>(Topics::auctionBids, 10,std::bind(&Auctioneer::BidCallback, this, std::placeholders::_1));
    newTaskSubscriber = this->create_subscription<task_auction::msg::Task>(Topics::newTask, 10, std::bind(&Auctioneer::NewTaskCallback, this, std::placeholders::_1));
}


void Auctioneer::NewTaskCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg)
{
    auto timer = this->create_wall_timer(auctionDuration, std::bind(&Auctioneer::BidCompleteTimerCallback, this));
    Auction auction(msg->id, timer);
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
    }
    else
    {
        RCLCPP_WARN(this->get_logger(), "Recevied bid for inactive auction: '%ld' from client %ld", msg->task_id, msg->robot_id);
    }
}

void Auctioneer::BidCompleteTimerCallback()
{
  //TODO: How do we link this its auction??
}