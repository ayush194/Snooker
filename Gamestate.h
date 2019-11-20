#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <reactphysics3d.h>
#include "App.h"
#include "Pooltable.h"
#include "Ball.h"
#include "Cuestick.h"
#include "glm_includes.h"

#include <iostream>

class GameState {
public:
    rp3d::DynamicsWorld* world;
    
    static float balls_pos_i[][3];
    static float walls_pos[][3];
    static float walls_shape[][3];
    static float outer_walls_pos[][3];
    static float outer_walls_shape[][3];

    //constant physics time step
    const float time_step = 1.0/60.0;

    //16 dynamic rigid bodies
    rp3d::RigidBody* rigidbodies[16];
	rp3d::ProxyShape* proxyshapes[16];
	rp3d::SphereShape* sphere_shape;

    rp3d::RigidBody* rigidwalls[6];
	rp3d::BoxShape* box_shapes[6];
	rp3d::ProxyShape* box_proxyshapes[6];

    rp3d::RigidBody* rigidwalls_outer[4];
	rp3d::BoxShape* box_shapes_outer[4];
	rp3d::ProxyShape* box_proxyshapes_outer[4];

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

    uint turn;
    bool turn_continued;
    bool fault_occurred;
    int score[2];
    int balls_on_board = 15;

    GameState();
    ~GameState();
    void hit();
    void simulate();
    void setCueStick();
    void updateState();
    void performBallCheck();
    void renderAim();
};

#endif