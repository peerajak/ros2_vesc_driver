from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'wheel_base',
            default_value='0.3',
            description='Wheel base of the boat (meters)'
        ),
        DeclareLaunchArgument(
            'max_rpm',
            default_value='1000.0',
            description='Maximum RPM for motors'
        ),
        DeclareLaunchArgument(
            'rpm_per_mps',
            default_value='30000.0',
            description='Scaling factor from m/s to RPM'
        ),

        Node(
            package='vesc_diff_drive',
            executable='vesc_diff_drive',
            name='vesc_diff_drive',
            output='screen',
            parameters=[{
                'wheel_base': LaunchConfiguration('wheel_base'),
                'max_rpm': LaunchConfiguration('max_rpm'),
                'rpm_per_mps': LaunchConfiguration('rpm_per_mps'),
            }]
        )
    ])