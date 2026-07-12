#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>  //must be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include "camera.h"
#include "shader.h"
#include "input.h"
#include "model.h"
#include "light.h"

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

    std::vector<Shader> shaders;

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);

    float data[288] = { 
        //POSITION           //NORMALS          TEXTURE COORDS
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,              //FACE 1 (BACK)
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,              //FACE 2 (FRONT)
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,          //FACE 3 (LEFT)
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,           //FACE 4 (RIGHT)
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,            //FACE 5 (BOTTOM)
        0.5f, -0.5f, 0.5, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,             //FACE 6 (TOP)
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
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

    float data2[36] = {
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f
    };

    Mesh cube;
    Model crate;
    Mesh platform;
    Model ground;

    //Light light;
    //Shader lightShader;
    Shader groundShader;

    void* inputPointers[3]= {&camera, &deltaTime, &fov};

    //---------------------------------CONSTRUCTOR-----------------------------------------

    Engine(){
        setupGLFW();
        createWindow();

        camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 5.0f, 0.1f);

        objectShader = Shader("../shaders/shader.vs", "../shaders/shader.fs");
        //lightShader = Shader("../shaders/lightShader.vs", "../shaders/lightShader.fs");
        groundShader = Shader("../shaders/groundShader.vs", "../shaders/groundShader.fs");
        shaders.push_back(objectShader);
        //shaders.push_back(lightShader);
        shaders.push_back(groundShader);

        cube = Mesh(data, sizeof(data)/sizeof(data[0]), 8);
        platform = Mesh(data2, sizeof(data2)/sizeof(data2[0]), 6);

        //light = Light(&lightShader, cube, glm::vec3(4.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), {0.4f, 0.6f, 1.0f});

        crate = Model(&objectShader, cube, "../res/container.png", "../res/specularmap.png", MaterialType::Brass);
        ground = Model(&groundShader, platform, glm::vec3(0.2f, 0.2f, 0.2f), MaterialType::Obsidian);

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
            viewMatrix = camera.getView();
            projectionMatrix = glm::perspective(glm::radians(fov), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
            setSpaces();
            glfwSetWindowUserPointer(window, &inputPointers);

            //light.setColor(glm::vec3(abs(sin(time)), 1.0f, 1.0f));

            objectShader.set3f("camPos", camera.getPosition());
            groundShader.set3f("camPos", camera.getPosition());
            //setLights();

            //render objects
            crate.render(positions, time);
            //light.render(time);
            ground.render(glm::vec3(0.0f, -5.0f, 0.0f), time);

            //callbacks & buffers
            glfwPollEvents();
            glfwSwapBuffers(window);
        }
        glfwTerminate();
    }

    /*
    void setLights(){
        objectShader.set3f("light.pos", light.getPosition());
        objectShader.set3f("light.color", light.getColor());
        objectShader.setFloat("light.ambient", light.getIntensity().ambient);
        objectShader.setFloat("light.diffuse", light.getIntensity().diffuse);
        objectShader.setFloat("light.specular", light.getIntensity().specular);

        groundShader.set3f("light.pos", light.getPosition());
        groundShader.set3f("light.color", light.getColor());
        groundShader.setFloat("light.ambient", light.getIntensity().ambient);
        groundShader.setFloat("light.diffuse", light.getIntensity().diffuse);
        groundShader.setFloat("light.specular", light.getIntensity().specular);
    }*/

    void setSpaces(){
        for(Shader s : shaders){
            s.use();
            s.setMatrix4f("view", viewMatrix);
            s.setMatrix4f("proj", projectionMatrix);
        }
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