# Veddar VESC Interface
 This repo is originally from origin	https://github.com/f1tenth/vesc.git (fetch)
![ROS2 CI Workflow](https://github.com/f1tenth/vesc/workflows/ROS2%20CI%20Workflow/badge.svg)

Packages to interface with Veddar VESC motor controllers. See https://vesc-project.com/ for details

This is a ROS2 implementation of the ROS1 driver using the new serial driver located in [transport drivers](https://github.com/ros-drivers/transport_drivers).

## How to test

1. Clone this repository and [transport drivers](https://github.com/ros-drivers/transport_drivers) into `src`.
2. `rosdep update && rosdep install --from-paths src -i -y`
3. Plug in the VESC with a USB cable.
4. Modify `vesc/vesc_driver/params/vesc_config.yaml` to reflect any changes.
5. Build the packages `colcon build`
6. `ros2 launch vesc_driver vesc_driver_node.launch.py`
7. If prompted "permission denied" on the serial port: `sudo chmod 777 /dev/ttyACM0`

```
ros2 launch vesc_driver vesc_driver_node.launch.py 
ros2 topic echo /sensors/core
ros2 topic pub /commands/motor_master/duty_cycle std_msgs/msg/Float64 "{data: 0.3}"
ros2 topic pub /commands/motor_slave/speed std_msgs/msg/Float64 "{data: 3000}"
ros2 topic pub /commands/motor_master/current std_msgs/msg/Float64 "{data: 1.2}"
```

```
ros2 topic list
/commands/motor_master/brake
/commands/motor_master/current
/commands/motor_master/duty_cycle
/commands/motor_master/speed
/commands/motor_slave/brake
/commands/motor_slave/current
/commands/motor_slave/duty_cycle
/commands/motor_slave/speed
/commands/servo/position
/parameter_events
/rosout
/sensors/core
/sensors/imu
/sensors/imu/raw
/sensors/servo_position_command
```

TODO

- currently slave can_id is hardcoded to 62, change that
- topic like break,position,current need to have master, slave
