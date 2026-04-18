import rclpy
from rclpy.node import Node
from rclpy.qos import QoSPresetProfiles
from std_msgs.msg import Int32
from px4_msgs.msg import ManualControlSetpoint

from basic_controller import PayloadBasicController

class PayloadManualController(PayloadBasicController):
    def __init__(self, servo_start=1465):
        super().__init__('manual_controller')
        self.rc_sub = self.create_subscription(
            msg_type=ManualControlSetpoint,
            topic='/fmu/out/manual_control_setpoint',
            callback=self.manual_setpoint_cb,
            qos_profile=QoSPresetProfiles.SENSOR_DATA.value,
        )
        self.servo_us = servo_start  # int
        self.pump_throttle = 0  # int
        self.get_logger().info('Manual payload control node started')

    def manual_setpoint_cb(self, msg: ManualControlSetpoint):
        sp = msg.data
        self.servo_us = int(sp.aux1)
        self.servo_publisher.publish(self.servo_us)
        self.pump_throttle = int(sp.aux2)
        self.pump_publisher.publish(self.pump_throttle)

def main():
    rclpy.init()
    manual_controller = PayloadManualController()

    try:
        while rclpy.ok():
            throttle = int(input("Enter pump throttle: "))
            manual_controller.publish_pump_throttle(throttle)
    except KeyboardInterrupt:
        pass

    manual_controller.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()