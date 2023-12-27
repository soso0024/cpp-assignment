#include <a_star.h>
#include <maze.h>
#include <cmath>
#include <vector>
using namespace std;
using namespace ecn;

class Position : public Point
{
public:
    int distance; // Distance from the parent node

    // Constructors
    Position(int _x, int _y) : Point(_x, _y), distance(0) {}
    Position(ecn::Point p) : Point(p.x, p.y), distance(0) {}
    Position(int _x, int _y, int _distance) : Point(_x, _y), distance(_distance) {}

    // New method to return the distance from the parent node
    int distToParent()
    {
        return distance;
    }

    // Determine if the cell is a corridor
    bool is_corridor(int x, int y) {
        int freeCount = 0;
        for(size_t i = 0; i < dx.size(); ++i) {
            if(maze.isFree(x + dx[i], y + dy[i])) {
                freeCount++;
            }
        }
        // A corridor has exactly one or two open sides (straight corridor or dead-end)
        return freeCount == 1 || freeCount == 2;
    }

    bool is_corridor(int x, int y, int &dx, int &dy) {
        // Initially assume the cell is not part of a corridor
        bool isCorridor = false;

        // Check for horizontal corridor
        if (dx != 0 && maze.isFree(x + dx, y)) {
            // Count free neighbors
            int freeCount = (int)maze.isFree(x, y + 1) + (int)maze.isFree(x, y - 1);
            if (freeCount == 1) {
                // It's a horizontal corridor or corner; update dy if we're at a corner
                if (maze.isFree(x, y + 1)) {
                    dy = 1;
                    dx = 0; // Reset dx because it's a corner changing to vertical
                } else if (maze.isFree(x, y - 1)) {
                    dy = -1;
                    dx = 0; // Reset dx because it's a corner changing to vertical
                }
                isCorridor = true; // We are still in a corridor or just hit a corner
            }
        }
        // Check for vertical corridor
        else if (dy != 0 && maze.isFree(x, y + dy)) {
            // Count free neighbors
            int freeCount = (int)maze.isFree(x + 1, y) + (int)maze.isFree(x - 1, y);
            if (freeCount == 1) {
                // It's a vertical corridor or corner; update dx if we're at a corner
                if (maze.isFree(x + 1, y)) {
                    dx = 1;
                    dy = 0; // Reset dy because it's a corner changing to horizontal
                } else if (maze.isFree(x - 1, y)) {
                    dx = -1;
                    dy = 0; // Reset dy because it's a corner changing to horizontal
                }
                isCorridor = true; // We are still in a corridor or just hit a corner
            }
        }

        return isCorridor;
    }

    // Generate all positions reachable from this one
    std::vector<Position> children() {
        std::vector<Position> generated;

        // For each direction
        for(int i = 0; i < 4; ++i) {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            int new_distance = 0;

            // Proceed in a straight line down the corridor
            while(maze.isFree(new_x, new_y)) {
                new_distance++;
                new_x += dx[i];
                new_y += dy[i];

                // Stop if it's not a corridor anymore
                if(!is_corridor(new_x, new_y)) {
                    break;
                }
            }

            // Check if we have moved from the original position
            if(new_distance > 0) {
                // Subtract the last step since we stop at corners and intersections
                new_x -= dx[i];
                new_y -= dy[i];
                new_distance--;

                // Create a new Position with the updated coordinates and distance
                generated.push_back(Position(new_x, new_y, new_distance));
            }
        }
        return generated;
    }

    // Visualization method for A* search
    void show(bool closed, const Point &parent) override
    {
        // Determine the color based on whether the node is in the closed or open list
        const int b = closed ? 255 : 0, r = closed ? 0 : 255;

        // Start from the current position
        int _x = x;
        int _y = y;

        // Trace back to the parent, storing all positions along the way
        std::vector<Point> trace;
        for (int i = 0; i < 4; i++) {
            _x = x + dx[i];
            _y = y + dy[i];
            if (maze.isFree(_x, _y)) {
                trace.push_back(Point(_x, _y));
                while (is_corridor(_x, _y, dx[i], dy[i]) && (_x != parent.x || _y != parent.y)) {
                    _x += dx[i];
                    _y += dy[i];
                    trace.push_back(Point(_x, _y));
                }
                if (_x == parent.x && _y == parent.y) {
                    for (const auto &p : trace) {
                        maze.write(p.x, p.y, r, 0, b, false);
                    }
                    break;
                }
            }
        }
        maze.write(x, y, r, 0, b);
    }

    // Method to print the final path from start to goal
    void print(const Point &parent)
    {
        // Start from the current position
        int _x = x;
        int _y = y;

        // Trace back to the parent, recording all positions along the way
        std::vector<Point> trace;
        for (int i = 0; i < 4; i++) {
            _x = x + dx[i];
            _y = y + dy[i];
            if (maze.isFree(_x, _y)) {
                trace.push_back(Point(_x, _y));
                while (is_corridor(_x, _y, dx[i], dy[i]) && (_x != parent.x || _y != parent.y)) {
                    _x += dx[i];
                    _y += dy[i];
                    trace.push_back(Point(_x, _y));
                }
                if (_x == parent.x && _y == parent.y) {
                    for (auto p = trace.rbegin(); p != trace.rend(); ++p) {
                        maze.passThrough(p->x, p->y);
                    }
                    break;
                }
            }
        }
        maze.passThrough(x, y);
    }

private:
    std::vector<int> dx{-1, 0, 1, 0}; // N, S, E, W directions
    std::vector<int> dy{0, 1, 0, -1};
};

int main(int argc, char **argv)
{
    // Load the maze file
    std::string filename = Maze::mazeFile("maze.png");
    if(argc == 2) {
        filename = argv[1];
    }

    // Initialize the maze
    Position::maze.load(filename);

    // Set the start and goal positions
    Position start = Position::maze.start();
    Position goal = Position::maze.end();

    // Call A* algorithm
    ecn::Astar(start, goal);

    // Save the final image with the solution
    Position::maze.saveSolution("corridor");
    cv::waitKey(0);
}
