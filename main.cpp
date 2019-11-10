//
//  Created by Ayush Kumar on 27/09/19.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#include "App.h"

//model loading library
#include "objloader.hpp"
#include "shader.h"

#include "Pooltable.h"
#include "Cuestick.h"
#include "Ball.h"
#include "glm_includes.h"

#include <reactphysics3d.h>

#include <iostream>
#include <vector>

int main() {
	App();
	PoolTable::loadVertexData();
	PoolTable* pooltable = new PoolTable(0);
	CueStick::loadVertexData();
	CueStick* cuestick = new CueStick(0);
	Ball::loadVertexData();
	Ball* balls[16];

	cuestick->pos = glm::vec3(0.0, 0.0, 1.5);
	//Initial Position of the balls
	//Radius = 0.132
	//location of the 6 holes
	//1 -> (-3.028, -5.473, 1.080)
	//2 -> (-3.107, -0.010, 1.080)
	//3 -> (-3.028, 5.473, 1.080)
	//4 -> (3.054, 5.473, 1.080)
	//5 -> (3.107, 0.010, 1.080)
	//6 -> (3.054, -5.473, 1.080)
	//Radius of hole -> 0.300
	//Right wall at x = 3.054
	//Bottom wall at y = -5.474
	//Top Wall at y = 5.473
	//Left Wall at x = -3.028
	//Surface of the table at z = 0.492
	float balls_pos_i[][3] = {
		{ 0.000, -2.733,  1.080},		//Cue

		{ 0.005,  2.083,  1.080},		//Ball1

		{-0.135,  2.317,  1.080},		//Ball2
		{ 0.151,  2.325,  1.080},		//Ball3

		{-0.273,  2.574,  1.080},		//Ball4
		{-0.013,  2.559,  1.080},		//Ball5
		{ 0.280,  2.564,  1.080},		//Ball6

		{-0.402,  2.809,  1.080},		//Ball7
		{-0.140,  2.821,  1.080},		//Ball8
		{ 0.123,  2.805,  1.080},		//Ball9
		{ 0.406,  2.807,  1.080},		//Ball10

		{-0.532,  3.039,  1.080},		//Ball11
		{-0.273,  3.058,  1.080},		//Ball12
		{ 0.000,  3.049,  1.080},		//Ball13
		{ 0.265,  3.044,  1.080},		//Ball14
		{ 0.540,  3.044,  1.080}		//Ball15
	};

	
	for(int i = 0; i < 16; i++) {
		balls[i] = new Ball(i);
		//balls[i]->pos = glm::vec3(balls_pos_i[i][0], balls_pos_i[i][1], balls_pos_i[i][2]);
	}
	
	//building shaders
	Shader pooltable_diffuse_shader("shaders/pooltable_diffuse.vs", "shaders/pooltable_diffuse.fs");
	Shader ball_diffuse_shader("shaders/ball_diffuse.vs", "shaders/ball_diffuse.fs");
	Shader axes_shader("shaders/axes.vs", "shaders/axes.fs");

	
	//using ReactPhysics3d library
	rp3d::Vector3 gravity(0.0, 0.0, -9.81);
	rp3d::DynamicsWorld world(gravity);
	world.enableSleeping(true);
	//world.setSleepLinearVelocity();		//threshold for when to put the body on sleep
	//world.setSleepAngularVelocity();
	
	// Initial position and orientation of the rigid body
	rp3d::Vector3 init_position(0.0, 0.0, 0.0) ;
	rp3d::Quaternion init_orientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(init_position, init_orientation) ;
	//Create 16 dynamic rigid bodies and atttach the shape sphere with each one of them
	rp3d::RigidBody* rigidbodies[16];
	rp3d::ProxyShape* proxyshapes[16];
	rp3d::Transform prev_transforms[16];
	rp3d::SphereShape sphere_shape(Ball::radius);
	for(int i = 0; i < 16; i++) {
		transform.setPosition(rp3d::Vector3(balls_pos_i[i][0], balls_pos_i[i][1], balls_pos_i[i][2]));
		rigidbodies[i] = world.createRigidBody(transform);
		rigidbodies[i]->getMaterial().setBounciness(0);
		rigidbodies[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.3));
		rigidbodies[i]->getMaterial().setRollingResistance(rp3d::decimal(0.3));
		//rigidbodies[i]->setLinearDamping(0.05);
		//rigidbodies[i]->setAngularDamping(0.05);
		proxyshapes[i] = 
			rigidbodies[i]->addCollisionShape(&sphere_shape, rp3d::Transform::identity(), Ball::mass);
		prev_transforms[i] = rp3d::Transform(init_position, init_orientation);
	}
	
	//Create 6 more static rigid bodies for the 6 pieces of walls
	float walls_pos[][3] = {
		{-3.107, -2.732, 1.026},
		{-3.107, 2.732, 1.026},
		{0.020, 5.562, 1.026},
		{3.148, 2.732, 1.026},
		{3.148, -2.732, 1.026},
		{0.020, -5.572, 1.026}
	};
	float walls_shape[][3] = {
		{0.1, 2.450, 0.126},
		{0.1, 2.450, 0.126},
		{2.728, 0.08, 0.126},
		{0.1, 2.450, 0.126},
		{0.1, 2.450, 0.126},
		{2.728, 0.08, 0.126}
	};
	rp3d::RigidBody* rigidwalls[6];
	rp3d::BoxShape* box_shapes[6];
	rp3d::ProxyShape* box_proxyshapes[6];
	for(int i = 0; i < 6; i++) {
		transform.setPosition(rp3d::Vector3(walls_pos[i][0], walls_pos[i][1], walls_pos[i][2]));
		rigidwalls[i] = world.createRigidBody(transform);
		rigidwalls[i]->setType(rp3d::BodyType::STATIC);
		rigidwalls[i]->getMaterial().setBounciness(0);
		rigidwalls[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.06));
		box_shapes[i] = new rp3d::BoxShape(rp3d::Vector3(walls_shape[i][0], walls_shape[i][1], walls_shape[i][2]));
		box_proxyshapes[i] = rigidwalls[i]->addCollisionShape(box_shapes[i], rp3d::Transform::identity(), 5000.0);
	}

	//And 1 static rigid body for the surface of the pooltable
	transform.setPosition(rp3d::Vector3(0.0, 0.0, 0.952));
	rp3d::RigidBody* surface = world.createRigidBody(transform);
	rp3d::BoxShape box_shape(rp3d::Vector3(6.127 / 2, 11.016 / 2, 0.001 / 2));
	surface->setType(rp3d::BodyType::STATIC);
	surface->getMaterial().setBounciness(0);
	surface->getMaterial().setFrictionCoefficient(0.3);
	rp3d::ProxyShape* surface_proxyshape = surface->addCollisionShape(&box_shape, rp3d::Transform::identity(), 5000.0);
	
	//Add 1 rigid high-friction surface below the pool table surface
	transform.setPosition(rp3d::Vector3(0.0, 0.0, 0.652));
	rp3d::RigidBody* frictionsurface = world.createRigidBody(transform);
	rp3d::BoxShape box_shape2(rp3d::Vector3(6.127, 11.016, 0.001));
	frictionsurface->setType(rp3d::BodyType::STATIC);
	frictionsurface->getMaterial().setBounciness(0);
	frictionsurface->getMaterial().setFrictionCoefficient(1.0);
	rp3d::ProxyShape* frictionsurface_proxyshape = 
		frictionsurface->addCollisionShape(&box_shape2, rp3d::Transform::identity(), 5000.0);

	//provide an impulse to the cue in the y direction
	rigidbodies[0]->setLinearVelocity(rp3d::Vector3(0.0, 40.0, 0.0));		//only for debugging

	//constant physics time step
	const float time_step = 1.0/60.0;
	float accumulator = 0;
	
	App::updateFrame();
	int iters = 0;
	bool simulation_complete = false;
	while (!glfwWindowShouldClose(App::window)) {
		//process input
		App::updateFrame();
		App::processInput();
		App::clearColor();

		iters++;
		if (iters == 2) {
			//takes arbitrarily long time in the 2nd iteration of the loop
			//std::cout << App::DELTA_TIME << std::endl;
			continue;
		}

		std::cout << "entered main loop" << std::endl;
		std::cout << "Delta time : " << App::DELTA_TIME << std::endl;

		//create transform matrices
		glm::mat4 view = App::getViewMatrix();
		glm::mat4 projection = App::getProjectionMatrix();

		axes_shader.use();
		axes_shader.setMat4("view", view);
		axes_shader.setMat4("projection", projection);
		//draw axes
		App::drawAxes();
		
		pooltable_diffuse_shader.use();
		pooltable_diffuse_shader.setMat4("model", pooltable->getModelMatrix());
		pooltable_diffuse_shader.setMat4("view", view);
		pooltable_diffuse_shader.setMat4("projection", projection);
		pooltable_diffuse_shader.setVec3("glightpos", glm::vec3(3,4,5));
		pooltable->render(&pooltable_diffuse_shader);

		pooltable_diffuse_shader.setMat4("model", cuestick->getModelMatrix());
		cuestick->render(&pooltable_diffuse_shader);

		//If the cue tip has hit the ball, then add a torque and force on the cue
		//depending on the speed which will be decided by the user
		//rigidbodies[0]->applyForceToCenter(rp3d::Vector3(2.0, 0.0, 0.0));
		//rigidbodies[0]->applyTorque(rp3d::Vector3(0.0, 3.0, 0.0));


		//before rendering the balls, need to perform physics calculation
		//add the time difference in the accumulator
		accumulator += App::DELTA_TIME ;
		//while there is enough accumulated time, take one or several physics steps
		while(accumulator >= time_step) {
			std::cout << "inside accumulator loop" << std::endl;
			//update the Dynamics world with a constant time step
			world.update(time_step);
			//decrease the accumulated time
			accumulator -= time_step ;
		}

		//check if the simulation is complete
		bool simulation_complete = true;
		for(int i = 0; i < 16; i++) {
			if (!rigidbodies[i]->isSleeping()) {
				simulation_complete = false; break;
			}
		}
		if (simulation_complete) std::cout << "Simulation Complete" << std::endl;

		//after the simulation has completed for this frame, update the positions
		//and transformations of the balls
		rp3d::decimal factor = accumulator/time_step;
		for(int i = 0; i < 16; i++) {
			rp3d::Transform curr_transform = rigidbodies[i]->getTransform();
			rp3d::Transform interpolated_transform = 
				rp3d::Transform::interpolateTransforms(prev_transforms[i], curr_transform, factor);
			float matrix[16];
			interpolated_transform.getOpenGLMatrix(matrix);
			balls[i]->model = glm::make_mat4(matrix);
			prev_transforms[i] = curr_transform;
		}
		

		ball_diffuse_shader.use();
		ball_diffuse_shader.setMat4("view", view);
		ball_diffuse_shader.setMat4("projection", projection);
		ball_diffuse_shader.setVec3("glightpos", glm::vec3(3,4,5));
		for(int i = 0; i < 16; i++) {
			ball_diffuse_shader.setMat4("model", balls[i]->getModelMatrix());
			balls[i]->render(&ball_diffuse_shader);
		}

		//accesorial actions
		App::endFrame();
	}
	glfwTerminate();
	//deallocate everythin that was dynamically created (using the new keyword);
	for(int i = 0; i < 16; i++) {
		delete balls[i];
	}
	for(int i = 0; i < 6; i++) {
		delete box_shapes[i];
	}
	delete pooltable;
	delete cuestick;

	return 0;
}

