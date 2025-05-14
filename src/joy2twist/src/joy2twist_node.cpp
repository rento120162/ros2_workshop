#include <functional>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/joy.hpp"

using namespace std::chrono_literals;

class JoyTransrateNode : public rclcpp::Node{
public:
    std::string type_;
    double liner_x;
    double liner_y;

    JoyTransrateNode() : Node("joy2twist_node"){
        declare_parameter("max_liner_x", -1.0);
        declare_parameter("max_liner_y", -1.0);
        liner_x = get_parameter("max_liner_x").as_double();
        liner_y = get_parameter("max_liner_y").as_double();
        RCLCPP_INFO(this->get_logger(), "max liner_x:%f\r\n",liner_x);
        RCLCPP_INFO(this->get_logger(), "max liner_y:%f\r\n",liner_y);

        publisher = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

        auto callback = [this](const sensor_msgs::msg::Joy &msg) -> void {
            auto message = geometry_msgs::msg::Twist();

            message.linear.x = liner_x * msg.axes[1];
            message.linear.y = liner_y * msg.axes[2];

            this->publisher->publish(message);
        };

        subscriber = this->create_subscription<sensor_msgs::msg::Joy>("joy", 10, callback);
    }
private:
    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscriber;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JoyTransrateNode>());
    rclcpp::shutdown();
    return 0;
}