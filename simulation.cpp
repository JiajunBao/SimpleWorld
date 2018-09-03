//
// Created by jajuan on 18-6-29.
//
#include "simulation.h"

static bool initSpecies(world_t &world, const string &speciesFile);
static bool species_initName(world_t &world, const string &speciesFile, string &creatureDir);
static bool species_initProgram(world_t &world, const string &creatureDir);
static bool species_isDuplicate(const world_t &world, const string &speciesName);

static bool doInstruction(grid_t &grid, creature_t &creature);
static bool updataProgramID(creature_t &creature);

static bool faceBorder(const grid_t &grid, const creature_t &creature);
static bool faceForest(const grid_t &grid, const creature_t &creature);
static bool faceLake(const grid_t &grid, const creature_t &creature);

static bool isEnemy(const grid_t &grid, const creature_t &creature);
static bool isSame(const grid_t &grid, const creature_t &creature);
static bool faceWall(const grid_t &grid, const creature_t &creature);
static bool isEmpty(const grid_t &grid, const creature_t &creature);


static bool hop(grid_t &grid, creature_t &creature);
static bool turnLeft(creature_t &creature);
static bool turnRight(creature_t &creature);
static bool infect(grid_t &grid, creature_t &creature);
static bool ifEmpty(const grid_t &grid, creature_t &creature);
static bool ifEnemy(const grid_t &grid, creature_t &creature);
static bool ifSame(const grid_t &grid, creature_t &creature);
static bool ifWall(const grid_t &grid, creature_t &creature);
static bool go(creature_t &creature);

static bool initGrid(world_t &world,const string &worldFile);
//static bool initCreatures(world_t &world, const istringstream &creaturesLine);
static direction_t direcName2DirecCode(const string &name);

bool initWorld(world_t &world, const string &speciesFile,const string &worldFile){
    initSpecies(world,speciesFile);
    initGrid(world,worldFile);
    return true;
}
// MODIFIES: world
//
// EFFECTS: Initialize "world" given the species summary file
//          "speciesFile" and the world description file
//          "worldFile". This initializes all the components of
//          "world". Returns true if initialization is successful.

static bool initSpecies(world_t &world, const string &speciesFile){
    string creatureDir;
    species_initName(world, speciesFile, creatureDir);
    species_initProgram(world,creatureDir);
    return true;
}

static bool species_initName(world_t &world, const string &speciesFile, string &creatureDir){
    ifstream iFile;
    iFile.open(speciesFile);
    if(!iFile){
        cout<<"Error: Cannot open file "<<speciesFile<<"!"<<endl;
        iFile.close();
        exit(0);
    }
    string tmp; //Store the string
    iFile>>creatureDir; //Add the string "Creature" in the first line in and it will be covered in line +4
    unsigned int speciesCount=0;
    iFile.get();
    while(getline(iFile,tmp) && tmp.length()!=0){
            if (!species_isDuplicate(world,tmp)) {
                if (speciesCount>=MAXSPECIES){
                    cout<<"Error: Too many species!"<<endl;
                    cout<<"Maximal number of species is "<<MAXSPECIES<<"."<<endl;
                    iFile.close();
                    exit(0);
                }
                world.species[speciesCount].name = tmp;
                speciesCount++;
            }
    }
    world.numSpecies=speciesCount;
    iFile.close();
    return true;
}
// MODIFIES: species
//
// REQUIRES: The first line gives the directory names for the speciesfiles.
//           The last species should be followed by a vacant line.
// EFFECT: Get species types from the "speciesFile" given.
// return true if initialization is successful
// Record the number of species in world.numSpecies

static bool species_isDuplicate(const world_t &world, const string &speciesName){
    int i=0;
    while (world.species[i].name.length()!=0){
        if (world.species[i].name==speciesName){
            return true;
        }
        i++;
    }
    return false;
}
//EFFECTS: If the speciesName has appear in the species list ,
//         return true,
//         otherwise return false.

