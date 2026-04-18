import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32

class PayloadBasicController(Node):
    def __init__(self, name):
        super().__init__(name)
        self.servo_publisher = self.create_publisher(Int32, 'set_servo_us', 10)
        self.pump_publisher = self.create_publisher(Int32, 'set_pump_throttle', 10)
        self.get_logger().info('Basic controller node started')

    def publish_pump_throttle(self, throttle):
        msg = Int32()
        msg.data = int(throttle)
        self.pump_publisher.publish(msg)
        self.get_logger().info(f'Publishing pump throttle: {throttle}')

    def publish_servo_us(self, us):
        msg = Int32()
        msg.data = int(us)
        self.servo_publisher.publish(msg)
        self.get_logger().info(f'Publishing servo us: {us}')

def main():
    rclpy.init()
    basic_controller = PayloadBasicController("basic_controller")

    try:
        while rclpy.ok():
            entry = input("Enter servo us (s{n}) or pump throttle ({n}): ")
            if entry.startswith('s'):
                basic_controller.publish_servo_us(int(entry[1:]))
            else:
                basic_controller.publish_pump_throttle(int(entry))
    except KeyboardInterrupt:
        pass

    basic_controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()