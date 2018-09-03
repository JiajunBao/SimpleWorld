#include <iostream>
#include "simulation.h"
#include <fstream>
using namespace std;


int test(int argc,char*argv[]) {
    world_t world1;
    initWorld(world1, argv[1], argv[2]);
//    for (int roundID = 0; roundID < atoi(argv[3]); ++roundID) {
//
//    }
    printGrid(world1.grid);











//    //adjacent point test
//    point_t pt=adjacentPoint({1,1},NORTH);
//    cout<<pt.r<<" "<<pt.c<<endl;

//    //Terrain Read-in
//    cout<<"*****Terrain Test*******"<<endl;
//    cout<<"TerrHeight: "<<world1.grid.height<<endl;
//    cout<<"TerrWidth: "<<world1.grid.width<<endl;
//    cout<<"TerrainLayout: "<<endl;
//    for (int k = 0; k < world1.grid.height; ++k) {
//        for (int i = 0; i < world1.grid.width; ++i) {
//            cout<<terrainShortName[world1.grid.terrain[k][i]];
//        }
//        cout<<endl;
//    }
//
//    //creature read-in
//    cout<<"****Creature Read-in******"<<endl;
//    cout<<"Number of creatures: "<<world1.numCreatures<<endl;
//    cout<<"**********************"<<endl;
//    for (int i = 0; i < world1.numCreatures; ++i) {
//        cout<<"CreatureID: "<<i<<endl;
//        cout<<"Creature Name: "<<world1.creatures[i].species->name<<endl;
//        cout<<"ProgramSize: "<<world1.creatures[i].species->programSize<<endl;
//        for (int j = 0; j < world1.creatures[i].species->programSize; ++j) {
//            cout<<world1.creatures[i].species->program[j].op<<" "<<world1.creatures[i].species->program[j].address<<endl;
//        }
//        cout<<"**********************"<<endl;
//    }
//
//        cout<<"****Direction Read-in******"<<endl;
//    for (int i = 0; i < world1.numCreatures; ++i) {
//        cout<<"CreatureID: "<<i<<endl;
//        cout<<"Creature Name: "<<world1.creatures[i].species->name<<endl;
//        cout<<"Direction: "<<world1.creatures[i].direction<<endl;
//        cout<<"Location: ("<<world1.creatures[i].location.r<<" "<<world1.creatures[i].location.c<<")"<<endl;
//        cout<<"Abilities: "<<endl;
//        cout<<world1.creatures[i].ability[FLY]<<endl;
//        cout<<world1.creatures[i].ability[ARCH]<<endl;
//        cout<<"**********************"<<endl;
//    }
//
//    //square Assign Test
//    for (int i = 0; i < world1.grid.height; ++i) {
//        for (int j = 0; j < world1.grid.width; ++j) {
//            cout<<world1.grid.squares[i][j]<<endl;
//        }
//    }
//    for (int k = 0; k < world1.numCreatures; ++k) {
//        cout<<&world1.creatures[k]<<endl;
//    }
//
//    //program test
//            cout<<"Number of Species: "<<world1.numSpecies<<endl;
//    for(int i=0; i < world1.numSpecies;i++) {
//        cout << world1.species[i].name << ": " <<"ProgramSize: "<<world1.species[i].programSize<<endl;
//        for (int j = 0; j < world1.species[i].programSize; j++) {
//            cout << world1.species[i].program[j].op << " " << world1.species[i].program[j].address << endl;
//        }
//    }
//    cout << "--InitProgramTestComplete--"<<endl;
    return 0;
}