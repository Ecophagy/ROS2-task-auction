#include "rclcpp/rclcpp.hpp"
#include "auctionClient.hpp"

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AuctionClient>());
    rclcpp::shutdown();
    return 0;
}
