#include <sudoku/grid.h>
#include <iostream>

int main(int argc, char** argv)
{
    // get the starting grid and solve it
    // easy is the name of the starting grid (see starts folder)
    // Boolean tells if we should display each solving step
    // initialize sudoku grid and solve sudoku
    Grid(argc, argv, "easy", false).solve();

    return 0;

}
