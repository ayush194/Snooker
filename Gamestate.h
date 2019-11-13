#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <reactphysics3d.h>
#include "App.h"
#include "Pooltable.h"
#include "Ball.h"
#include "Cuestick.h"
#include "glm_includes.h"

#include <iostream>

// #define PLAYER1 0
// #define PLAYER2 0
#define HIT_FORCE_INCREMENT 3.0 //TODO: increase this if not enough increment in force
#define CUE_STICK_MOVE_INCREMENT 0.1

typedef enum State {
    SIMULATION1, //simulation going after player1 turn, do nothing just update frame
    SIMULATION2, //simulation going after player2 turn, do nothing just update frame
    FAULT1, //fault by player1, this occurs before player2 turn, who moves the ball wherever he wants
    FAULT2, //fault by player1, this occurs before player2 turn, who moves the ball wherever he wants
    TURN1_MOVE, //player1 turn to move the cue_stick
    TURN2_MOVE, //player2 turn to move the cue_stick
    TURN1_HIT, //player1 turn to hit the ball. Apply force in this state
    TURN2_HIT //player2 turn to hit the ball. Apply force in this state
} State;

class GameState {
public:
    rp3d::DynamicsWorld* world;
    
    static float balls_pos_i[][3];
    static float walls_pos[][3];
    static float walls_shape[][3];

    //constant physics time step
    const float time_step = 1.0/60.0;

    //16 dynamic rigid bodies
    rp3d::RigidBody* rigidbodies[16];
	rp3d::ProxyShape* proxyshapes[16];
	rp3d::SphereShape* sphere_shape;

    rp3d::RigidBody* rigidwalls[6];
	rp3d::BoxShape* box_shapes[6];
	rp3d::ProxyShape* box_proxyshapes[6];

    rp3d::RigidBody* surface;
    rp3d::BoxShape* box_shape;
    rp3d::ProxyShape* surface_proxyshape;
    
    rp3d::RigidBody* frictionsurface;
    rp3d::BoxShape* box_shape2;
    rp3d::ProxyShape* frictionsurface_proxyshape;

    rp3d::Transform prev_transforms[16];

    	
	PoolTable* pooltable;
	CueStick* cuestick;
	Ball* balls[16];

    bool simulation_complete;
    float accumulator;

    GameState();
    ~GameState();
    void simulate();
    void setCueStick();
    void updateState();
};

#endif