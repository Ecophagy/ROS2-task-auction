#include "rclcpp/rclcpp.hpp"
#include "auctioneer.hpp"

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Auctioneer>());
    rclcpp::shutdown();
    return 0;
}
