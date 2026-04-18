import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32

class PumpPublisher(Node):
    def __init__(self):
        super().__init__('pump_publisher')
        self.publisher_ = self.create_publisher(Int32, 'set_pump_throttle', 10)
        self.get_logger().info('Pump publisher node started')

    def publish_pump_throttle(self, throttle):
        msg = Int32()
        msg.data = int(throttle)
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing pump throttle: {throttle}')

def main():
    rclpy.init()
    pump_publisher = PumpPublisher()

    try:
        while rclpy.ok():
            throttle = int(input("Enter pump throttle: "))
            pump_publisher.publish_pump_throttle(throttle)
    except KeyboardInterrupt:
        pass

    pump_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()