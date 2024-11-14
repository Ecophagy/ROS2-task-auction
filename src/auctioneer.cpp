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
    // New task received, so start a new auction
    auctions[msg->id] = {};
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
        auctions[msg->task_id].push_back(bid);
    }
    else
    {
        RCLCPP_WARN(this->get_logger(), "Recevied bid for inactive auction: '%d' from client %d", msg->task_id, msg->robot_id);
    }
}