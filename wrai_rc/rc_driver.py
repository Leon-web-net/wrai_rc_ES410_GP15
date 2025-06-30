import serial.serialutil
import rclpy
from rclpy.node import Node
import rclpy.logging
import tf2_ros
import fsai_messages.msg
import fsai_messages.srv

import serial
import enum
import math
import datetime

WHEEL_DIAMETER_INCH = 20
WHEEL_DIAMETER = WHEEL_DIAMETER_INCH * 0.0254
WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * math.pi
WHEEL_TICKS = 20
TICK_DISTANCE = WHEEL_CIRCUMFERENCE / WHEEL_TICKS
WHEEL_BASE = 1.53

class Calibration:
    def __init__(self, mn=None, mx=None):
        self.__mn, self.__mx = mn, mx
    
    def midpoint(self):
        if self.__mn is None or self.__mx is None:
            return None
        return (self.__mn + self.__mx) / 2
    
    def convert(self, ideal):
        return min( self.__mx, max( self.__mn, ideal + self.midpoint() ) )

class RcDriver(Node):
    def __init__(self):
        super().__init__('rc_driver')

        self.pub_ = self.create_publisher(fsai_messages.msg.Status, 'status', 10)
        self.sub_ = self.create_subscription(fsai_messages.msg.Control, 'ctrl', self.__ctrl_callback, 10)

        self.__calibrate = Calibration()
        self.__serial = None
        self.__timer = self.create_timer(0.05, self.__serial_callback)

        self.__mission = fsai_messages.msg.Status.AMI_NOT_SELECTED
        self.__as_state = None
        self.__buffer = b''
        self.__steer = None
        self.__brake = 0
        self.__rpm = 0
        self.__lenc, self.__renc, self.__senc = 0, 0, 0

        # convert steering angle to encoder ticks, conversion is doing using a polynomial
        # generated from the calibration data enc = [0] + ( ang * [1] ) + ( ang * [2]**2 ) + ( ang * [3]**3 )
        self.__steerpoly = (2.252428829, 3001.47924, 465.8237052, 7981.656532)

        # create services
        self.create_service(fsai_messages.srv.Mission, '~/mission', self.__mission_callback)
        self.create_service(fsai_messages.srv.Grossfunk, '~/grossfunk', self.__grossfunk_callback)
        self.create_service(fsai_messages.srv.Power, "~/power", self.__pwr_callback)

        self.__statusTimer = self.create_timer(0.05, self.__status_publish)

        # tf broadcast
        #self.__tfBroadcaster = tf2_ros.TransformBroadcaster(self)
        #self.__tfTimer = self.create_timer(0.05, self.__tf_callback)       

    def __grossfunk_callback(self, request, response):
        """ Callback function for if the emulated grossfunc is used

            If estop is called then the vehicle will enter ebrake mode.
            On the real car this would only happen if the vehicle was on, but
            if the ebrake was pressed and the vehicle was then turned on it would
            go into ebrake mode so the functionality is close enough.

            If the toggle is called and the vehicle is in ready mode, then the vehicle
            will go to drive mode. This is the same as the real car.
        """
        self.get_logger().info(f"Received grossfunk request: {request}")

        # if ebrake is requsted then we go to ebrake no matter what
        if request.estop:
            self.get_logger().info("Switching to ebrake mode")
            self.__as_state = fsai_messages.msg.Status.AS_EMERGENCY_BRAKE

        # if the toggle switch is set and we are in the ready state, then we go to driving
        elif request.toggle and self.__as_state == fsai_messages.msg.Status.AS_READY:
            self.get_logger().info("Switching to driving mode")
            self.__as_state = fsai_messages.msg.Status.AS_DRIVING
            
        response.success = True
        return response

    def __pwr_callback(self, request, response):
        """ Callback function for if the emulated power buttons are used.

            If all the buttons are pressed then the vehicle will turn on.
            On the real car you might have individual systems turned on and off, 
            e.g. LV on for the computer but everything else off.
            But the vehicle will only move if everything is on so this is close enough.
        """
        self.get_logger().debug(f"Received power message: {request}")

        if request.asms and request.tsms and request.lvms:
            self.__as_state = fsai_messages.msg.Status.AS_OFF
            response.pwr = True
        else:
            self.__as_state = None
            self.__mission = fsai_messages.msg.Status.AMI_NOT_SELECTED
            response.pwr = False
        
        return response


    def __mission_callback(self, request, response):
        """ Callback function for if the emulated mission screen is used.
        
            Can only change the mission if the vehicle is in OFF mode.
            Same are real vehicle.
        """
        self.get_logger().info(f"Received mission request: {request}")
        if self.__as_state == None:
            self.get_logger().info(f"Vehicle is off")
        elif self.__as_state == fsai_messages.msg.Status.AS_OFF and request.mission >= 0 and request.mission <= 7:
            self.get_logger().info(f"Switching to ready state with mission {request.mission}")
            self.__mission = request.mission
            self.__as_state = fsai_messages.msg.Status.AS_READY

        response.mission = self.__mission
        return response


    def __status_publish(self):
        """Publish the status of the car as a FsStatus message.
        
            This is an emulation of the real car's behaviour."""
        
        speed, brake, steer = 0, 1, None

        if self.__as_state == None: # vehicle is off
            return

        # === publish status ===
        status = fsai_messages.msg.Status()
        status.header.stamp = self.get_clock().now().to_msg()

        if self.__as_state == fsai_messages.msg.Status.AS_DRIVING:
            #status.steering_angle = float(self.__senc)
            status.steering_angle = float( (self.__senc - self.__steerpoly[0]) / (
                self.__steerpoly[1] + self.__steerpoly[2]**2 + self.__steerpoly[3]**3 ) ) 
            status.f_brake = float(0)
            status.r_brake = float(0)
            status.fl_rpm = float(0)
            status.fr_rpm = float(0)
            status.rl_rpm = float(0)
            status.rr_rpm = float(0)
            status.fl_tick = 0#status.rl_tick = self.__lenc
            status.fr_tick = 0#status.rr_tick = self.__renc

        # states
        status.as_state = self.__as_state
        status.ami_state = self.__mission

        self.pub_.publish( status )
   
    def __ctrl_callback(self, message):
        #self.get_logger().info(f"Received control message: {message}")

        # convert steering angle to encoder ticks, conversion is doing using a polynomial
        # generated from the calibration data 
        #self.__steer = self.__steerpoly[0] + \
        #    message.steer_angle * self.__steerpoly[1] + \
        #    message.steer_angle * self.__steerpoly[2]**2 + \
        #    message.steer_angle * self.__steerpoly[3]**3
        self.__steer = message.steer_angle

        self.__brake = ( message.brake_press_f + message.brake_press_r ) /2

        # fsai_messages.Ctrl supports two drive modes, NORMAL and NORMAL_MS
        # this determines if the speed is in rpm or m/s
        self.__rpm = 0

        # if ebrake is requested, go to ebrake
        # if we're already in ebrake, stay in it
        if self.__as_state == fsai_messages.msg.Status.AS_EMERGENCY_BRAKE:
            pass
        elif message.state == fsai_messages.msg.Control.EBRAKE:
            self.__as_state = fsai_messages.msg.Status.AS_EMERGENCY_BRAKE

        # if we're in finished mode, stay in it
        elif self.__as_state == fsai_messages.msg.Status.AS_FINISHED:
            pass
        # if we're in driving mode and we request finished, go to finished
        elif self.__as_state == fsai_messages.msg.Status.AS_DRIVING and message.state == fsai_messages.msg.Control.FINISHED:
            self.__as_state = fsai_messages.msg.Status.AS_FINISHED
        
        # if we're in driving mode, then drive
        self.__rpm = ( message.axle_speed_f + message.axle_speed_r ) /2
        if self.__as_state == fsai_messages.msg.Status.AS_DRIVING:
            avg_speed = ( message.axle_speed_f + message.axle_speed_r ) /2
            if message.state == fsai_messages.msg.Control.NORMAL_MS:
                self.__rpm = (avg_speed / WHEEL_CIRCUMFERENCE) * 60
            elif message.state == fsai_messages.msg.Control.NORMAL:
                self.__rpm = avg_speed

    def __serial_callback(self):
        # connect to serial if needed
        """
        if self.__serial is None:
            try:
                self.__serial = serial.Serial('/dev/ttyACM1', 9600)
            except serial.serialutil.SerialException as e:
                self.get_logger().error(f"{e}",throttle_duration_sec=3)
                return
        """
        
        try:
            if self.__serial is None:
                device = "/dev/serial/by-id/usb-Arduino__www.arduino.cc__0042_4343935353635181F082-if00"
                self.get_logger().info("Attempting to reconnect",throttle_duration_sec=3)
                self.__serial = serial.Serial(device, 115200)

                # temp bodge to reset the arduino on each connection
                self.__serial.dtr = 1
                import time
                time.sleep( 1 )
            else:
                self.__serial.dtr = 0
        except serial.serialutil.SerialException as e:
            self.get_logger().error(f"{e}",throttle_duration_sec=3)
            return
        #self.get_logger().info( f"waiting {self.__serial.in_waiting}")

        # read serial data
        if self.__serial.in_waiting > 0:
            self.__buffer += self.__serial.read(self.__serial.in_waiting)
            packets = self.__buffer.split(b'\n')
            self.__buffer = packets[-1]

            for packet in packets[:-1]:

                #self.get_logger().info( f"serial recv {packet}")
                try:
                    self.get_logger().info( packet.decode() )
                    mn, mx, self.__lenc, self.__renc, self.__senc = ( int(i) for i in packet.decode().split(" "))
                    self.__calibrate = Calibration( mn, mx )

                except ValueError as e:
                    #self.get_logger().error( e )
                    # invalid format
                    continue

        if self.__calibrate.midpoint() is None:
            return

        # default to doing nothing
        l, r, s = 0, 0, 0

        #self.get_logger().info( self.__steer )

        # currently having issues with encoders, so temporarily disabling and switching to directly passing steering voltages
        if self.__steer is not None:
            s = self.__steer * 1000
        
        #    # based on latest encoder data, calculate the control values for steering
        #    kp = 1.0
#
        #    # self.__steer is an idealised encoder value that assumes that 0 is the midpoint, adjust to reality
        #    target = self.__calibrate.convert( self.__steer )
        #    error = target - self.__senc
        #    s = error * kp

        l = r = self.__rpm

        self.get_logger().info(f"Sending: {l} {r} {s}")

        # steering +CCW, -CW
        self.__serial.write(f'{int(l)} {int(r)} {int(s)}\n'.encode())
        self.get_logger().info(f"Sent: {int(l)} {int(r)} {int(s)}")


def main(args=None):
    rclpy.init(args=args)

    rcd = RcDriver()

    rclpy.spin(rcd)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    rcd.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
