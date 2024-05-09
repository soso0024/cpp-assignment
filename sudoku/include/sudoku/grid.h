#ifndef SUDOKU_GRID_H
#define SUDOKU_GRID_H

#include <sudoku/starting_grids.h>
#include <sudoku/cell.h>
#include <array>

class Grid
{
public:
  explicit Grid(int argc, char** argv, std::string start = "easy", bool display = false);

  void solve();

  void print() const;

private:

  bool solveNextCell();

  bool display = false;
  std::array<Cell, 81> cells;
  uint guesses{};
  uint cancels{};
};

#endif
