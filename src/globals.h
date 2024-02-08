#pragma once

//#define NO_SOUNDS

#define PADDLE_RADIUS 2.8
#define PADDLE_HEIGHT 1.15
#define PADDLE_MASS 5

#define PUCK_RADIUS 2.0
#define PUCK_HEIGHT 1.15
#define PUCK_MASS 4

#define TABLE_DIM_Z 80
#define TABLE_DIM_X 50

#define TABLE_DIM_Z_HALF 40
#define TABLE_DIM_X_HALF 25

#define GOAL_WIDTH 13.6

const double g_d180DivPi = 180.0 / 3.14159265;
const double g_dPiDiv180 = 3.14159265 / 180.0;

#define TODEGREES(x) (x * g_d180DivPi)
#define TORADIANS(x) (x * g_dPiDiv180)

#define GRAVITY -98
#define PHYSICS_UPDATE_FPS 300 
#define MOUSE_MOVEMENT_MULT 5000

#define PUCK_MAX_VEL 150
#define PUCK_MAX_VEL_SQR (PUCK_MAX_VEL * PUCK_MAX_VEL)

#define PADDLE_MAX_VEL 100
#define PADDLE_MAX_VEL_SQR (PUCK_MAX_VEL * PUCK_MAX_VEL)

#define SQUARE(x) ((x)*(x))