static opcode_t opName2Code(const string &name) {
    if (name == "hop") {
        return HOP;
    }
    else if(name == "left"){
        return LEFT;
    }
    else if(name == "right"){
        return RIGHT;
    }
    else if(name == "infect"){
        return INFECT;
    }
    else if(name == "ifempty"){
        return IFEMPTY;
    }
    else if(name == "ifenemy"){
        return IFENEMY;
    }
    else if(name == "ifsame"){
        return IFSAME;
    }
    else if(name == "ifwall"){
        return IFWALL;
    }
    else if(name == "go"){
        return GO;
    }
    else{
        return OP_SIZE;
    }

}
// EFFECT: Return the opCode for a given opName.
//         If the opName is not defined, return OP_SIZE.

static bool species_initProgram(world_t &world,const string &creatureDir){
    string line;//Store a line got from the file by "getline()"
    string record; //Store the first thing in a line
    unsigned int programCount=0;
    for (unsigned int i = 0; i <world.numSpecies; ++i) {
        ifstream iFile(creatureDir+"/"+world.species[i].name);
        if(!iFile){
            cout<<"Error: Cannot open file "<<creatureDir+"/"+world.species[i].name<<"!"<<endl;
            iFile.close();
            exit(0);
        }
        while (getline(iFile,line) && line.length()!=0){
            istringstream tmpString(line); //Bend tmpString to line
            tmpString>>record;
            opcode_t recordCode=opName2Code(record); //the opcode for the variable record
            if(recordCode!=OP_SIZE){
                if (programCount >= MAXPROGRAM){
                    cout<<"Error: Too many instructions for species "<<world.species[i].name<<"!"<<endl;
                    cout<<"Maximal number of instructions is "<<MAXPROGRAM<<"."<<endl;
                    iFile.close();
                    exit(0);
                }
                world.species[i].program[programCount].op=recordCode;
                if(recordCode==IFEMPTY||recordCode==IFENEMY||recordCode==IFSAME||recordCode==IFWALL||recordCode==GO){
                    tmpString>>world.species[i].program[programCount].address;
                }
                programCount++;
            }
            else{
                cout<<"Error: Instruction "<<record<<" is not recognized!"<<endl;
                iFile.close();
                exit(0);
            }
        }
        world.species[i].programSize=programCount;
        programCount=0; //Clear the program Count after the initialization for the species was done.
        iFile.close();  //
    }
    return true;
}
// MODIFIES: species
// REQUIRES: The last species program should be followed by a vacant line.
// EFFECT: Get the programs from the Corresponding files
// Return turn if initializatin succeeds.
// Record the numebr of programs for a species in world.species[i].programSize.
//

static terrain_t terrShortName2terrCode(const char &name){
    if(name == 'P'){
        return PLAIN;
    }
    else if(name == 'L'){
        return LAKE;
    }
    else if(name == 'F'){
        return FOREST;
    }
    else if(name == 'H'){
        return HILL;
    }
    else{
        return TERRAIN_SIZE;
    }
}
//  EFFECT: Return the terrain Code for a given terrain string.
//          Return TERRAIN_SIZE if the terrain string does not have a corresponding code.

