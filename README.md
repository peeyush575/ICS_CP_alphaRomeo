# ICS_CP_alphaRomeo
We have made a `Maze Creator and Solver` using C.  
Usage:  
It will first ask for the dimensions of the maze that you desire to generate (int the format of nxm).  
Then, it will generate the maze of the required size and ask if you want to solve it (you need to press 1 in order for it to solve the maze).  
It will then output the correct co-ordinates for the required solution path.  

Sample Input and Output:   
```
Please provide the dimensions of the maze you desire to create (in nxm format) : 5x5
###########
#     #   #
##### # # #
#   #   # #
# ####### #
#         #
# #########
#     #   #
##### ### #
#         #
###########
-------------------------------------------------------------
To find the solution to the above maze, type 1: 1
Solving the maze assuming the source and destination to be first and last cell respectively:
(1, 1) (2, 1) (3, 1) (3, 2) (4, 2) (4, 1) (5, 1) (5, 2) (5, 3) (4, 3) (3, 3) (2, 3) (1, 3) (1, 4) (2, 4) (3, 4) (3, 5) (4, 5) (5, 5) 
```
