#include <a_star.h>
#include <maze.h>

using namespace std;
using namespace ecn;

// a node is a x-y position, we move from 1 each time
class Position : public Point
{
public:
    int distance; // Variable to hold the distance from the parent node

    // add
    std::vector<int> dx{-1, 0, 1, 0}; // dx & dy for N, S, E, W directions
    std::vector<int> dy{0, 1, 0, -1};

    // Point(-1, 0) is LEFT, (1, 0) is RIGHT, (0, -1) is UP, (0, 1) is DOWN
    // std::vector<Point> directions = {Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)};

    // constructor from coordinates
    Position(int _x, int _y) : Point(_x, _y) {}

    // constructor from base ecn::Point
    Position(ecn::Point p) : Point(p.x, p.y) {}

    // New constructor including distance
    Position(int _x, int _y, int _distance) : Point(_x, _y), distance(_distance){}

    int distToParent()
    {
        return distance;
    }

    // add
    bool is_corridor(int x, int y){
        int freeCount = 0;
        for(size_t i = 0; i < dx.size(); ++i){
            if(maze.isFree(x + dx[i], y + dy[i])){
                freeCount++;
            }
        }
        return freeCount == 2;
    }

    std::vector<Position> children()
    {
        std::vector<Position> generated;

        // For each direction
        for(size_t i = 0; i < dx.size(); ++i)
        {
            int new_x = x + dx[i];
            int new_y = y + dy[i];
            int new_distance = 0;

            // Proceed in a straight line down the corridor
            while(maze.isFree(new_x, new_y))
            {
                new_distance++;
                new_x += dx[i];
                new_y += dy[i];

                // Stop if it's not a corridor anymore
                if(!is_corridor(new_x, new_y) || !maze.isFree(new_x + dx[i], new_y + dy[i]))
                {
                    break;
                }
            }

            // Check if we have moved from the original position
            if(new_distance > 0)
            {
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
};

int main( int argc, char **argv )
{
    // load file
    std::string filename = Maze::mazeFile("maze.png");
    if(argc == 2)
        filename = std::string(argv[1]);

    // let Point know about this maze
    Position::maze.load(filename);

    // initial and goal positions as Position's
    Position start = Position::maze.start(),
            goal = Position::maze.end();

    // call A* algorithm
    ecn::Astar(start, goal);

    // save final image
    Position::maze.saveSolution("line");
    cv::waitKey(0);

}