static bool initGrid(world_t &world,const string &worldFile){
    ifstream iFile(worldFile);
    if(!iFile){
        cout<<"Error: Cannot open file "<<worldFile<<"!"<<endl;
        iFile.close();
        exit(0);
    }
    //Load the height and width of the terrain
    unsigned int tmpHeight=0, tmpWidth=0;
    iFile>>tmpHeight;
    iFile>>tmpWidth;
    //Test the validity of the height and width of the terrain
    if (!(tmpHeight>=1 && tmpHeight<=MAXHEIGHT)){
        cout<<"Error: The grid height is illegal!"<<endl;
        iFile.close();
        exit(0);
    }
    if (!(tmpWidth>=1 && tmpWidth<=MAXWIDTH)){
        cout<<"Error: The grid width is illegal!"<<endl;
        iFile.close();
        exit(0);
    }
    world.grid.height = tmpHeight;
    world.grid.width = tmpWidth;
        char terrShortName;
    //Load the layout of the terrain
    //REQUIRES: there is no additional comments follows any line of the layout
        for (unsigned int heightID = 0; heightID < world.grid.height; ++heightID) {
            for (unsigned int widthID = 0; widthID < world.grid.width; ++widthID) {
                iFile>>terrShortName;
                if(terrShortName2terrCode(terrShortName)==TERRAIN_SIZE){
                    cout<<"Error: Terrain square ("<<terrShortName
                        <<" "<<heightID<<" "<<widthID<<") is invalid!"<<endl;
                    iFile.close();
                    exit(0);
                }
                world.grid.terrain[heightID][widthID]=terrShortName2terrCode(terrShortName);
            }
        }
    //Load the creatures
        string creatureLine;
        unsigned int creatureID=0;
        string creatureName;
        iFile.get();
        while (getline(iFile,creatureLine) && creatureLine.length()!=0){
            istringstream record(creatureLine); //Bend the record to the line of a creature
            record>>creatureName;
            bool hasSpecies=false;//Store the state whether the creature belongs to a certain species
            for (unsigned int i = 0; i < world.numSpecies; ++i) {//Check whether the creature is found in the species list
                if (creatureName == world.species[i].name){//if so
                    hasSpecies=true;
                    //Check whether there is a space available for the creature
                    if (creatureID>=MAXCREATURES){
                        cout<<"Error: Too many creatures!"<<endl;
                        cout<<"Maximal number of creatures is "<<MAXCREATURES<<".";
                        iFile.close();
                        exit(0);
                    }
                    //Record the species of the creature
                    world.creatures[creatureID].species=&world.species[i];
                    //Record the direction of the creature
                    string directionName; //Store the direction name from the worldFile
                    record>>directionName;
                    //Check whether the directionName is recognized
                    if (direcName2DirecCode(directionName)==DIRECT_SIZE){
                        cout<<"Error: Direction "<<directionName<<" is not recognized!"<<endl;
                        iFile.close();
                        exit(0);
                    }
                    //Record the direction of the creature
                    world.creatures[creatureID].direction=direcName2DirecCode(directionName);
                    //Record the location of the creature
                    record>>world.creatures[creatureID].location.r;
                    record>>world.creatures[creatureID].location.c;
                    //Check whether the creature is inside the boundary of the grid.
                    if (world.creatures[creatureID].location.r>=world.grid.height
                        || world.creatures[creatureID].location.c>=world.grid.width){
                        cout<<"Error: Creature ("
                            <<world.creatures[creatureID].species->name
                            <<" "<<directName[world.creatures[creatureID].direction]
                            <<" "<<world.creatures[creatureID].location.r
                            <<" "<<world.creatures[creatureID].location.c
                            <<") is out of bound!"<<endl;
                        cout<<"The grid size is "<<world.grid.height<<"-by-"<<world.grid.width<<"."<<endl;
                        iFile.close();
                        exit(0);
                    }
                    //Record the ability of the creature
                    string abilityCode;
                    while (record>>abilityCode){
                        if (abilityCode == abilityShortName[FLY]){
                            world.creatures[creatureID].ability[FLY]= true;
                        }
                        else if (abilityCode == abilityShortName[ARCH]){
                            world.creatures[creatureID].ability[ARCH]=true;
                        }
                        else{
                            cout<<"Error: Creature ("<<world.creatures[creatureID].species->name
                                <<" "<<directName[world.creatures[creatureID].direction]
                                <<" "<<world.creatures[creatureID].location.r
                                <<" "<<world.creatures[creatureID].location.c
                                <<") has an invalid ability "<<abilityCode<<"!"<<endl;
                            iFile.close();
                            exit(0);
                        }
                    }
                    creatureID++;
                    break;
                }
            }
            if (!hasSpecies){//If the creature does not belong to any species
                cout<<"Error: Species "<<creatureName<<" not found!"<<endl;
                iFile.close();
                exit(0);
            }
        }
    world.numCreatures=creatureID;
    //Initialize the square pointers
    for (unsigned int heightID = 0; heightID < world.grid.height; ++heightID) {
        for (unsigned int widthID = 0; widthID < world.grid.width; ++widthID) {
            world.grid.squares[heightID][widthID]= nullptr;
        }
    }
    //Initialize the program ID and hill active
    world.creatures[creatureID].programID = 0;
    world.creatures[creatureID].hillActive = false;
    //Assign the square to the creature
    for (unsigned int j = 0; j < world.numCreatures; ++j) {// j is the creatureID
        //Check whether the square has been occupied
        if (world.grid.squares[world.creatures[j].location.r][world.creatures[j].location.c]!= nullptr){
            cout<<"Error: Creature (" <<world.creatures[j].species->name
                <<" "<<directName[world.creatures[j].direction]
                <<" "<<world.creatures[j].location.r
                <<" "<<world.creatures[j].location.c
                <<") overlaps with creature ("<<world.grid.squares[world.creatures[j].location.r][world.creatures[j].location.c]->species->name
                <<" "<<directName[world.grid.squares[world.creatures[j].location.r][world.creatures[j].location.c]->direction]
                <<" "<<world.grid.squares[world.creatures[j].location.r][world.creatures[j].location.c]->location.r
                <<" "<<world.grid.squares[world.creatures[j].location.r][world.creatures[j].location.c]->location.c
                <<")!"<<endl;
            iFile.close();
            exit(0);
        }
        //Check whether an animal which can not fly is assigned to a lake.
        if ((!world.creatures[j].ability[FLY])
            && world.grid.terrain[world.creatures[j].location.r][world.creatures[j].location.c] == LAKE ){
            cout<<"Error: Creature ("
                  <<world.creatures[j].species->name
                <<" "<<directName[world.creatures[j].direction]
                <<" "<<world.creatures[j].location.r
                <<" "<<world.creatures[j].location.c
                <<") is in a lake square!"<<endl;
            cout<<"The creature cannot fly!"<<endl;
            iFile.close();
            exit(0);
        }
        world.grid.squares[world.creatures[j].location.r][world.creatures[j].location.c] = &world.creatures[j];
    }
    iFile.close();
    return true;
}
//EFFECTS: Load the terrain and the creatures in the grid.

