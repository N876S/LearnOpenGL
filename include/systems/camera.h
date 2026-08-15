#pragma once

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    private:

    //-----------------------------------FIELDS-------------------------------------------

    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 400.0f;

    float movementSpeed;
    float sensitivity;
    float pitch;
    float yaw;

    float lastMouseX;
    float lastMouseY;

    bool firstMouseSnapFix = true;

    public:

    //---------------------------------CONSTRUCTOR-----------------------------------------

    Camera(glm::vec3 cameraPos, glm::vec3 cameraDir, float movementSpeed, float sensitivity);
    Camera() = default;

    //------------------------------------METHODS------------------------------------------
    
    void updatePosition(glm::vec3 offset);
    void updateDirection(double xpos, double ypos);
    void updateBasis();
    void updateFov(float fovOffset);
    void setFov(float fov);

    glm::mat4 getView();
    glm::vec3 getPosition();
    glm::vec3 getDir();
    glm::vec3 getRight();
    glm::vec3 getUp();
    float getFov();
    float getNearPlane();
    float getFarPlane();
    float getMovementSpeed();
};