#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "shader.h"
#include "input.h"
#include "model.h"

class Engine {
    public:

    //-----------------------------------FIELDS-------------------------------------------
    const int WINDOW_WIDTH = 1920;
    const int WINDOW_HEIGHT = 1080;
    const char* WINDOW_NAME = "LearnOpenGL";
    const int OPENGL_VERSION_MAJOR = 4;
    const int OPENGL_VERSION_MINOR = 6;

    GLFWwindow* window;

    float fov = 45.0f;
    float time = glfwGetTime();
    float lastTime = time;
    float deltaTime = time-lastTime;

    Camera camera;
    Shader objectShader;

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    float data[180] = { 
        //POSITION           TEXTURE COORDS
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,              //FACE 1
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,  
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,              //FACE 2
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,          //FACE 3
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,           //FACE 4
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,            //FACE 5
        0.5f, -0.5f, 0.5, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,             //FACE 6
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    std::vector<glm::vec3> positions = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    Model cube;
    void* inputPointers[3]= {&camera, &deltaTime, &fov};

    //---------------------------------CONSTRUCTOR-----------------------------------------

    Engine(){
        setupGLFW();
        createWindow();

        camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 5.0f, 0.1f);
        objectShader = Shader("../shaders/shader.vs", "../shaders/shader.fs");
        cube = Model(&objectShader, data, 180, "../res/image.jpg");

        glfwSetWindowUserPointer(window, &inputPointers);
    }

    //------------------------------------METHODS------------------------------------------

    void render(){
        while (!glfwWindowShouldClose(window)) {
            //global time updates
            time = glfwGetTime();
            deltaTime = time-lastTime;
            lastTime = time;

            //input
            processInput(window);

            //clear buffers
            glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //set view and proj matrices
            objectShader.use();
            objectShader.setMatrix4f("view", camera.getView());
            projectionMatrix = glm::perspective(glm::radians(fov), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
            objectShader.setMatrix4f("proj", projectionMatrix);
            glfwSetWindowUserPointer(window, &inputPointers);

            //render objects
            cube.render(positions, time);

            //callbacks & buffers
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        glfwTerminate();
    }

    void setupGLFW(){
        // initialize GLFW
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }

    void createWindow(){
        // make window
        window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to make GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);

        // initialize GLAD (after window)
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to load GLAD" << std::endl;
        }

        // one time calls
        // set viewport
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        //set callback functions
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        //enable buffers
        glEnable(GL_DEPTH_TEST);
        //enable vsync
        glfwSwapInterval(1);
        //capture cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
};

#endif