static direction_t direcName2DirecCode(const string &name){
    if (name == directName[EAST]){
        return EAST;
    }
    else if (name == directName[SOUTH]) {
        return SOUTH;
    }
    else if (name == directName[WEST]){
        return WEST;
    }
    else if (name == directName[NORTH]){
        return NORTH;
    }
    else{
        return DIRECT_SIZE;
    }
}
//EFFECTS: Return the directionCode for a given direction name
//         If the direction name is not recognized, return DRIECTI_SIZE;

static void printInstructionVerbosely(creature_t &creature, grid_t &grid){
    cout<<"Creature ("<<creature.species->name
        <<" " <<directName[creature.direction]
        <<" "<<creature.location.r
        <<" "<<creature.location.c
        <<") takes action:"<<endl;
    bool turnEnd= false;
    while (!turnEnd){
        cout<<"Instruction "<<creature.programID+1<<": "<<opName[creature.species->program[creature.programID].op];
        if (creature.species->program[creature.programID].address == 0){
            cout<<endl;
        }
        else{
            cout<<" "<<creature.species->program[creature.programID].address<<endl;
        }
        turnEnd = doInstruction(grid,creature);
    }
    printGrid(grid);
}
static void printInstructionConcisely(creature_t &creature, grid_t &grid){
    cout<<"Creature ("<<creature.species->name
        <<" " <<directName[creature.direction]
        <<" "<<creature.location.r
        <<" "<<creature.location.c
        <<") takes action: "<<flush;
    bool turnEnd= false;
    while (!turnEnd){
        turnEnd = doInstruction(grid,creature);
    }
    if (creature.programID!=0){
        cout<<opName[creature.species->program[creature.programID-1].op]<<endl;
    }
    else{
        cout<<opName[creature.species->program[creature.species->programSize-1].op]<<endl;
    }
}
void simulateCreature(creature_t &creature, grid_t &grid, bool verbose){
    if (verbose){
        //Simulate the animal in a verbose way
        if (grid.terrain[creature.location.r][creature.location.c]!=HILL){
            printInstructionVerbosely(creature,grid);
            creature.hillActive=false;
        }
        else if (creature.ability[FLY]){
            printInstructionVerbosely(creature,grid);
            creature.hillActive=false;
        }
        else if (creature.hillActive){
            printInstructionVerbosely(creature,grid);
            creature.hillActive=false;
        }
        else{
            creature.hillActive=true;
        }

    }
    else{
        //Simulate the animal in a concise way
        if (grid.terrain[creature.location.r][creature.location.c]!=HILL){//whether the animal is in hill
            printInstructionConcisely(creature,grid);
            creature.hillActive=false;
        }
        else if (creature.ability[FLY]){
            printInstructionConcisely(creature,grid);
            creature.hillActive=false;
        }
        else if (creature.hillActive){
            printInstructionConcisely(creature,grid);
            creature.hillActive=false;
        }
        else{
            creature.hillActive=true;
        }
    }
}
// REQUIRES: creature is inside the grid.
//
// MODIFIES: creature, grid, cout.
//
// EFFECTS: Simulate one turn of "creature" and update the creature,
//          the infected creature, and the grid if necessary.
//          The creature programID is always updated. The function
//          also prints to the stdout the procedure. If verbose is
//          true, it prints more information.

