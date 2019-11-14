#include "Gamestate.h"

float GameState::balls_pos_i[][3] = {
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

	//Create 6 more static rigid bodies for the 6 pieces of walls
float GameState::walls_pos[][3] = {
                                {-3.107, -2.732,  1.026},
                                {-3.107,  2.732,  1.026},
                                { 0.020,  5.562,  1.026},
                                { 3.148,  2.732,  1.026},
                                { 3.128, -2.732,  1.026},
                                { 0.020, -5.572,  1.026}
                            };

float GameState::walls_shape[][3] = {
                                { 0.100,  2.450,  2.000},
                                { 0.100,  2.450,  2.000},
                                { 2.728,  0.080,  2.000},
                                { 0.100,  2.450,  2.000},
                                { 0.100,  2.450,  2.000},
                                { 2.728,  0.080,  2.000}
                            };

float GameState::outer_walls_pos[][3] = {
                                {-3.502,  0.000,  0.000},
                                { 0.000,  5.809,  0.000},
                                { 3.547,  0.000,  0.000},
                                { 0.000, -5.809,  0.000},
                            };
float GameState::outer_walls_shape[][3] = {
                                { 0.100,  7.000,  2.000},
                                { 5.000,  0.100,  2.000},
                                { 0.100,  7.000,  2.000},
                                { 5.000,  0.100,  2.000},
                            };
//0.126 to match the height of the actual wall
//Initial Position of the balls

GameState::GameState() : time_step(1.0/60.0) {
    PoolTable::loadVertexData();
    CueStick::loadVertexData();
    Ball::loadVertexData();

    this->pooltable = new PoolTable(0);
    this->cuestick = new CueStick(0);
    for(int i = 0; i < 16; i++) {
		this->balls[i] = new Ball(i);
	}

    //using ReactPhysics3d library
	rp3d::Vector3 gravity(0.0, 0.0, -9.81);
	this->world = new rp3d::DynamicsWorld(gravity);
	this->world->enableSleeping(true);
	//world->setSleepLinearVelocity();		//threshold for when to put the body on sleep
	//world->setSleepAngularVelocity();
	
	// Initial position and orientation of the rigid body
	rp3d::Vector3 init_position(0.0, 0.0, 0.0) ;
	rp3d::Quaternion init_orientation = rp3d::Quaternion::identity();
	rp3d::Transform transform(init_position, init_orientation) ;
    for(int i = 0; i < 16; i++) {
        transform.setPosition(rp3d::Vector3(GameState::balls_pos_i[i][0], GameState::balls_pos_i[i][1], GameState::balls_pos_i[i][2]));
        this->prev_transforms[i] = transform;
    }
	
    //Create 16 dynamic rigid bodies and atttach the shape sphere with each one of them
    this->sphere_shape = new rp3d::SphereShape(Ball::radius);
	for(int i = 0; i < 16; i++) {
		transform.setPosition(rp3d::Vector3(GameState::balls_pos_i[i][0], GameState::balls_pos_i[i][1], GameState::balls_pos_i[i][2]));
		this->rigidbodies[i] = this->world->createRigidBody(transform);
		this->rigidbodies[i]->getMaterial().setBounciness(0.2);
		this->rigidbodies[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.06));
		this->rigidbodies[i]->getMaterial().setRollingResistance(rp3d::decimal(0.1));
		//rigidbodies[i]->setLinearDamping(0.05);
		//rigidbodies[i]->setAngularDamping(0.05);
		this->proxyshapes[i] = 
			this->rigidbodies[i]->addCollisionShape(this->sphere_shape, rp3d::Transform::identity(), (i == 0 ? Ball::mass_cue : Ball::mass_ball));
		this->prev_transforms[i] = rp3d::Transform(init_position, init_orientation);
	}
	
    //6 walls with holes lined in between them
	for(int i = 0; i < 6; i++) {
		transform.setPosition(rp3d::Vector3(GameState::walls_pos[i][0],GameState::walls_pos[i][1], GameState::walls_pos[i][2]));
		this->rigidwalls[i] = this->world->createRigidBody(transform);
		this->rigidwalls[i]->setType(rp3d::BodyType::STATIC);
		this->rigidwalls[i]->getMaterial().setBounciness(0.6);
		this->rigidwalls[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.2));
		this->box_shapes[i] = new rp3d::BoxShape(rp3d::Vector3(GameState::walls_shape[i][0], GameState::walls_shape[i][1], GameState::walls_shape[i][2]));
		this->box_proxyshapes[i] = this->rigidwalls[i]->addCollisionShape(this->box_shapes[i], rp3d::Transform::identity(), 5000.0);
	}

    //4 outer restricting walls
    for(int i = 0; i < 4; i++) {
        transform.setPosition(rp3d::Vector3(GameState::outer_walls_pos[i][0],GameState::outer_walls_pos[i][1], GameState::outer_walls_pos[i][2]));
		this->rigidwalls_outer[i] = this->world->createRigidBody(transform);
		this->rigidwalls_outer[i]->setType(rp3d::BodyType::STATIC);
		this->rigidwalls_outer[i]->getMaterial().setBounciness(0);
		this->rigidwalls_outer[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.06));
		this->box_shapes_outer[i] = new rp3d::BoxShape(rp3d::Vector3(GameState::outer_walls_shape[i][0], GameState::outer_walls_shape[i][1], GameState::outer_walls_shape[i][2]));
		this->box_proxyshapes_outer[i] = this->rigidwalls_outer[i]->addCollisionShape(this->box_shapes_outer[i], rp3d::Transform::identity(), 5000.0);
    }


	//And 1 static rigid body for the surface of the pooltable
	transform.setPosition(rp3d::Vector3(0.020, 0.0, 0.952));
	this->surface = this->world->createRigidBody(transform);
	this->box_shape = new rp3d::BoxShape(rp3d::Vector3(/*2.950*/3.0, 5.420, 0.001));
	this->surface->setType(rp3d::BodyType::STATIC);
	this->surface->getMaterial().setBounciness(0);
	this->surface->getMaterial().setFrictionCoefficient(0.1);
	this->surface_proxyshape = this->surface->addCollisionShape(this->box_shape, rp3d::Transform::identity(), 5000.0);
	
	//Add 1 rigid high-friction surface below the pool table surface
	transform.setPosition(rp3d::Vector3(0.0, 0.0, 0.252));
	this->frictionsurface = this->world->createRigidBody(transform);
	this->box_shape2 = new rp3d::BoxShape(rp3d::Vector3(20.127, 20.016, 0.001));
	this->frictionsurface->setType(rp3d::BodyType::STATIC);
	this->frictionsurface->getMaterial().setBounciness(0);
	this->frictionsurface->getMaterial().setFrictionCoefficient(1.0);
	this->frictionsurface_proxyshape = 
		this->frictionsurface->addCollisionShape(this->box_shape2, rp3d::Transform::identity(), 5000.0);

	//provide an impulse to the cue in the y direction
	//this->rigidbodies[0]->setLinearVelocity(rp3d::Vector3(0.0, 40.0, 0.0));		//only for debugging
	
    this->turn = 0;         //Currently, player 0's turn
    this->turn_continued = false;
    this->fault_occurred = false;
    this->score[0] = this->score[1] = 0;
	this->accumulator = 0;
    this->simulation_complete = true;
}

