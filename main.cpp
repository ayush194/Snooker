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
#include "Gamestate.h"
#include "glm_includes.h"

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
	GameState* gamestate = new GameState();

	cuestick->pos = glm::vec3(0.0, 0.0, 1.5);
	
	for(int i = 0; i < 16; i++) {
		balls[i] = new Ball(i);
		//balls[i]->pos = glm::vec3(balls_pos_i[i][0], balls_pos_i[i][1], balls_pos_i[i][2]);
	}
	
	//building shaders
	Shader pooltable_diffuse_shader("shaders/pooltable_diffuse.vs", "shaders/pooltable_diffuse.fs");
	Shader ball_diffuse_shader("shaders/ball_diffuse.vs", "shaders/ball_diffuse.fs");
	Shader axes_shader("shaders/axes.vs", "shaders/axes.fs");

	App::updateFrame();
	int iters = 0;

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

		
		//after the simulation has completed for this frame, update the positions
		//and transformations of the balls
		gamestate->simulate();
		gamestate->updateState(balls);
		

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
	//deallocate everything that was dynamically created (using the new keyword);
	delete gamestate;
	for(int i = 0; i < 16; i++) {
		delete balls[i];
	}
	delete pooltable;
	delete cuestick;

	return 0;
}