static bool doInstruction(grid_t &grid, creature_t &creature){
    if (creature.species->program[creature.programID].op == HOP){
        hop(grid,creature);
        return true;
    }
    else if (creature.species->program[creature.programID].op == LEFT){
        turnLeft(creature);
        return true;
    }
    else if (creature.species->program[creature.programID].op == RIGHT){
        turnRight(creature);
        return true;
    }
    else if (creature.species->program[creature.programID].op == INFECT){
        infect(grid,creature);
        return true;
    }
    else if (creature.species->program[creature.programID].op == IFEMPTY){
        ifEmpty(grid,creature);
        return false;
    }
    else if (creature.species->program[creature.programID].op == IFENEMY){
        ifEnemy(grid,creature);
        return false;
    }
    else if (creature.species->program[creature.programID].op == IFSAME){
        ifSame(grid,creature);
        return false;
    }
    else if (creature.species->program[creature.programID].op == IFWALL){
        ifWall(grid,creature);
        return false;
    }
    else if (creature.species->program[creature.programID].op == GO){
        go(creature);
        return false;
    }
    else{
        cout<<"Unrecognized opcode!"<<endl;
        return false;
    }
}
// EFFECTS: the creature do an instruction.
// Return true, if it has done hop, left, right, or infect
// Return false, if it has done other programs or the opcode is unrecognized.


void printGrid(const grid_t &grid){
    for (unsigned int heightID = 0; heightID < grid.height; ++heightID) {
        for (unsigned int widthID = 0; widthID < grid.width; ++widthID) {
            if (grid.squares[heightID][widthID] == nullptr){
                cout<<"____ "<<flush;
            }
            else{
                cout<<grid.squares[heightID][widthID]->species->name.substr(0,2)
                    <<"_"<<directShortName[grid.squares[heightID][widthID]->direction]<<" "<<flush;
            }
        }
        cout<<endl;
    }
}
// MODIFIES: cout.
//
// EFFECTS: print a grid representation of the creature world.

