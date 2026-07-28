#include "systems/camera.h"
#include "core/engine.h"

Camera::Camera(glm::vec3 cameraPos, glm::vec3 cameraDir, float movementSpeed, float sensitivity){
    this->cameraPos = cameraPos;
    this->cameraDir = cameraDir;
    this->movementSpeed = movementSpeed;
    this->sensitivity = sensitivity;
    lastMouseX = Engine::getWindowWidth()/2.0f;
    lastMouseY = Engine::getWindowHeight()/2.0f;
    updateBasis();

    pitch = glm::degrees(asin(cameraDir.y));
    yaw = glm::degrees(glm::atan(cameraDir.z/cameraDir.x));
}

void Camera::updatePosition(glm::vec3 offset){
    cameraPos += offset;
}

void Camera::updateDirection(double xpos, double ypos){
    if(firstMouseSnapFix == true){
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouseSnapFix = false;
    }

    float mouseOffsetX = xpos-lastMouseX;
    float mouseOffsetY = lastMouseY-ypos;

    lastMouseX = xpos;
    lastMouseY = ypos;

    mouseOffsetX *= sensitivity;
    mouseOffsetY *= sensitivity;

    pitch += mouseOffsetY;
    yaw += mouseOffsetX;

    if(pitch > 89.9f){
        pitch = 89.9f;
    }
    if(pitch < -89.9f){
        pitch = -89.9f;
    }

    cameraDir = glm::normalize(glm::vec3(cos(glm::radians(yaw))*cos(glm::radians(pitch)), sin(glm::radians(pitch)), sin(glm::radians(yaw))*cos(glm::radians(pitch))));
}

void Camera::updateBasis(){
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraRight = glm::normalize(glm::cross(cameraDir, cameraUp));
}

glm::mat4 Camera::getView(){
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraDir + cameraPos, cameraUp);
    return view;
}

glm::vec3 Camera::getPosition(){
    return cameraPos;
}

glm::vec3 Camera::getDir(){
    return cameraDir;
}