#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <functional>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float64.hpp"


using namespace std::chrono_literals;

class Node1: public rclcpp::Node{

    private: 
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;

        void msg(){
            std_msgs::msg::Float64 message;
            message.data = 50.0;
            RCLCPP_INFO(this->get_logger(), "Publicando: '%lf'", message.data);
            publisher_->publish(message);
        }
        
    public:
        Node1(): Node("node_1"){
            publisher_ = this->create_publisher<std_msgs::msg::Float64>("topic_1", 10);
            timer_ = this->create_wall_timer(500ms, std::bind(&Node1::msg, this));
        }
};

int main(int argc, char * argv[]){

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Node1>());
    rclcpp::shutdown();
    

    return 0;
}