# Real-Time ChatGPT Hybrid Decision-Making for Safe Human-Robot Interaction



## Overview

This project explores the integration of Large Language Models (LLMs), specifically ChatGPT, into robotic decision-making frameworks to enhance real-time human-robot collaboration. By leveraging a hybrid decision-making model, this system balances reactive control with deliberative planning, ensuring adaptive, efficient, and safe robotic behavior.

## Features

✅ **Hybrid Decision-Making** – Combines reactive and deliberative control strategies for improved real-time decision-making.\
✅ **LLM-Based Task Planning** – Uses ChatGPT to process high-level natural language commands and convert them into actionable robot instructions.\
✅ **MoveIt Integration** – Implements a hybrid planner in ROS2 MoveIt for dynamic path planning and obstacle avoidance.\
✅ **Human Detection and Interaction** – Utilizes a 3D depth camera and YOLO for real-time human pose estimation and collision avoidance.\
✅ **Safe Human-Robot Collaboration** – Implements layered safety measures to prevent collisions and enhance operational efficiency.

## System Architecture

The robotic system is structured into several key components:

- **UR5e Robotic Arm** – Primary manipulator executing planned trajectories.
- **ChatGPT Task Planner** – Processes human commands and generates structured task plans.
- **MoveIt Hybrid Planning Framework** – Provides real-time reactive path planning and obstacle avoidance.
- **PrimeSense 3D Depth Camera** – Enables human pose estimation and dynamic collision marker generation.
- **YOLO ROS Wrapper** – Detects humans and objects to generate collision boundaries for safer interactions.



## Results

### Simulation Performance

- MoveIt hybrid planner successfully adapted to dynamic environments, ensuring real-time path corrections.
- Human pose detection provided accurate real-time collision avoidance, though response times needed further optimization.

### Real-World Testing

- The global planner effectively executed pre-defined tasks with high accuracy.
- Real-time hybrid planning was validated in simulations but awaits further testing in physical environments.

## Future Work

🚀 Improve real-time responsiveness with optimized service calls.\
🚀 Enhance human detection accuracy using LiDAR or multi-camera setups.\
🚀 Scale the system for complex environments, such as factory floors or clinical settings.\
🚀 Explore alternative LLMs for improved command interpretation.

## Contributors

👤 **Nicholas Bell** – Lead Developer & Researcher\
📩 Contact: https\://www\.linkedin.com/in/nickojbell/

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## References

📜 For a detailed breakdown, refer to the full [Thesis Document](https://github.com/YOUR_USERNAME/Your_Repo_Name/blob/main/Thesis___Nicholas_Bell.pdf).

