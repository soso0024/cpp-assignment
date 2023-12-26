#include <iostream>
#include <cstdlib>
#include <ctime>
#include "maze.h"

using namespace ecn;

typedef struct
{
    int x, y;     // Node position - little waste of memory, but it allows faster generation
    void *parent; // Pointer to parent node
    char c;       // Character to be displayed
    char dirs;    // Directions that still haven't been explored
} Node;

Node *nodes;       // Nodes array
int width, height; // Maze dimensions
float P;           // Percentage of walls to remove for a non perfect maze

int init()
{
    nodes = new Node[width * height]{};
    if (!nodes)
    {
        std::cerr << "Out of memory!" << std::endl;
        return 1;
    }

    // Setup crucial nodes
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            Node &n = nodes[i + j * width];
            if (i * j % 2)
            {
                n.x = i;
                n.y = j;
                n.dirs = 15; // All directions can be explored
                n.c = ' ';
            }
            else
            {
                n.c = '#'; // Add walls between nodes
            }
            // Randomly remove some walls if not the border or start/end points
            if (static_cast<float>(rand()) / RAND_MAX < P && !(i == 1 && j == 1) &&
                i != 0 && j != 0 && j != height - 1 && i != width - 1 &&
                !(j == height - 2 && i == width - 2))
            {
                n.c = ' ';
            }
        }
    }
    return 0;
}

Node *link(Node *n)
{
    // Connects node to random neighbor (if possible) and returns
    // address of next node that should be visited

    int x, y;
    char dir;
    Node *dest;

    // Nothing can be done if null pointer is given - return
    if (n == NULL)
        return NULL;

    // While there are directions still unexplored
    while (n->dirs)
    {
        // Randomly pick one direction
        dir = (1 << (rand() % 4));

        // If it has already been explored - try again
        if (~n->dirs & dir)
            continue;

        // Mark direction as explored
        n->dirs &= ~dir;

        // Depending on chosen direction
        switch (dir)
        {
        // Check if it's possible to go right
        case 1:
            if (n->x + 2 < width)
            {
                x = n->x + 2;
                y = n->y;
            }
            else
                continue;
            break;

        // Check if it's possible to go down
        case 2:
            if (n->y + 2 < height)
            {
                x = n->x;
                y = n->y + 2;
            }
            else
                continue;
            break;

        // Check if it's possible to go left
        case 4:
            if (n->x - 2 >= 0)
            {
                x = n->x - 2;
                y = n->y;
            }
            else
                continue;
            break;

        // Check if it's possible to go up
        case 8:
            if (n->y - 2 >= 0)
            {
                x = n->x;
                y = n->y - 2;
            }
            else
                continue;
            break;
        }

        // Get destination node into pointer (makes things a tiny bit faster)
        dest = nodes + x + y * width;

        // Make sure that destination node is not a wall
        if (dest->c == ' ')
        {
            // If destination is a linked node already - abort
            if (dest->parent != NULL)
                continue;

            // Otherwise, adopt node
            dest->parent = n;

            // Remove wall between nodes
            nodes[n->x + (x - n->x) / 2 + (n->y + (y - n->y) / 2) * width].c = ' ';

            // Return address of the child node
            return dest;
        }
    }

    // If nothing more can be done here - return parent's address
    return static_cast<Node *>(n->parent);
}

void draw(Maze &maze)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (nodes[j + i * width].c == ' ')
            {
                maze.dig(j, i);
            }
        }
    }
    maze.save(); // Use the correct file name
}

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " width height probability" << std::endl;
        return 1;
    }

    width = std::atoi(argv[1]);
    height = std::atoi(argv[2]);
    P = std::atof(argv[3]);

    // Check for odd dimensions and positive values
    if (width % 2 == 0 || height % 2 == 0 || width <= 0 || height <= 0)
    {
        std::cerr << "Dimensions must be odd and greater than 0!" << std::endl;
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr))); // Seed random generator

    Maze maze(height, width); // Create a new maze object

    if (init())
    {             // Initialize maze
        return 1; // Error handling for memory allocation
    }

    // Setup the start node and begin linking nodes
    Node *start = &nodes[1 + width]; // Setup start node
    start->parent = start;
    Node *last = start;

    // Connect nodes until the start node is reached and can't be left
    while ((last = link(last)) != start)
        ;

    // Draw the maze on the ecn::Maze object
    draw(maze);

    // Free allocated memory (memory kaihou!!!)
    delete[] nodes;

    return 0;
}
