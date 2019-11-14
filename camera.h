//
//  Created by Ayush Kumar on 6/25/18.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Pooltable.h"
#include <cmath>

#include <vector>
#include <iostream>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

#define _OFF_X  2.5f
#define _OFF_Y  2.5f
#define _OFF_Z  4.0f
#define _R_     5.0f
#define _B_(i, j) PoolTable::holes[(i)][(j)]
// Default camera values
const float _speed       =  0.5f;
const float _sensitivity =  0.1f;
const float _zoom        =  45.0f;
const glm::vec3 _path[]  =  {   
                                {-3.028,  -5.473,   1.200},
                                {-3.028,   5.473,   1.200},
                                { 3.054,   5.473,   1.200},
                                { 3.054,  -5.473,   1.200}
                            };

//const float _speed = 1.0;
//const float _sensitivity = 0.025f;
//const float _zoom = 10.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 worldup;
    glm::vec3 up;
    glm::vec3 right;
    // Euler Angles
    // They are defined as follows
    // Suppose you are looking along y-axis and x-axis is to your right and z-axis is up
    // Then pitch varies from 0 to 90 as a vector along y-axis is rotated towards z-axis
    // And yaw varies from 0 to 90 as a vector along y-axis is rotated towards x-axis
    // (pitch, yaw) completely defines the front vector of the camera and the converse
    float yaw;
    float pitch;
    // Camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
    float param_u;
    float theta;
    bool locked;
    
    // Constructor with vectors
    Camera(glm::vec3 pos = glm::vec3(0.0f, -5.577f, 1.026f),  
        glm::vec3 fr = glm::vec3(0.0f, 1.0f, 0.0f), 
        glm::vec3 u = glm::vec3(0.0f, 0.0f, 1.0f)) : 
        movement_speed(_speed), mouse_sensitivity(_sensitivity), zoom(_zoom), param_u(0), theta(M_PI / 4),
        locked(false)
    {
        //position = pos;
        updateCameraPosition();
        front = glm::normalize(fr);
        worldup = u;
        //calculate yaw and pitch values based on the front vector the user specified
        pitch = glm::degrees(asin(front.z));
        yaw = -glm::degrees(acos(front.y / cos(glm::radians(pitch))));
        zoom = _zoom;
        updateCameraVectors();
    }
    
    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() {
        /*
        //debug statements
        printf("position : %f %f %f\n", position.x, position.y, position.z);
        printf("position + front : %f %f %f\n", (position + front).x, (position + front).y, (position + front).z);
        printf("up : %f %f %f\n", up.x, up.y, up.z);
        */
        return glm::lookAt(position, position + front, up);
    }

    void toggleLock() {
        locked = !locked;
        std::cout << "camera lock: " << int(locked) << std::endl;
    }

    glm::vec3 getParametricBoundary(float u) {
        if (0 <= u && u <= 1) {
            return (u-0.0f) * _path[1] + (1.0f-u) * _path[0];
        } else if (1 <= u && u <= 2) {
            return _path[1];
        } else if (2 <= u && u <= 3) {
            return (u-2.0f) * _path[2] + (3.0f-u) * _path[1];
        } else if (3 <= u && u <= 4) {
            return _path[2];
        } else if (4 <= u && u <= 5) {
            return (u-4.0f) * _path[3] + (5.0f-u) * _path[2];
        } else if (5 <= u && u <= 6) {
            return _path[3];
        } else if (6 <= u && u <= 7) {
            return (u-6.0f) * _path[0] + (7.0f-u) * _path[3];
        } else if (7 <= u && u <= 8) {
            return _path[0];
        }
        std::cerr << "Invalid Parameter" << std::endl;
        return glm::vec3(-1, -1, -1);
    }

    glm::vec3 getParametricCameraPath(float u, float t) {
        //camera path is a rounded rectangle around the rectangle _path and radius _R_
        //when u is in [0, 1), [2, 3), [4, 5), [6, 7), it traces the rectangle _path
        u = (u - floor(u)) + (int(floor(u)) % 8 + 8) % 8;
        if (0 <= u && u < 1) {
            return u * _path[1] + (1.0f-u) * _path[0] + _R_ * glm::vec3(-cos(t), 0, sin(t));
        } else if (1 <= u && u < 2) {
            return _path[1] + _R_ * glm::vec3(-cos(t) * cos((u-1) * M_PI / 2), cos(t) * sin((u-1) * M_PI / 2), sin(t));
        } else if (2 <= u && u < 3) {
            return (u-2.0f) * _path[2] + (3.0f-u) * _path[1] + _R_ * glm::vec3(0, cos(t), sin(t));
        } else if (3 <= u && u < 4) {
            return _path[2] + _R_ * glm::vec3(cos(t) * sin((u-3) * M_PI / 2), cos(t) * cos((u-3) * M_PI / 2), sin(t));
        } else if (4 <= u && u < 5) {
            return (u-4.0f) * _path[3] + (5.0f-u) * _path[2] + _R_ * glm::vec3(cos(t), 0, sin(t));
        } else if (5 <= u && u < 6) {
            return _path[3] + _R_ * glm::vec3(cos(t) * cos((u-5) * M_PI / 2), -cos(t) * sin((u-5) * M_PI / 2), sin(t));
        } else if (6 <= u && u < 7) {
            return (u-6.0f) * _path[0] + (7.0f-u) * _path[3] + _R_ * glm::vec3(0, -cos(t), sin(t));
        } else if (7 <= u && u < 8) {
            return _path[0] + _R_ * glm::vec3(-cos(t) * sin((u-7) * M_PI / 2), -cos(t) * cos((u-7) * M_PI / 2), sin(t));
        }
        std::cerr << "Invalid Parameter" << std::endl;
        return glm::vec3(-1, -1, -1);
    }

    void updateCameraPosition() {
        /*
        if (0 <= param_u && param_u < 1) {
            position = param_u * _path[1] + (1.0f-param_u) * _path[0] + _R_ * glm::vec3(-cos(theta), 0, sin(theta));
        } else if (1 <= param_u && param_u < 2) {
            position = (param_u-1.0f) * _path[2] + (2.0f-param_u) * _path[1] + _R_ * glm::vec3(0, cos(theta), sin(theta));
        } else if (2 <= param_u && param_u < 3) {
            position = (param_u-2.0f) * _path[3] + (3.0f-param_u) * _path[2] + _R_ * glm::vec3(cos(theta), 0, sin(theta));
        } else {
            position = (param_u-3.0f) * _path[0] + (4.0f-param_u) * _path[3] + _R_ * glm::vec3(0, -cos(theta), sin(theta));
        }
        */
        position = getParametricCameraPath(param_u, theta);
    }
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(CameraMovement direction, float delta_time)
    {
        if (locked) return;
        //camera must always look towards the cueball
        float velocity = movement_speed * delta_time;
        if (direction == FORWARD) {
            //position += front * velocity;
            theta = std::min(float(M_PI/2), theta + velocity);
        }
        if (direction == BACKWARD) {
            //position -= front * velocity;
            theta = std::max(0.0f, theta - velocity);
        }
        if (direction == LEFT) {
            //position -= right * velocity;
            if ((1 <= param_u && param_u < 2) || (3 <= param_u && param_u < 4) || (5 <= param_u && param_u < 6) || (7 <= param_u && param_u < 8)) {
                //inside the circular radius
                param_u += 2 * velocity;
            } else if ((2 <= param_u && param_u < 3) || (6 <= param_u && param_u < 7)) {
                //inside the shorter side of the rectangle
                param_u += 1.5 * velocity;
            } else {
                param_u += velocity;
            }
            param_u = (param_u - floor(param_u)) + int(floor(param_u)) % 8;
        }
        if (direction == RIGHT) {
            //position += right * velocity;
            if ((1 <= param_u && param_u < 2) || (3 <= param_u && param_u < 4) || (5 <= param_u && param_u < 6) || (7 <= param_u && param_u < 8)) {
                //inside the circular radius
                param_u -= 2 * velocity;
            } else if ((2 <= param_u && param_u < 3) || (6 <= param_u && param_u < 7)) {
                //inside the shorter side of the rectangle
                param_u -= 1.2 * velocity;
            } else {
                param_u -= velocity;
            }
            param_u = (param_u - floor(param_u)) + (int(floor(param_u)) % 8 + 8) % 8;
        }    
        // make sure the user stays at the ground level (xz plane)
        updateCameraPosition();
    }
    
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch = true)
    {
        if (locked) return;
        xoffset *= mouse_sensitivity;
        yoffset *= mouse_sensitivity;
        
        yaw   += xoffset;
        pitch += yoffset;
        
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainpitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }
        // Prevent yaw from becoming too large and losing precision
        yaw = glm::mod(yaw, 360.0f);
        // update front, right and up Vectors using the updated Euler angles
        updateCameraVectors();
    }
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset)
    {
        if (locked) return;
        if (zoom >= 1.0f && zoom <= 45.0f)
            zoom -= yoffset;
        if (zoom <= 1.0f)
            zoom = 1.0f;
        if (zoom >= 45.0f)
            zoom = 45.0f;
    }
    
private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // Calculate the new front vector
        glm::vec3 fr;
        fr.y = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        fr.z = sin(glm::radians(pitch));
        fr.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(fr);
        // Also re-calculate the right and up vector
        right = glm::normalize(glm::cross(front, worldup));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }
};
#endif
