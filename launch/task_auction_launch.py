from launch import LaunchDescription

from launch_ros.actions import Node


def generate_launch_description():
     return LaunchDescription([
          Node(
               package='task_auction',
               executable='auctioneer',
               name='auctioneer'
          ),
          Node(
               package='task_auction',
               executable='auctionClient',
               name='client1',
               parameters=[
                    {'robot_id': 1}
               ]
          ),
          Node(
               package='task_auction',
               executable='auctionClient',
               name='client2',
               parameters=[
                    {'robot_id': 2}
               ]
          ),
     ])