""" Starting the real vehicle requires multiple steps.
    The RC driver emulates those steps that are required for the 
    real vehicle using ROS services.
    This launch file steps through the startup sequence of the vehicle.

    1. Power on the vehicle
    2. Select the mission
    3. Toggle the grossfunk
    4. Vehicle is ready to drive
"""


from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, ExecuteProcess, GroupAction, LogInfo, OpaqueFunction, RegisterEventHandler
from launch.event_handlers import OnProcessExit
from launch.substitutions import FindExecutable, LaunchConfiguration
from launch_ros.actions import Node

def launch_setup(context, *args, **kwargs):
    """Doing this as a function to be able to access the context object
        This means we can get the values of the Launch args"""

    mission_lookup = {"ACCELERATION":      1,
                    "SKIDPAD":             2,
                    "AUTOCROSS":           3,
                    "TRACK_DRIVE":         4,
                    "STATIC_INSPECTION_A": 5,
                    "STATIC_INSPECTION_B": 6,
                    "AUTONOMOUS_DEMO":     7}

    mission = LaunchConfiguration('mission').perform(context)

    if not mission.isdigit():
        mission  = mission_lookup[mission.upper()]    

    power = ExecuteProcess(
        cmd=[[
            FindExecutable(name='ros2'),
            " service call ",
            "rc_driver/power ",
            "fsai_messages/srv/Power ",
            "'{asms: 1, tsms: 1, lvms: 1}'"
            ]],
        shell=True
        )

    select_mission = ExecuteProcess(
        cmd=[[
            FindExecutable(name='ros2'),
            " service call ",
            "rc_driver/mission ",
            "fsai_messages/srv/Mission ",
            f"'{{mission: {mission}}}'"
            ]],
            shell=True
        )
    
    toggle_grossfunk = ExecuteProcess(
        cmd=[[
            FindExecutable(name='ros2'),
            " service call ",
            "rc_driver/grossfunk ",
            "fsai_messages/srv/Grossfunk ",
            "'{toggle: 1}'",
            ]],
        shell=True
    )

    startup_sequence = GroupAction(
        actions=[
            power,
            RegisterEventHandler(
                event_handler=OnProcessExit(
                    target_action=power,
                    on_exit=[
                        LogInfo(msg='Powered on'),
                        select_mission
                    ]
                ) ),
            RegisterEventHandler(
                event_handler=OnProcessExit(
                    target_action=select_mission,
                    on_exit=[
                        LogInfo(msg='Selected mission'),
                        toggle_grossfunk
                    ]
                ) ) ] )
    
    return [startup_sequence]

def generate_launch_description():
    opfunc = OpaqueFunction(function=launch_setup)

    return LaunchDescription([
            DeclareLaunchArgument(
                'mission',
                default_value='STATIC_INSPECTION_A',
            ),
            opfunc
        ])
