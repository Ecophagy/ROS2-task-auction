# Task Auction in ROS2

This ROS2 package implements a first-price sealed-bid auction process to efficiently allocate tasks amongst an arbitrary number of clients (robots).

The auctioneer can run any number of independent auctions simultaneously, each one ending after a pre-set period of time has elapsed.
Once complete, the auctioneer will publish which client won the auction (the client that submitted the lowest cost bid), so the task can be allocated to the winning client by an (unimplemented) task manager.
Each client subscribes the auction topic, and can submit a bid to every auction based on a local cost calculation.

By using the publish/subscribe model, we can have any number of clients dynamically join and leave the network and bid for tasks without the auctioneer or other clients needing to know particulars.
This allows us to flexibly manage a team or swarm of robots at run time.

## Dependencies

This project is based on and requires on [ROS2 Jazzy](https://docs.ros.org/en/jazzy/Installation.html), and is expected to be built with [colcon](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Colcon-Tutorial.html).
No other dependencies should be required.

It has only been tested on Ubuntu 24.04, although could conceivably work on any OS supported by ROS2, albeit with different usage instructions.

## Usage

- Ensure you `source /opt/ros/jazzy/setup.bash` either directly in console or in your .bashrc file
- Put this package inside the `src` directory of a [ROS2 workspace](https://docs.ros.org/en/jazzy/Tutorials/Beginner-Client-Libraries/Creating-A-Workspace/Creating-A-Workspace.html)
- Build with `colcon build` in the root of the workspace
- Source the built workspace with `source install/setup.bash`
- An example launch file is provided, and can be run with `ros2 launch task_auction task_auction_launch.py`
- Alternatively, you can run the nodes manually with `ros2 run`. The system expects one auctioneer node and any number of auctionClients nodes, one per actual client machine
- Auction off a new task by publishing a `Task` message to the `/new_tasks` topic. This can be done via `rqt` through `Plugins > Topics > Message Publisher`, or directly with `ros2 topic pub /new_tasks task_auction/msg/Task <task msg>`. **N.B.** Only publish this task message **once**, otherwise repeats will override the existing auction.
- Logging messages will show the auction start, bids received, and the winner chosen once the auction completes.
- The winning bid will be published to `/auctions/winner`.

## Future Work

This package acts as a skeleton for a task auction process.
Further work would be necessary to use to actually allocate tasks amongst a team.

### Tasks

A "task" is currently not defined. The current task message has a string which is really a placeholder for a task definition.
A significant part of using this package would be defining what a task is and how they can be generically represented in order for a client to calculate a bid.

### Task Manager

This package does not include a "Task Manager", which would be responsible for creating new tasks and sending them out for auction, as well as assigning tasks to winning clients.
It may also track which tasks are assigned to which clients, and reissue a task for auction if not completed fast enough.
By logging time taken to complete vs client bid, the system can evaluate the accuracy of each client's bid calculation.

### Auction Client

The auction clients currently calculate their bid for any task by generating a uniformly distributed random number.
For real-world use, clients would need to generate an actual cost/bid of task for the client's state, taking into account the client's capabilities and both the task itself and the client's existing queue of tasks.

For example, if the tasks were based on moving to positions, the bid could be in terms of predicted time taken, based on the client's current position, expected velocity, and other tasks already committed to.

### Auctions

The auction process could be developed by adding secondary systems:

- Reserve price: A minimum price which has to be beaten for any client to win the auction. If not met, the task can be re-auctioned at a later date or discarded.
- Buyout price: If a client submits a bid which beats the buyout price, they immediately win the auction without having to wait for regular completion.
- Quorum: Resolve the auction once a certain number of bids have come in, rather than waiting for the full timeout.
