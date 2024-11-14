#include "rclcpp/rclcpp.hpp"

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    // Todo spin on auctioneer class
    rclcpp::shutdown();
    return 0;
}
