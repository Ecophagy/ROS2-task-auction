#include "auctionClient.hpp"
#include "constants.hpp"
#include <functional>

AuctionClient::AuctionClient() : Node ("auctionClient")
{
    std::random_device os_seed;
    const auto seed = os_seed();
    generator = std::mt19937(seed);
    distribute = std::uniform_int_distribution<unsigned int>(1,10);

    this->declare_parameter(Parameters::robotId, 0);
    robotId = this->get_parameter(Parameters::robotId).as_int();
    bidPublisher = this->create_publisher<task_auction::msg::Bid>(Topics::auctionBids, 1);
    auctionSubscriber = this->create_subscription<task_auction::msg::Task>(Topics::auctions, 10, std::bind(&AuctionClient::AuctionCallback, this, std::placeholders::_1));
}

void AuctionClient::AuctionCallback(std::unique_ptr<task_auction::msg::Task, std::default_delete<task_auction::msg::Task>> msg)
{
    auto bid = task_auction::msg::Bid();
    bid.task_id = msg->id;
    bid.robot_id = robotId;
    bid.bid_value = distribute(generator); // Bid value is random value uniformly distributed between 1 and 10.
    RCLCPP_INFO(this->get_logger(), "Submitting bid for task %ld of %ld", msg->id, bid.bid_value);
    bidPublisher->publish(bid);
}