GameState::~GameState() {
    //allocate all variable initialized with the new keyword
    for(int i = 0; i < 6; i++) {
		delete this->box_shapes[i];
	}
    for(int i = 0; i < 16; i++) {
		delete balls[i];
	}
	delete pooltable;
	delete cuestick;
}

void GameState::simulate() {
	//If the cue tip has hit the ball, then add a torque and force on the cue
	//depending on the speed which will be decided by the user
	//rigidbodies[0]->applyForceToCenter(rp3d::Vector3(2.0, 0.0, 0.0));
	//rigidbodies[0]->applyTorque(rp3d::Vector3(0.0, 3.0, 0.0));


	//before rendering the balls, need to perform physics calculation
	//add the time difference in the accumulator
	this->accumulator += App::DELTA_TIME ;
	//while there is enough accumulated time, take one or several physics steps
	while(this->accumulator >= this->time_step) {
		//std::cout << "inside accumulator loop" << std::endl;
		//update the Dynamics world with a constant time step
		this->world->update(GameState::time_step);
		//decrease the accumulated time
		this->accumulator -= GameState::time_step ;
	}

    //check if the simulation is complete
    this->simulation_complete = true;
    for(int i = 0; i < 16; i++) {
        if (this->rigidbodies[i] && !this->rigidbodies[i]->isSleeping()) {
            this->simulation_complete = false; break;
        }
    }
    //if (this->simulation_complete) std::cout << "Simulation Complete" << std::endl;
}

void GameState::hit() {
    
}

