#include "rclcpp/rclcpp.hpp"
#include "auctionClient.hpp"

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AuctionClient>(1)); // TODO: get id from launch parameter
    rclcpp::shutdown();
    return 0;
}
