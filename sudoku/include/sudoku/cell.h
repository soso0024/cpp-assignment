#ifndef SUDOKU_CELL_H
#define SUDOKU_CELL_H

#include <algorithm>
#include <vector>
#include <numeric>
#include <array>
#include <iostream>

class Grid;

class Cell
{
  // how many times a given number was pruned, size 10 to forget about index 0
  std::array<uint, 10> pruned{};

public:

  // current guess
  uint digit = 0;
  // candidates
  std::vector<uint> candidates;

  // Cells that cannot have the same digit as this one
  std::array<Cell*, 8+8+8-4> neighboors;

  Cell() {}

  inline Cell(const Cell &src) = delete;

  void init(uint row, uint col, std::array<Cell, 81> &grid, uint digit = 0);

  /// Returns the symbol this cell should be printed as
  inline char symbol() const
  {
    return digit ? digit+48 : ' ';
  }

  /// TODO Returns whether a given guess is feasible for this cell
  bool couldBe(uint guess);

  /// TODO Set the guess for this cell and prunes it from its neighboors
  void setGuess(uint guess);

  /// TODO Cancels the last guess that was done on this cell
  /// Restore the guess on its neighboors if it was pruned
  void cancelGuess();

  /// Restore the guess as a candidate for this cell if it was part of the candidates
  /// unless it was pruned several times
  void restore(uint guess);

  /// TODO returns True if the given guess can be removed from the candidates of this cell
  /// also True if the cell is already set and is different from the guess
  /// False otherwise
  bool canPrune(uint guess) const;

  /// Remove this guess from the candidates and increments how many times this guess was pruned
  void prune(uint guess);

  inline bool operator==(uint guess) const
  {
    return digit == guess;
  }

  // static functions to use in algorithms

  /// returns True if this cell has an assigned digit
  /// (guess or given from the start)
  inline static bool isAssigned(const Cell &cell)
  {
    return cell.digit != 0;
  }

  /// returns True if this cell is valid: has a digit different from its neighboors
  inline static bool isValid(const Cell &cell)
  {
    if(cell.digit == 0)
      return false;
    return std::all_of(cell.neighboors.begin(), cell.neighboors.end(),
                       [&cell](const auto &nb){return cell.digit != nb->digit;});
  }
};

#endif
