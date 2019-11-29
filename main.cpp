//
//  Created by Ayush Kumar on 27/09/19.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#include <Headers/App.h>

//model loading library
#include <Headers/objloader.hpp>
#include <Headers/shader.h>

#include <Headers/Gamestate.h>
#include <Headers/glm_includes.h>
#include <stb_image/stb_image.h>
#include <Headers/Environment.h>

#include <iostream>
#include <vector>
#include <cassert>

int main() {
	App();
	GameState* gamestate = new GameState();
	Environment::loadVertexData();
	Environment* env = new Environment();
	//App::gamestate = gamestate;
	
	//building shaders
	Shader pooltable_diffuse_shader("shaders/pooltable_diffuse.vs", "shaders/pooltable_diffuse.fs");
	Shader ball_diffuse_shader("shaders/ball_diffuse.vs", "shaders/ball_diffuse.fs");
	Shader axes_shader("shaders/axes.vs", "shaders/axes.fs");
	Shader env_shader("shaders/cubemap.vs", "shaders/cubemap.fs");

	App::updateFrame();
	int iters = 0;

	//gamestate->cuestick->model = glm::translate(glm::vec3(0.1, 0.5, 0.1)) * gamestate->cuestick->model;

	while (!glfwWindowShouldClose(App::window)) {
		//process input
		App::updateFrame();
		//if (gamestate->simulation_complete) {
		App::processInput();
		//}
		App::clearColor();

		iters++;
		if (iters == 2) {
			//takes arbitrarily long time in the 2nd iteration of the loop
			//std::cout << App::DELTA_TIME << std::endl;
			continue;
		}

		//std::cout << "entered main loop" << std::endl;
		//std::cout << "Delta time : " << App::DELTA_TIME << std::endl;
		//std::cout << "theta: " << App::CAMERA.theta << std::endl;

		//create transform matrices
		glm::mat4 view = App::getViewMatrix();
		glm::mat4 projection = App::getProjectionMatrix();
		//remove translation to create a fake_view matrix
		glm::mat4 fake_view = glm::mat4(glm::mat3(view));

		env_shader.use();
		env_shader.setMat4("model", env->getModelMatrix());
		env_shader.setMat4("view", fake_view);
		env_shader.setMat4("projection", projection);
		env->render(&env_shader);

		axes_shader.use();
		axes_shader.setMat4("view", view);
		axes_shader.setMat4("projection", projection);
		//draw axes
		//App::drawAxes();
		gamestate->pooltable->drawBoundary();
		if (gamestate->simulation_complete) {
			gamestate->renderAim();
			//render fakeball
		}
		
		pooltable_diffuse_shader.use();
		pooltable_diffuse_shader.setMat4("model", gamestate->pooltable->getModelMatrix());
		pooltable_diffuse_shader.setMat4("view", view);
		pooltable_diffuse_shader.setMat4("projection", projection);
		pooltable_diffuse_shader.setVec3("glightpos", glm::vec3(3,4,5));
		gamestate->pooltable->render(&pooltable_diffuse_shader);

		if (gamestate->simulation_complete) {
			gamestate->setCueStick();
			glm::vec3 force_dirn = gamestate->cuestick->aim;
			force_dirn[2] = 0.0f;
			glm::vec3 force = 25.0f * (1.0f - gamestate->cuestick->animate_factor) * force_dirn;
			if (App::HIT) {
				//std::cout << "HIT : " << gamestate->cuestick->animate_factor << std::endl;
				gamestate->simulation_complete = false;
				assert(gamestate->rigidbodies[0]);
				gamestate->rigidbodies[0]->setLinearVelocity(rp3d::Vector3(force[0], force[1], force[2]));
				gamestate->rigidbodies[0]->setAngularVelocity(rp3d::Vector3(0.0, 0.0, 10.0));
				App::HIT = false;
				//Apply force to the cue
			}
			float radius = App::getMouseDrag();
			if (radius != -1) gamestate->cuestick->animate(radius);
			else gamestate->cuestick->unanimate();
			pooltable_diffuse_shader.setMat4("model", gamestate->cuestick->getModelMatrix());
			//renderAim();
			//it should be line till the first intersection with a ball/wall and at that point render a small fake ball
			gamestate->cuestick->render(&pooltable_diffuse_shader);
		}

		
		//after the simulation has completed for this frame, update the positions
		//and transformations of the balls
		gamestate->simulate();
		if (!gamestate->simulation_complete) {
			gamestate->updateState();
		}

		if (gamestate->simulation_complete && iters > 2) {
			gamestate->performBallCheck();
		}
		

		ball_diffuse_shader.use();
		ball_diffuse_shader.setMat4("view", view);
		ball_diffuse_shader.setMat4("projection", projection);
		ball_diffuse_shader.setVec3("glightpos", glm::vec3(3,4,5));
		for(int i = 0; i < 16; i++) {
			ball_diffuse_shader.setMat4("model", gamestate->balls[i]->getModelMatrix());
			if (!gamestate->balls[i]->scored) {
				gamestate->balls[i]->render(&ball_diffuse_shader);
			}
		}
		if (gamestate->simulation_complete) {
			//render fakeball
			ball_diffuse_shader.setMat4("model", gamestate->fakeball->getModelMatrix());
			//switch to wireframe mode
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			gamestate->fakeball->render(&ball_diffuse_shader);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		//accesorial actions
		App::endFrame();
	}
	glfwTerminate();
	//deallocate everything that was dynamically created (using the new keyword);
	delete gamestate;
	delete env;

	return 0;
}
