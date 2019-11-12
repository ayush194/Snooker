//
//  Created by Ayush Kumar on 6/25/18.
//  Copyright © 2018 Ayush Kumar. All rights reserved.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float _speed       =  2.5f;
const float _sensitivity =  0.1f;
const float _zoom        =  45.0f;

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
    
    // Constructor with vectors
    Camera(glm::vec3 pos = glm::vec3(0.0f, -5.577f, 1.026f),  
        glm::vec3 fr = glm::vec3(0.0f, 1.0f, 0.0f), 
        glm::vec3 u = glm::vec3(0.0f, 0.0f, 1.0f)) : 
        movement_speed(_speed), mouse_sensitivity(_sensitivity), zoom(_zoom)
    {
        position = pos;
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
    
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboard(CameraMovement direction, float delta_time)
    {
        float velocity = movement_speed * delta_time;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        // make sure the user stays at the ground level (xz plane)
        //position.y = 0.0f;
    }
    
    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch = true)
    {
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
