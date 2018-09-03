//
// Created by jajuan on 18-6-29.
//

#ifndef PROJECT3_SIMULATION_H
#define PROJECT3_SIMULATION_H

#include "world_type.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <cassert>

using namespace std;

bool initWorld(world_t &world, const string &speciesFile,const string &worldFile);
// MODIFIES: world
//
// EFFECTS: Initialize "world" given the species summary file
//          "speciesFile" and the world description file
//          "worldFile". This initializes all the components of
//          "world". Returns true if initialization is successful.

void simulateCreature(creature_t &creature, grid_t &grid, bool verbose);
// REQUIRES: creature is inside the grid.
//
// MODIFIES: creature, grid, cout.
//
// EFFECTS: Simulate one turn of "creature" and update the creature,
//          the infected creature, and the grid if necessary.
//          The creature programID is always updated. The function
//          also prints to the stdout the procedure. If verbose is
//          true, it prints more information.

void printGrid(const grid_t &grid);
// MODIFIES: cout.
//
// EFFECTS: print a grid representation of the creature world.

#endif //PROJECT3_SIMULATION_H
