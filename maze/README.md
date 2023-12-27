# Maze Project

<p align="center">
  <img src="https://github.com/soso0024/cpp-assignment/blob/main/maze/mazes/_cell.png" alt="maze image 01" width="450"/>
  <img src="https://github.com/soso0024/cpp-assignment/blob/main/maze/mazes/_corridor.png" alt="maze image 02" width="450"/>
</p>

## Overview

This project involves generating and solving mazes using C++ programming as part of the Advanced Robot Programming Labs in the Option Robotique Control & Robotics master program. Inspired by a Computerphile video, the project combines Git and CMake for development and compilation.

## Lab Content

- **Maze Generation**: Create mazes of given dimensions and modify them to meet specific criteria.
- **Maze Solving**: Implement various maze-solving algorithms using the A* (A-Star) algorithm with different approaches:
  - Cell-based motions
  - Line-based motions
  - Corridor-based motions

## Required Work

1. **Maze Generation**: Generate a maze and modify the provided C-code from Wikipedia to compile as C++ and save the final maze to an image file.
2. **Maze Solving**: Develop programs for solving the maze using the three mentioned A* approaches. This involves modifying and extending the provided code and classes.

## Provided Tools

### Classes

- **ecn::Maze**: Interfaces with an image for maze creation and access, including methods for building, saving, and modifying a maze.
- **ecn::Point**: Implements basic properties of a 2D point, useful for maze-solving algorithms.

### Utilities

- Git repository and CMake for downloading and compiling the project.
- Utility functions like `is_corridor` for identifying maze characteristics.

## Building and Running

The project uses a standard C++ build process:

```bash
mkdir build
cd build
cmake ..
make
```

After building, you can launch the program to see the results.
