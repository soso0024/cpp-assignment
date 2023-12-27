# Treasure Hunt Game

## Overview
Treasure Hunt is a 1 vs 1 video game developed by Clara Thomas and Luc Vignolles. It's a grid-based game where each player controls a boat with the goal of finding treasure while avoiding rocks. The unique aspect of this game is that players only know their Euclidean distance from the treasure, not its exact location.

## Getting Started
To set up the game, clone the game template into your desired coding folder:

```bash
cp -r /opt/duels/games/treasure_hunt .
cd treasure_hunt
gqt # configure QtCreator
```

The game files are now ready for use in the 'treasure_hunt' folder.

## Game Description
The game operates in an infinite loop where players receive feedback from the game and have 100 ms to compute their next move. Players can choose to compete against various AI levels. The game is compiled using the included `CMakeLists.txt` file.

### Input Rules
Players can control their boats with the following actions:
- TURN LEFT
- TURN RIGHT
- MOVE FORWARD
- USE SONAR (to detect rocks)

### Game Rules
The game is lost if a player hits a rock and won if a player reaches the treasure before their opponent. Draws are possible under certain conditions.

## Expected Work
### AI Class
There's an AI class provided in the game code. Your task is to implement the `computeFrom(feedback)` function, which decides the best action to reach the treasure without hitting any rocks.

### Algorithm Implementation
Implement the following steps in the `TreasureHuntAI` class:
1. Update the map using sonar data.
2. Identify possible treasure positions.
3. Determine the first move based on the A* path.
4. Optional: Find the path to a candidate treasure position.

## Tools Provided
1. **A* Algorithm**: Same as used in the Maze project.
2. **Boat Node**: A class compatible with the A* algorithm.
3. **Orientation**: Functions to assist with boat orientation.
