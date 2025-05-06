""" Launch the driver node and automatically step through the 
    startup sequence of the vehicle and selecting a mission."""

import os
import launch
from launch_ros.actions import Node
from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch.actions import ExecuteProcess, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions.path_join_substitution import PathJoinSubstitution

def generate_launch_description():
    package_dir = get_package_share_directory('wrai_rc')

    use_sim_time = False
    mission = "SKIDPAD"

    vehicle_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([package_dir, 'launch', 'run_driver.py']),
        ]),
        launch_arguments={'mission': mission}.items()
    )

    teleop = Node(
        package="teleop_twist_keyboard",
        executable="teleop_twist_keyboard",
        output="screen",
        prefix='xterm -e',
        parameters=[{'use_sim_time': use_sim_time}]
    )

    translate = Node(
        package="topic_tools",
        executable="transform",
        arguments=["/cmd_vel", 
                   "/ctrl", "fsai_messages/Control", 
                   """fsai_messages.msg.Control(
                        steer_angle=m.angular.z*500.0,
                        axle_speed_f=m.linear.x*10.0,
                        axle_speed_r=m.linear.x*10.0,
                        state=fsai_messages.msg.Control.NORMAL   
                   )""", 
                   "--import", "std_msgs", "fsai_messages", "--wait-for-start"],
        parameters=[{'use_sim_time': use_sim_time}]
    )


    return LaunchDescription([
        vehicle_launch,
        teleop,
        translate
    ])