glm::vec3 GameState::setCueStick() {
    //the cuestick should always lie along the line joining the camera with the cue ball
    glm::mat4 rot_init = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 cue_position = glm::vec3(this->balls[0]->model[3]);
    //get the projection of the camera->cue vector on the surface of the pooltable
    glm::vec3 boundary_position = App::CAMERA.getParametricBoundary(App::CAMERA.param_u);
    //glm::ved3 camers_offset_position = glm::vec3();
    glm::vec3 cuestick_dirn(glm::normalize(cue_position - boundary_position));
    //currently, the snooker points in the positive z direction
    //create a transform such that the cuestick lies along the cuestick_dirn
    glm::vec3 rand_dirn = glm::normalize(glm::vec3(cuestick_dirn[0], cuestick_dirn[1], cuestick_dirn[2] + 0.5));
    glm::vec3 dirn1 = glm::normalize(glm::cross(cuestick_dirn, rand_dirn));
    glm::vec3 dirn2 = glm::normalize(glm::cross(cuestick_dirn, dirn1));
    //now dirn1, dirn2, cuestick_dirn is an orthogonal set of vectors
    float tmp[16] = {   dirn1[0], dirn1[1], dirn1[2], 0,
                        dirn2[0], dirn2[1], dirn2[2], 0,
                        cuestick_dirn[0], cuestick_dirn[1], cuestick_dirn[2], 0,
                        0, 0, 0, 1};
    glm::mat4 cuestick_transform =  glm::make_mat4(tmp);
    glm::mat4 translation_transform = glm::translate(boundary_position + this->cuestick->animate_factor * (cue_position - boundary_position));
    this->cuestick->model = translation_transform * cuestick_transform * rot_init;
    return cuestick_dirn;
}

void GameState::updateState() {
    rp3d::decimal factor = accumulator/time_step;
    for(int i = 0; i < 16; i++) {
        if (!this->rigidbodies[i]) continue;
        rp3d::Transform curr_transform = rigidbodies[i]->getTransform();
        rp3d::Transform interpolated_transform = 
            rp3d::Transform::interpolateTransforms(this->prev_transforms[i], curr_transform, factor);
        float matrix[16];
        interpolated_transform.getOpenGLMatrix(matrix);
        this->balls[i]->model = glm::make_mat4(matrix);
        this->prev_transforms[i] = curr_transform;
    }
}

void GameState::performBallCheck() {
    this->turn_continued = false;
    this->fault_occurred = false;
    for(int i = 0; i < 16; i++) {
        if (!rigidbodies[i]) continue;
        glm::vec3 translation = glm::column(this->balls[i]->model, 3);
        if (translation[2] < 0.962) {
            //this ball has entered a hole, hence remove it from the scene
            if (i == 0) {
                //fault occured
                fault_occurred = true;
                score[turn] -= 5;
                //place the cue ball and all scored balls back on the table
                this->balls[0]->model = glm::translate(glm::vec3(balls_pos_i[0][0], balls_pos_i[0][1], balls_pos_i[0][2]));
                this->rigidbodies[0]->setLinearVelocity(rp3d::Vector3(0.0, 0.0, 0.0));
                this->rigidbodies[0]->setAngularVelocity(rp3d::Vector3(0.0, 0.0, 0.0));
                rp3d::Transform t(rp3d::Vector3(balls_pos_i[0][0], balls_pos_i[0][1], balls_pos_i[0][2]), rp3d::Quaternion::identity());
                this->rigidbodies[0]->setTransform(t);
            } else {
                if (fault_occurred) {
                    //place the ball back on the board;
                    this->balls[i]->model = glm::translate(glm::vec3(balls_pos_i[i][0], balls_pos_i[i][1], balls_pos_i[i][2]));
                    this->rigidbodies[i]->setLinearVelocity(rp3d::Vector3(0.0, 0.0, 0.0));
                    this->rigidbodies[i]->setAngularVelocity(rp3d::Vector3(0.0, 0.0, 0.0));
                    rp3d::Transform t(rp3d::Vector3(balls_pos_i[i][0], balls_pos_i[i][1], balls_pos_i[i][2]), rp3d::Quaternion::identity());
                    this->rigidbodies[i]->setTransform(t);
                    score[turn] -= i;
                    continue;
                }
                this->balls[i]->scored = true;
                turn_continued = true;
                balls_on_board--;
                score[turn] += i;
                this->world->destroyRigidBody(this->rigidbodies[i]);
                this->rigidbodies[i] = NULL;
                this->proxyshapes[i] = NULL;
            }
        }
    }
    if (balls_on_board == 0) {
        //gameover
        std::cout << std::endl;
        std::cout << "Player 1 score : " << score[0] << std::endl;
        std::cout << "Player 2 score : " << score[1] << std::endl;
        std::cout << std::endl;
        if (score[0] > score[1]) {
            std::cout << "Player 1 wins!" << std::endl;
        } else if (score[0] < score[1]) {
            std::cout << "Player 2 wins!" << std::endl;
        } else {
            std::cout << "It's a rare tie! Hats off to both of you for such an interesting game!" << std::endl;
        }
        std::cout << std::endl;
    }
    if (!this->turn_continued) {
        turn = (turn + 1) % 2;
    }
}