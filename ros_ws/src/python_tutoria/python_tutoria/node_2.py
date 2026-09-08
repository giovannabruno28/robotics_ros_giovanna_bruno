#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float64

class Node2(Node):

    def __init__(self):
        
        super().__init__("node_2")

            self.get_logger().info("Subscriber started!")

            self.subscription = self.create_subscription(
                Float64,
                "topic_1",
                self.listener_callback,
                10
            )

            self.publisher_ = self.create_publisher(
                Float64,
                "topic_2",
                10
            )

            self.timer = self.create_timer(
                1.0,
                self.timer_callback
            )

            self.peso = 0.0
            self.imc = 0.0
        
        def listener_callback(self, msg):
            peso = msg.data

            
        

        def timer_callback(self):
            msg = Float64()
            msg.data = self.imc
            self.publisher_.publish(msg)
            self.get_logger().info(
            f"Publishing: {msg.data}"
        )

def main():

    rclpy.init()

    node2 = Node2()

    rclpy.spin(node2)

    node2.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()