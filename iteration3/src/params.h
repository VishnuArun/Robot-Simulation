/**
 * @file params.h
 *
 * @copyright 2017 3081 Staff, All rights reserved.
 *
 */

#ifndef SRC_PARAMS_H_
#define SRC_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"

/*******************************************************************************
 * Macros
 ******************************************************************************/
/*
 * @file. Constructors for classes should use reasonable default values as
 * defined here. An exception might be if the value is 0.
 */

// graphics
#define X_DIM 1024
#define Y_DIM 768
#define TEXT_BOX_WIDTH 50
#define GUI_MENU_WIDTH 180
#define GUI_MENU_GAP 10

// sensor graphics
#define SENSOR_GRAPHICS_RADIUS 5.0

// arena
#define N_LIGHTS 4
#define N_ROBOTS 10
#define MAX_LIGHT 5
#define ARENA_X_DIM X_DIM
#define ARENA_Y_DIM Y_DIM
#define HUNGRY_TIME 250

// game status
#define WON 0
#define LOST 1
#define PLAYING 2

// entity
#define DEFAULT_POSE \
  { 200, 200, 0}
#define DEFAULT_COLOR \
  { 255, 255, 255 }
#define DEFAULT_RADIUS 20

// mobile entity
#define STARTING_VELOCITY 0.0

// robot
#define ROBOT_ANGLE_DELTA 1
#define ROBOT_SPEED_DELTA 1
#define ROBOT_COLLISION_DELTA 1
#define ROBOT_RADIUS 30
#define ROBOT_MIN_RADIUS 8
#define ROBOT_MAX_RADIUS 14
#define ROBOT_INIT_POS \
  { 500, 500 , 0}
#define ROBOT_COLOR \
  { 0, 0, 255 }
// Color switching value
#define ROBOT_COLOR_HUNGRY \
  { 255, 0, 0}

#define ROBOT_HEADING 270
#define ROBOT_INIT_SPEED 5
#define ROBOT_MAX_SPEED 10
#define ROBOT_MAX_ANGLE 360

// food
#define FOOD_RADIUS 20
#define FOOD_COLLISION_DELTA 1
#define FOOD_INIT_POS \
  { 400, 400 }
#define FOOD_COLOR \
  { 255, 0, 0 }

// light
#define LIGHT_POSITION \
  { 200, 200 }
#define LIGHT_RADIUS 30.0
#define LIGHT_MIN_RADIUS 10
#define LIGHT_MAX_RADIUS 50
#define LIGHT_COLOR \
  { 255, 255, 255 }
#define LIGHT_SPEED 10.0
#define LIGHT_ARC_TIME 10


#endif  // SRC_PARAMS_H_
