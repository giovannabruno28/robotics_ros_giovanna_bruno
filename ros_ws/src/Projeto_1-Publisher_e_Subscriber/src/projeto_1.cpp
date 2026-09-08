#include <iostream>
#include <cmath>
#include <memory>
#include <string>
#include <chrono>
#include <functional>  
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/vector3.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

class NodeFunction: public rclcpp::Node{

    private:
        rclcpp::Subscription<geometry_msgs::msg::Vector3>::SharedPtr subscription_goal;
        rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr subscription_rb_pos;
        geometry_msgs::msg::Vector3::SharedPtr goal_ptr;
        geometry_msgs::msg::Pose::SharedPtr rb_pos_ptr;

        rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;


        void topic_callback_goal(const geometry_msgs::msg::Vector3::SharedPtr msg){
            goal_ptr = msg;
        }

        void topic_callback_rb_pos(const geometry_msgs::msg::Pose::SharedPtr msg){
            rb_pos_ptr = msg;
        }

        void msg(){
            geometry_msgs::msg::TwistStamped message;

            if (goal_ptr == nullptr || rb_pos_ptr == nullptr) {
                message.twist.linear.x = 0.0;
                message.twist.angular.z = 0.0;
                message.header.stamp = this->now();
                publisher_->publish(message);
                return;
            }

            message = *calculate_vel();
            message.header.stamp = this->now(); //adiciona no header do TwistStamped o tempo atual

            publisher_->publish(message);
        }

    public:

        NodeFunction() : Node("project_1")
        {
            subscription_goal = this->create_subscription<geometry_msgs::msg::Vector3>(
                "goal", 10,
                std::bind(&NodeFunction::topic_callback_goal, this, _1)
            );

            subscription_rb_pos = this->create_subscription<geometry_msgs::msg::Pose>(
                "robot_position", 10,
                std::bind(&NodeFunction::topic_callback_rb_pos, this, _1)
            );

            publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("cmd_vel", 10);
            timer_ = this->create_wall_timer(100ms, std::bind(&NodeFunction::msg, this) );
        }

        geometry_msgs::msg::TwistStamped::SharedPtr calculate_vel (){
            geometry_msgs::msg::TwistStamped::SharedPtr result = std::make_shared<geometry_msgs::msg::TwistStamped>();

            float dx = goal_ptr->x - rb_pos_ptr->position.x;
            float dy = goal_ptr->y - rb_pos_ptr->position.y;
            float theta_goal = std::atan2(dy, dx);
            
            double yaw = std::atan2(
                2.0 * (rb_pos_ptr->orientation.w * rb_pos_ptr->orientation.z +
                    rb_pos_ptr->orientation.x * rb_pos_ptr->orientation.y),
                1.0 - 2.0 * (rb_pos_ptr->orientation.y * rb_pos_ptr->orientation.y +
                            rb_pos_ptr->orientation.z * rb_pos_ptr->orientation.z)
            );

            double angle_error = std::atan2(std::sin(theta_goal - yaw), std::cos(theta_goal - yaw));

            if (angle_error > 1.0){
                angle_error = 1.0;
            } else if (angle_error < 0.5 && angle_error > -0.5){
                angle_error = 0.0;
            } else if (angle_error < -1.0){
                angle_error = -1.0;
            } else {
                angle_error *= 0.5;
            }

            result->twist.angular.z = angle_error;

            float real_dist= std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
            real_dist *= 0.5;

            if (real_dist > 1.0){
                real_dist = 1.0;
            } else if (real_dist < 0.5 && real_dist > -0.5){
                real_dist = 0.0;
            } else if (real_dist < -1.0){
                real_dist = -1.0;
            }

            result->twist.linear.x = real_dist;

            return result;
        }
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<NodeFunction>());
    rclcpp::shutdown();
    return 0;
}

