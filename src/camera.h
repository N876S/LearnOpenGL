#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
        glm::vec3 cameraPos;
        glm::vec3 cameraDir;
        glm::vec3 cameraUp;
        glm::vec3 cameraRight;

        float movementSpeed;
        float sensitivity;
        float pitch;
        float yaw;

        float lastMouseX = 1920.0f/2.0f;
        float lastMouseY = 1080.0f/2.0f;

        bool firstMouseSnapFix = true;

        Camera(glm::vec3 cameraPos, glm::vec3 cameraDir, float movementSpeed, float sensitivity){
            this->cameraPos = cameraPos;
            this->cameraDir = cameraDir;
            this->movementSpeed = movementSpeed;
            this->sensitivity = sensitivity;
            updateBasis();

            pitch = glm::degrees(asin(cameraDir.y));
            yaw = glm::degrees(glm::atan(cameraDir.z/cameraDir.x));
        }
        
        void updatePosition(glm::vec3 offset){
            cameraPos += offset;
        }

        void updateDirection(double xpos, double ypos){
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

        void updateBasis(){
            cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
            cameraRight = glm::normalize(glm::cross(cameraDir, cameraUp));
        }

        glm::mat4 getView(){
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(cameraPos, cameraDir + cameraPos, cameraUp);
            return view;
        }
};

#endif