static point_t adjacentPoint(point_t pt, direction_t dir){
    point_t adjPoint={0,0};
    if (dir == EAST){
        adjPoint.r=pt.r;
        adjPoint.c=pt.c+1;
    }
    else if (dir == SOUTH){
        adjPoint.r=pt.r+1;
        adjPoint.c=pt.c;
    }
    else if (dir == WEST){
        adjPoint.r=pt.r;
        adjPoint.c=pt.c-1;
    }
    else if (dir == NORTH){
        adjPoint.r=pt.r-1;
        adjPoint.c=pt.c;
    }
    else{
        adjPoint.r=MAXHEIGHT+1;
        adjPoint.c=MAXWIDTH+1;
    }
    return adjPoint;
}
// EFFECTS: Returns a point that results from moving one square
//          in the direction "dir" from the point "pt".

static direction_t leftFrom(direction_t dir){
    if (dir == EAST){
        return NORTH;
    }
    else if (dir == NORTH){
        return WEST;
    }
    else if (dir == WEST){
        return SOUTH;
    }
    else if (dir == SOUTH){
        return EAST;
    }
    else{
        return DIRECT_SIZE;
    }
}
// EFFECTS: Returns the direction that results from turning
//          left from the given direction "dir".

static direction_t rightFrom(direction_t dir){
    if (dir == EAST){
        return SOUTH;
    }
    else if (dir == SOUTH){
        return WEST;
    }
    else if (dir == WEST){
        return NORTH;
    }
    else if (dir == NORTH){
        return EAST;
    }
    else{
        return DIRECT_SIZE;
    }
}
// EFFECTS: Returns the direction that results from turning
//          right from the given direction "dir".

static bool hop(grid_t &grid, creature_t &creature){
    if (faceWall(grid,creature) || !isEmpty(grid,creature)){
        updataProgramID(creature);
        return true;
    }
    else {
        grid.squares[creature.location.r][creature.location.c] = nullptr;
        creature.location = adjacentPoint(creature.location, creature.direction);
        grid.squares[creature.location.r][creature.location.c] = &creature;
        updataProgramID(creature);
        return true;
    }
}

static bool turnLeft(creature_t &creature){
    creature.direction=leftFrom(creature.direction);
    updataProgramID(creature);
    return true;
}

static bool turnRight(creature_t &creature){
    creature.direction=rightFrom(creature.direction);
    updataProgramID(creature);
    return true;
}

static bool infectIt(const creature_t &host, creature_t &slave){
    slave.species=host.species;
    slave.programID=0;
    return true;
}

static bool infect(grid_t &grid, creature_t &creature){
    if (!creature.ability[ARCH]){//If the creature can arch.
        if (isEmpty(grid,creature)||faceBorder(grid,creature)||faceForest(grid,creature)||isSame(grid,creature)){
            updataProgramID(creature);
            return true;
        }
        else if (isEnemy(grid,creature)){
            point_t adjPoint=adjacentPoint(creature.location,creature.direction);
            infectIt(creature,*grid.squares[adjPoint.r][adjPoint.c]);
            assert(infectIt(creature,*grid.squares[adjPoint.r][adjPoint.c]));
            updataProgramID(creature);
            return true;
        }
        else{
            cout<<"Error in the function infect."<<endl;
            updataProgramID(creature);
            return false;
        }
    }
    else{//If the creature can arch.
        point_t adjPoint=creature.location;
        while (adjPoint.r<grid.height && adjPoint.c<grid.width && adjPoint.r>=0 && adjPoint.c>=0){
            if (grid.squares[adjPoint.r][adjPoint.c] != nullptr
                && grid.squares[adjPoint.r][adjPoint.c]->species != creature.species){
                infectIt(creature,*grid.squares[adjPoint.r][adjPoint.c]);
                assert(infectIt(creature,*grid.squares[adjPoint.r][adjPoint.c]));
                updataProgramID(creature);
                return true;
            }
            else {
                adjPoint=adjacentPoint(adjPoint,creature.direction);
            }
        }
        //Nothing to infect.
        updataProgramID(creature);
        return true;
    }
}

