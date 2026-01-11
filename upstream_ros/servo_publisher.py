import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32

class ServoPublisher(Node):
    def __init__(self):
        super().__init__('servo_publisher')
        self.publisher_ = self.create_publisher(Int32, 'set_servo_us', 10)
        self.get_logger().info('Servo publisher node started')

    def publish_servo_us(self, us):
        msg = Int32()
        msg.data = int(us)
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing servo us: {us}')

def main():
    rclpy.init()
    servo_publisher = ServoPublisher()

    try:
        while rclpy.ok():
            us = int(input("Enter servo us: "))
            servo_publisher.publish_servo_us(us)
    except KeyboardInterrupt:
        pass

    servo_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()