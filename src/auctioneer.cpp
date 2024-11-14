#include "auctioneer.hpp"
#include "constants.hpp"
#include <functional>

Auctioneer::Auctioneer() : Node("auctioneer")
{
    auctionPublisher = this->create_publisher<task_auction::msg::Task>(Topics::auctions, 10);

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