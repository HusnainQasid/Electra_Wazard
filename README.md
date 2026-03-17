Electrical House Wiring System (C++ / Qt Project)

Overview

This project is an Electrical House Wiring System Simulator developed using C++ (Object-Oriented Programming) and the Qt Framework.
It provides an interactive environment to design and visualize electrical wiring layouts inside a house using efficient algorithms.

The system simulates real-world wiring logic by allowing users to place components and automatically generate optimized wiring paths.

Features

Interactive grid-based layout for house wiring design

Placement of electrical components such as:

Main Board

Sockets

Fans

Bulbs

Click-based component placement with validation rules

Implementation of the A Pathfinding Algorithm* for shortest path calculation

Visual representation of wiring connections inside the grid

Structured using Object-Oriented Programming principles

Technologies Used

C++

Qt Framework (GUI Development)

Data Structures & Algorithms

A* Pathfinding Algorithm

How It Works

The user interacts with a grid-based interface.

Electrical components are placed using mouse clicks.

The system validates placement rules.

The A* algorithm calculates the shortest wiring path.

The wiring is visually displayed on the grid.

Project Structure
fitup/
│── src/              # Source code files
│── include/          # Header files
│── ui/               # Qt UI files
│── build/            # Build files (optional)
│── README.md
│── CMakeLists.txt / .pro file

Installation & Setup
Prerequisites

Qt installed (Qt Creator recommended)

C++ Compiler (GCC / MSVC)

Steps

Clone the repository:

git clone https://github.com/HusnainQasid/fitup.git


Open the project in Qt Creator

Build the project

Run the application

Future Improvements

Advanced device placement rules

Multi-device wiring optimization

Improved UI/UX design

Real-time path visualization enhancements

Performance optimization for larger grids

Project Status

This project is currently under development, and new features are actively being added.

Learning Outcomes

Practical implementation of Object-Oriented Programming

Hands-on experience with Qt GUI development

Application of A pathfinding algorithm*

Understanding of real-world system simulation

Contributing

Contributions, suggestions, and feedback are welcome.
Feel free to fork the repository and submit a pull request.

License

This project is open-source and available under the MIT License.

Author

Husnain Qasid
BS Artificial Intelligence Student
Aspiring Software Engineer with interest in Algorithms, AI, and System Design
