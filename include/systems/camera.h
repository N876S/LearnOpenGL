#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:

    //-----------------------------------FIELDS-------------------------------------------

    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    float movementSpeed;
    float sensitivity;
    float pitch;
    float yaw;

    float lastMouseX;
    float lastMouseY;

    bool firstMouseSnapFix = true;

    //---------------------------------CONSTRUCTOR-----------------------------------------

    Camera(glm::vec3 cameraPos, glm::vec3 cameraDir, float movementSpeed, float sensitivity);
    Camera() = default;

    //------------------------------------METHODS------------------------------------------
    
    void updatePosition(glm::vec3 offset);
    void updateDirection(double xpos, double ypos);
    void updateBasis();

    glm::mat4 getView();
    glm::vec3 getPosition();
};

#endif