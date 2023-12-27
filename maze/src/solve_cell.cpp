#include <a_star.h>
#include <maze.h>

using namespace std;
using namespace ecn;

// a node is a x-y position, we move from 1 each time
class Position : public Point
{
public:
    // constructor from coordinates
    Position(int _x, int _y) : Point(_x, _y) {}

    // constructor from base ecn::Point
    Position(ecn::Point p) : Point(p.x, p.y) {}

    int distToParent()
    {
        // in cell-based motion, the distance to the parent is always 1
        return 1;
    }

    std::vector<Position> children()
    {
        // this method should return  all positions reachable from this one
        std::vector<Position> generated;

        // TODO add free reachable positions from this point
        // Point(-1, 0) is LEFT, (1, 0) is RIGHT, (0, -1) is UP, (0, 1) is DOWN
        std::vector<Point> directions = {Point(-1, 0), Point(1, 0), Point(0, -1), Point(0, 1)};

        // For each direction
        for(const auto &dir : directions){
            Position next(x + dir.x, y + dir.y);

            // Check if the new location is free in the maze
            if(maze.isFree(next.x, next.y)){
                generated.push_back(next);
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
    Position::maze.saveSolution("cell");
    cv::waitKey(0);

}