static bool ifEmpty(const grid_t &grid, creature_t &creature){
    if (isEmpty(grid,creature) && !faceBorder(grid,creature)){
        creature.programID=creature.species->program[creature.programID].address-1;
    }
    else{
        updataProgramID(creature);
    }
    return true;
}

static bool ifEnemy(const grid_t &grid, creature_t &creature){
    if (isEnemy(grid,creature) && !faceForest(grid,creature)){
        creature.programID=creature.species->program[creature.programID].address-1;
    }
    else{
        updataProgramID(creature);
    }
    return true;
}

static bool ifSame(const grid_t &grid, creature_t &creature){
    if (isSame(grid,creature) && !faceForest(grid,creature)){
        creature.programID=creature.species->program[creature.programID].address-1;
    }
    else{
        updataProgramID(creature);
    }
    return true;
}

static bool ifWall(const grid_t &grid, creature_t &creature){
    if (faceWall(grid,creature)){
        creature.programID=creature.species->program[creature.programID].address-1;
    }
    else{
        updataProgramID(creature);
    }
    return true;
}

static bool go(creature_t &creature){
    creature.programID=creature.species->program[creature.programID].address-1;
    return true;
}


static bool isEmpty(const grid_t &grid, const creature_t &creature){
    point_t adjPoint=adjacentPoint(creature.location,creature.direction);
    return (!faceBorder(grid,creature)) && grid.squares[adjPoint.r][adjPoint.c]== nullptr;
}

static bool isEnemy(const grid_t &grid, const creature_t &creature){
    point_t adjPoint=adjacentPoint(creature.location,creature.direction);
    return (!faceForest(grid,creature)) && (!isEmpty(grid,creature)) && (!faceBorder(grid,creature))
            && grid.squares[adjPoint.r][adjPoint.c]->species!= creature.species;
}

static bool isSame(const grid_t &grid, const creature_t &creature){
    point_t adjPoint=adjacentPoint(creature.location,creature.direction);
    return (!faceForest(grid,creature)) && (!isEmpty(grid,creature)) && (!faceBorder(grid,creature))
            && grid.squares[adjPoint.r][adjPoint.c]->species== creature.species;
}

static bool faceWall(const grid_t &grid, const creature_t &creature){
    return faceBorder(grid,creature) || (faceLake(grid,creature) && (!creature.ability[FLY]));
}

static bool isForest(const grid_t &grid, const point_t& pt){
    return grid.terrain[pt.r][pt.c] == FOREST;
}
//EFFECT: Return true if the terrain of pt is forest.

static bool isLake(const grid_t &grid, const point_t& pt){
    return grid.terrain[pt.r][pt.c] == LAKE;
}
//EFFECT: Return true if the terrain of pt is forest.

static bool faceForest(const grid_t &grid, const creature_t &creature){
    return (!faceBorder(grid,creature)) && isForest(grid, adjacentPoint(creature.location,creature.direction));
}

static bool faceLake(const grid_t &grid, const creature_t &creature){
    return (!faceBorder(grid,creature)) && isLake(grid, adjacentPoint(creature.location,creature.direction));
}

static bool faceBorder(const grid_t &grid, const creature_t &creature){
    point_t adjPoint=adjacentPoint(creature.location,creature.direction);
    return (adjPoint.r<0 || adjPoint.c<0 || adjPoint.r>=grid.height || adjPoint.c>=grid.width);
}

static bool updataProgramID(creature_t &creature){
    if (creature.programID<creature.species->programSize-1){//Check whether it is the last program of the species
        creature.programID++;
        return true;
    }
    else{
        creature.programID=0;//if it is the late program, point back to the first program.
        return false;
    }
}
// EFFECTS: Update the programID of a creature
//          Return true, if the update is to +1;
//          Return false, if the update is to go back to the first program.

