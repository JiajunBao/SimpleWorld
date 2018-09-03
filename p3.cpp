//
// Created by jajuan on 18-6-29.
//
#include "simulation.h"

int main(int argc,char*argv[]){
    if (argc < 4){
        cout<<"Error: Missing arguments!"<<endl
            <<"Usage: ./p3 <species-summary> <world-file> <rounds> [v|verbose]"<<endl;
        exit(0);
    }
    world_t world;
    initWorld(world,argv[1],argv[2]);
    bool verbose;
    if (argc <= 4){
        verbose= false;
    }
    else {
        verbose = (string(argv[4]) == "v" || string(argv[4]) == "verbose");
    }
    int roundNum = atoi(argv[3]);
    if (roundNum < 0){
        cout<<"Error: Number of simulation rounds is negative!"<<endl;
        exit(0);
    }
    cout<<"Initial state"<<endl;
    printGrid(world.grid);
    for (int roundID = 0; roundID < roundNum; ++roundID) {
        cout<<"Round "<<roundID+1<<endl;
        if (verbose){
            for (unsigned int creatureID = 0; creatureID < world.numCreatures; ++creatureID) {
                simulateCreature(world.creatures[creatureID],world.grid, verbose);
            }
        }
        else{
            for (unsigned int creatureID = 0; creatureID < world.numCreatures; ++creatureID) {
                simulateCreature(world.creatures[creatureID],world.grid, verbose);
            }
            printGrid(world.grid);
        }
    }
    return 0;
}