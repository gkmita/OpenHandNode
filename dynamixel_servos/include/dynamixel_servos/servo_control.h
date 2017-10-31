#ifndef SERVOS_CONTROL_H
#define SERVOS_CONTROL_H

#if defined(__linux__) || defined(__APPLE__)
#include <fcntl.h>
#include <termios.h>
#define STDIN_FILENO 0
#elif defined(_WIN32) || defined(_WIN64)
#include <conio.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include <ros/ros.h>
#include <signal.h>
#include "dynamixel_sdk/dynamixel_sdk.h"                                 // Uses Dynamixel SDK library
#include "dynamixel_servos/InfoMessage.h"
#include "dynamixel_servos/CommandMessage.h"

// Control table address
#define ADDR_PRO_TORQUE_ENABLE          64                 // Control table address is different in Dynamixel model
#define ADDR_PRO_GOAL_POSITION          116 //596
#define ADDR_PRO_PRESENT_POSITION       132 //611
#define ADDR_PRO_CURRENT_VALUE			126 // 2 Bajty

// Protocol version
#define PROTOCOL_VERSION                2.0                 // See which protocol version is used in the Dynamixel

// Default setting
#define BAUDRATE                        3000000
#define DEVICENAME                      "/dev/ttyUSB0"      // Check which port is being used on your controller
                                                            // ex) Windows: "COM1"   Linux: "/dev/ttyUSB0" Mac: "/dev/tty.usbserial-*"
#define FIRST_ID						21
#define SERVOS_NUMBER					3

#define TORQUE_ENABLE                   1                   // Value for enabling the torque
#define TORQUE_DISABLE                  0                   // Value for disabling the torque
#define DXL_MINIMUM_POSITION_VALUE      500             // Dynamixel will rotate between this value
#define DXL_MAXIMUM_POSITION_VALUE      2000             // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     20                  // Dynamixel moving status threshold


#define ESC_ASCII_VALUE                 0x1b



enum Commands
{
	disableTorque = 0,
	enableTorque = 1,
	writeGoalPosition = 2
};


class ServoControl
{
 private:

  dynamixel::PortHandler *portHandler;
  dynamixel::PacketHandler *packetHandler;

 public:
  ServoControl();
  ~ServoControl();

 private:

  int getch(void);
  int kbhit(void);
  
  public:
  
  bool open_port();
  bool set_baudrate();
  bool close_port();
  
  void enable_torque(uint8_t servo_id);
  void disable_torque(uint8_t servo_id);
  void write_goal_position(uint8_t servo_id, uint32_t goal_position);
  void test();
  uint32_t read_present_position(uint8_t servo_id);
  uint16_t read_present_current_value(uint8_t servo_id);
  void command_callback(const dynamixel_servos::CommandMessage::ConstPtr& msg);

};

#endif // SERVOS_CONTROL_H