""" Launch the driver node and automatically step through the 
    startup sequence of the vehicle and selecting a mission."""

import os
import launch
from launch_ros.actions import Node
from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
from launch.actions import ExecuteProcess, IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch.substitutions.path_join_substitution import PathJoinSubstitution

def generate_launch_description():
    package_dir = get_package_share_directory('wrai_rc')

    # Declare the mission argument
    mission_arg = DeclareLaunchArgument(
        'mission',
        default_value='SKIDPAD',
        description='Mission to be executed by the vehicle'
    )

    # Use the mission argument
    mission = LaunchConfiguration('mission')

    driver = Node(
        package="wrai_rc",
        executable="rc_driver",
        output="screen"
    )

    vehicle_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            PathJoinSubstitution([package_dir, 'launch', 'start_vehicle.py']),
        ]),
        launch_arguments={'mission': mission}.items()
    )

    return LaunchDescription([
        mission_arg,
        driver,
        vehicle_launch
    ])
