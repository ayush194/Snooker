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
                                {-3.107,  -2.732, 1.026},
                                {-3.107,  2.732,  1.026},
                                {0.020,   5.562,  1.026},
                                {3.148,   2.732,  1.026},
                                {3.148,   -2.732, 1.026},
                                {0.020,   -5.572, 1.026}
                            };

float GameState::walls_shape[][3] = {
                                {0.1,     2.450,  0.126},
                                {0.1,     2.450,  0.126},
                                {2.728,   0.08,   0.126},
                                {0.1,     2.450,  0.126},
                                {0.1,     2.450,  0.126},
                                {2.728,   0.08,   0.126}
                            };

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
	
    //Create 16 dynamic rigid bodies and atttach the shape sphere with each one of them
    this->sphere_shape = new rp3d::SphereShape(Ball::radius);
	for(int i = 0; i < 16; i++) {
		transform.setPosition(rp3d::Vector3(GameState::balls_pos_i[i][0], GameState::balls_pos_i[i][1], GameState::balls_pos_i[i][2]));
		this->rigidbodies[i] = this->world->createRigidBody(transform);
		this->rigidbodies[i]->getMaterial().setBounciness(0);
		this->rigidbodies[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.3));
		this->rigidbodies[i]->getMaterial().setRollingResistance(rp3d::decimal(0.3));
		//rigidbodies[i]->setLinearDamping(0.05);
		//rigidbodies[i]->setAngularDamping(0.05);
		this->proxyshapes[i] = 
			this->rigidbodies[i]->addCollisionShape(this->sphere_shape, rp3d::Transform::identity(), Ball::mass);
		this->prev_transforms[i] = rp3d::Transform(init_position, init_orientation);
	}
	
	for(int i = 0; i < 6; i++) {
		transform.setPosition(rp3d::Vector3(GameState::walls_pos[i][0],GameState::walls_pos[i][1], GameState::walls_pos[i][2]));
		this->rigidwalls[i] = this->world->createRigidBody(transform);
		this->rigidwalls[i]->setType(rp3d::BodyType::STATIC);
		this->rigidwalls[i]->getMaterial().setBounciness(0);
		this->rigidwalls[i]->getMaterial().setFrictionCoefficient(rp3d::decimal(0.06));
		this->box_shapes[i] = new rp3d::BoxShape(rp3d::Vector3(GameState::walls_shape[i][0], GameState::walls_shape[i][1], GameState::walls_shape[i][2]));
		this->box_proxyshapes[i] = this->rigidwalls[i]->addCollisionShape(this->box_shapes[i], rp3d::Transform::identity(), 5000.0);
	}

	//And 1 static rigid body for the surface of the pooltable
	transform.setPosition(rp3d::Vector3(0.0, 0.0, 0.952));
	this->surface = this->world->createRigidBody(transform);
	this->box_shape = new rp3d::BoxShape(rp3d::Vector3(6.127 / 2, 11.016 / 2, 0.001 / 2));
	this->surface->setType(rp3d::BodyType::STATIC);
	this->surface->getMaterial().setBounciness(0);
	this->surface->getMaterial().setFrictionCoefficient(0.3);
	this->surface_proxyshape = this->surface->addCollisionShape(this->box_shape, rp3d::Transform::identity(), 5000.0);
	
	//Add 1 rigid high-friction surface below the pool table surface
	transform.setPosition(rp3d::Vector3(0.0, 0.0, 0.652));
	this->frictionsurface = this->world->createRigidBody(transform);
	this->box_shape2 = new rp3d::BoxShape(rp3d::Vector3(6.127, 11.016, 0.001));
	this->frictionsurface->setType(rp3d::BodyType::STATIC);
	this->frictionsurface->getMaterial().setBounciness(0);
	this->frictionsurface->getMaterial().setFrictionCoefficient(1.0);
	this->frictionsurface_proxyshape = 
		this->frictionsurface->addCollisionShape(this->box_shape2, rp3d::Transform::identity(), 5000.0);

	//provide an impulse to the cue in the y direction
	//this->rigidbodies[0]->setLinearVelocity(rp3d::Vector3(0.0, 40.0, 0.0));		//only for debugging
	
	this->accumulator = 0;
    this->simulation_complete = false;
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
		std::cout << "inside accumulator loop" << std::endl;
		//update the Dynamics world with a constant time step
		this->world->update(GameState::time_step);
		//decrease the accumulated time
		this->accumulator -= GameState::time_step ;
	}

    //check if the simulation is complete
    this->simulation_complete = true;
    for(int i = 0; i < 16; i++) {
        if (!this->rigidbodies[i]->isSleeping()) {
            this->simulation_complete = false; break;
        }
    }
    if (this->simulation_complete) std::cout << "Simulation Complete" << std::endl;
}

void GameState::setCueStick() {
    //the cuestick should always lie along the line joining the camera with the cue ball
    glm::mat4 rot_init = glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 cue_position = glm::vec3(this->balls[0]->model[3]);
    //get the projection of the camera->cue vector on the surface of the pooltable
    
    //glm::ved3 camers_offset_position = glm::vec3();
    glm::vec3 cuestick_dirn(glm::normalize(cue_position - App::CAMERA.position));
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
    glm::mat4 translation_transform = glm::translate(App::CAMERA.position + cuestick_dirn * 2.0f);
    this->cuestick->model = translation_transform * cuestick_transform * rot_init;
}

void GameState::updateState() {
    rp3d::decimal factor = accumulator/time_step;
    for(int i = 0; i < 16; i++) {
        rp3d::Transform curr_transform = rigidbodies[i]->getTransform();
        rp3d::Transform interpolated_transform = 
            rp3d::Transform::interpolateTransforms(this->prev_transforms[i], curr_transform, factor);
        float matrix[16];
        interpolated_transform.getOpenGLMatrix(matrix);
        this->balls[i]->model = glm::make_mat4(matrix);
        this->prev_transforms[i] = curr_transform;
    }
}