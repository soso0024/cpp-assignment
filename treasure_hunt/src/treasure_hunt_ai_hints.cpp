#include "treasure_hunt_ai.h"
#include <duels/utils/grid_point.h>

#include <duels/algo/a_star.h>

constexpr int ROCK{1};
constexpr int FREE{0};
constexpr int UNKNOWN{2};

TreasureHuntAI::TreasureHuntAI()
    : map(20, 30, UNKNOWN) // fill the 20x30 map with 2's (unknown)
{
    // register this map for all grid points
    pose.setMap(map);

    // the treasure may be at any position
    for(int x = 1; x < map.width()-1; ++x)
    {
        for(int y = 1; y < map.height()-1; ++y)
            candidates.emplace_back(x, y);
    }
}

Action TreasureHuntAI::computeFrom(const Feedback &feedback)
{
    // register where we are
    pose.set(feedback.pose);
    // if we are here then it is not a treasure position
    removeCandidate(pose);

    // TODO process feedback.scan to update map
    for(const auto &[x, y, rock]: feedback.scan) {
        map.cell(x, y) = rock ? ROCK : FREE;
    }

    // Function to process feedback.treasure_distance to update candidates
    // as we compare floating points, a small threshold is useful to tell if two values are equal
    const auto threshold{0.5};
    // prune means like remove
    pruneCandidates([&](const Vector2D<int> &candidate)
    {
        // TODO return true if this candidate cannot be the treasure position
        double distance_to_candidate = sqrt(pow(candidate.x - pose.x, 2) + pow(candidate.y - pose.y, 2));
        return abs(distance_to_candidate - feedback.treasure_distance) > threshold;
    });

    // use A* to find shortest path to first candidate, will assume unknown cells are free
    const auto path{Astar(pose, candidates[0])};
    const auto next{path[1]};

    Action action; // compute it from pose and next

    // TODO identify which move from pose to next
    if(map.cell(path[1]) == UNKNOWN){
        action = Action::SONAR;
    }else if(pose != path[1]){
        action = Action::MOVE;
    }else{
        // Boat facing right or left or up or down
        if((pose.orientation == Orientation::RIGHT && path[1].orientation == Orientation::UP) ||
           (pose.orientation == Orientation::LEFT && path[1].orientation == Orientation::DOWN) ||
           (pose.orientation == Orientation::UP && path[1].orientation == Orientation::LEFT) ||
           (pose.orientation == Orientation::DOWN && path[1].orientation == Orientation::RIGHT)){
            action = Action::TURN_LEFT;
        }
        else{
            action = Action::TURN_RIGHT;
        }
    }
    return action;
}
