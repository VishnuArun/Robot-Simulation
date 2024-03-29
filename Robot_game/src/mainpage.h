#ifndef SRC_MAINPAGE_H_
#define SRC_MAINPAGE_H_
/*! \mainpage Robot Simulator Index Page
 *
 * \section intro_sec Introduction
 * Copyright 2018 Vishnu Arun
 *
 * Design Documentation

Overview

"Robot Simulator" is a game that seeks to emulate the behavior of a user contolled robot confined to a fixed amount of space. The robot itself has lives that emulates a constraint where user must keep the number of lives above 0 throguhout the course of the game. The goal of the game is to "capture" or tag 3 fixed bases before the robot loses all of its lives.


Environment

The enviroment is redrawn using NANOGUI at each discrete timestep. The enviroment also includes a small panel with messages and user accessible buttons. Play/Pause and New Game are availble in this panel and Win/Lose is also communicated here by replacing the default "Robot Simulator" text.

The graphics_arena_viewer files contain the graphics portion of the code. This decision was made in case we want to adapt the game to a different GUI rather thatn NANOGUI. The file also includes the pause function where the game will stop updating timesteps when pause flag is set. This pause flag will also suppress input so the user connot pause the game then load the robot to maximum speed for cheating in the game.

In the arena.cc file robot lives are constantly monitored at each timestep. Since lives are an attribute of the robot class we must "get lives" from the robot itself. This was chosen in case we want to eventually add multiple robots where a local lives variable may require more work to maintain. Bases also also constantly monitored. Every time all entities are refreshed we check how many bases have the capture flag set versues all the bases spawned. This was chosen in case we add more bases so we will not need to change any hardcoded values in various files such as this one.

Colors for each entity are set in the params.h file. This decision was chosen to allow easy change of these colors by simply modifying one file to adapt to a new color scheme for example.

The arena size, and intializations for the robot, obstacle, and bases are set in params.h file. This allows a very simple way to change common parameters for the entire game. An example is changing the size of the game to fit different display aspect ratios or increasing the number of obstacles.

Main.cc will start the entire game and initialize all game elements. This abstraction allows easy code interpretablity since each distinct element is kept seperate. Entity_factory.cc will create all new entities and randomize placement and set all inital values.


Entities

Obstacles are treated as mobile entities so we initialize speed as well as other values. Due to expanding the number of variables to this entities we Overload the default reset so all variables are properly reset. The obstacles were also designed to move around other obstacles and walls upon contact. This is accomplished by using an ArcReverse flag that is monitored at each time step to properly reverse when sensors contact a wall, obstacle, or base. When a collision is detected this flag is set and the ArcReverse sequence begins where wheel velocities change to ensure turning behavior.

Robot is treates as a mobile entity. The controller adjusts all movement from graphics file. On collsiion handling we ensure the robot stops moving, but also implement a mercy mode when hitting an obstacle. The mercy mode is a feature for the user where the robot can become invincible upon contact with a obstacle for a finite amount of time. The time is set to roughly 2 seconds as an industry standard. The robot will not loses lives for this time and the colors will change between two preset colors defined in the params.h file. This was chosen incase the user wanted to match a color theme. The lives are depicted in the body of the robot by a number displaying the variable value of lives. Since this variable is automatically updated at each timestep it is always accurate. The reset() is overloaded due to the addition of these new variables which must be reset.

Base is an immobile entity. The colors and intial positions are derived from params.h file in case the user wants to modify these. The reset() is overloaded due to the addition of a captured flag that must be reset. 
 *
 */
#endif  // SRC_MAINPAGE_H_
