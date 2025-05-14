from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()
    
    pub_node1 = Node(
        package = 'joy2twist',
        executable = 'joy2twist_node',
        name = 'joy2twist_node',
        output = 'screen',
        namespace = 'namespace01',
        remappings = [('cmd_vel', 'cmd_vel2')]
    )
    
    pub_node2 = Node(
        package = 'joy',
        executable = 'joy_node',
        name = 'joy_node',
        output = 'screen',
        namespace = 'namespace02',
        remappings = [('joy', 'joy')]
    )
    
    ld.add_action(pub_node1)
    ld.add_action(pub_node2)
    
    return ld