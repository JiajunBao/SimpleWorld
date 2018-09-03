# SimpleWorld
Simulate a simple world with a number of creatures running around and living togerther.
# Overview
The simple world program for this project simulates a number of creatures running around in a simple world. The world is an m-by-n two-dimensional grid of squares. Each creature lives in one of the squares, faces in one of the major compass directions (north, east, south, or west), and belongs to a particular species, which determines how that creature behaves. Each square also has a specific terrain type. Each creature may also have some special abilities, including flying and archery. 
# Program Arguments
The expected order of arguments is: 

```[species-summary] [world-file] [rounds] [v|verbose] ```

The first three arguments are mandatory. They give the name of the species summary file, the name of the world file, and the number of simulation rounds, respectively. The fourth argument is optional. If the fourth argument is the string “v” or the string “verbose”, your program should print the simulation result verbosely, which will be explained later. Otherwise, if it is omitted or is any other string, your program should print the result concisely, which will also be explained later. If there are more than four arguments, the remaining arguments are ignored.

Suppose that you program is called p3. It may be invoked by typing in a terminal: 

```./p3 species world 10 v ```

Then your program should read the species summary from the file called “species” and the world file from the file called “world”. The number of simulation rounds is 10. Your program should print the simulation information verbosely.

# Testing
A sample test world file is avaliable. The result is also given in the file "world4sbwqp50v"
To run this test case, type the following commands (Note: suppose the name of the compiled program is foo):

``` ./foo species sampleWorld 50 v > world.out ```

Then the output of your program is redirected to a file named world.out.

# Reference
Weikang Qian, Yue Yao, Shuyang Huang, Zhuangzhuang Zhou, "Project-3-Description", UM-SJTU Joint Institute, 2018.
