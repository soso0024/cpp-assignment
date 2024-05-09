#include <sudoku/cell.h>
#include <sudoku/grid.h>
#include <iostream>

void Cell::init(uint row, uint col, std::array<Cell, 81> &grid, uint digit)
{
    if(digit)
    {
        this->digit = digit;
        // no candidates we know this one
    }
    else
    {
        candidates.resize(9);
        std::iota(candidates.begin(), candidates.end(), 1);
    }

    const auto cellPtr = [&grid](uint row, uint col) -> Cell*
    {return grid.data() + 9*row + col;};

    // register neighboors
    const uint quad_row = 3*(row/3);
    const uint quad_col = 3*(col/3);
    auto nb{neighboors.begin()};
    for(uint ax = 0; ax < 9; ++ax)
    {
        // same colum, different row
        if(ax != row)
            *(nb++) = cellPtr(ax,col);
        // same row, different column
        if(ax != col)
            *(nb++) = cellPtr(row, ax);
        // same 3x3 sub-square
        const uint r = quad_row + ax / 3;
        const uint c = quad_col + ax % 3;
        if(r != row && c != col)
            *(nb++) = cellPtr(r, c);
    }
}

void Cell::prune(uint guess)
{  
    if(auto elem = std::find(candidates.begin(), candidates.end(), guess);
            elem != candidates.end())
    {
        if(pruned[guess] != 0)
            throw std::runtime_error("Guess is both in candidates and pruned list");
        candidates.erase(elem);
        pruned[guess]++;
        return;
    }

    // the guess was pruned before, it is fine to prune it again
    if(pruned[guess])
        pruned[guess]++;
}



void Cell::restore(uint guess)
{
    // the guess was actually not pruned
    if(!pruned[guess])
        return;

    // if is not pruned anymore, add it to the candidates again
    if(!--pruned[guess])
        candidates.push_back(guess);
}

// TODO section

bool Cell::couldBe(uint guess)
{
    // code 1
    // check digit is 0
    if (digit != 0) {
        // if value is already setting then return false
        return false;
    }

    // check candidates in for loop
    for (int candidate : candidates) {
        // check guess in the candidate
        if (candidate == guess) {
            // if find then return true
            return true;
        }
    }

    // if not find then return false
    return false;

    // code 2
    // if value is already setting or there is the guess in candidate
    // return digit == 0 && std::find(candidates.begin(), candidates.end(), guess) != candidates.end();
}

bool Cell::canPrune(uint guess) const
{
    // if digit is not 0 (something already set)
    if(digit != 0){
        // if digit is not same as guess then true (can delete)
        // if same then retun false (can not delete)
        return digit != guess;
    }
    // check candidate size > 1
    // and candidates include guess
    bool isGuessInCandidates = std::find(candidates.begin(), candidates.end(), guess) != candidates.end();
    return candidates.size() > 1 && isGuessInCandidates;
}

void Cell::setGuess(uint guess)
{
    digit = guess;

    // remove guess from candidate of next cell
    for (Cell* neighbour : neighboors) {
        neighbour->prune(guess);
    }
}

void Cell::cancelGuess()
{
    uint prevGuess = digit; // save remove value
    digit = 0; // get value from cell

    for (Cell* neighbour : neighboors) {
        neighbour->restore(prevGuess);
    }
}
