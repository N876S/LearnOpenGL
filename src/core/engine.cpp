#include "core/engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Engine::Engine(){
    setupGLFW();
    createWindow();

    //create camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 20.0f, 0.1f);

    //create input manager
    inputManager = InputManager();

    createShaders();
    createObjects();
}

void Engine::createShaders(){
    objectShader = Shader("../shaders/objectShader.vs", "../shaders/objectShader.fs");
    shaders.push_back(objectShader);

    lightShader = Shader("../shaders/simpleShader.vs", "../shaders/simpleShader.fs");
    shaders.push_back(lightShader);

    transparentShader = Shader("../shaders/simpleTextureShader.vs", "../shaders/simpleTextureShader.fs");
    shaders.push_back(transparentShader);
}

void Engine::createObjects(){
    //create lights
    light1 = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), {0.0f, 0.05f, 0.1f}, glm::vec3(0.0f, -1.0f, 0.0f));
    lights.push_back(light1);
    light2 = PointLight(&lightShader, "../res/car1/Pony_cartoon.obj", glm::vec3(1.0f, 5.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), {0.0f, 0.5f, 1.0f}, {1.0f, 0.022f, 0.0019f});
    lights.push_back(light2);
    light3 = SpotLight(5.0f, 10.0f, glm::vec3(1.0f, 1.0f, 1.0f), {0.0f, 1.0f, 1.0f});
    lights.push_back(light3);

    //create models
    car1 = Model("../res/car1/Pony_cartoon.obj");
    car2 = Model("../res/car2/lambo.obj");

    //create simplemeshes
    grass = SimpleMesh(data3, 30, "../res/window.png");
}

void Engine::render(){
    while (!glfwWindowShouldClose(window)) {
        //global time updates
        time = glfwGetTime();
        deltaTime = time-lastTime;
        lastTime = time;

        //input
        inputManager.processInput(window);

        //clear buffers
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        //set view and proj matrices
        viewMatrix = camera.getView();
        projectionMatrix = glm::perspective(glm::radians(camera.getFov()), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, camera.getNearPlane(), camera.getFarPlane());
        setSpaces();

        //set uniforms
        setUniforms();

        //render objects

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -10.0f)); 
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	
        objectShader.setMatrix4f("model", model);

        car1.draw(objectShader);

        //car army
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 10; j++){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(5.0f * i, 0.0f, 10.0f * j));
                model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f)); 
                objectShader.setMatrix4f("model", model);

                car2.drawOutlined(objectShader, lightShader, glm::vec3(0.1f, 0.5f, 0.5f), 0.001f, glm::vec3(5.0f * i, 0.0f, 10.0f * j));
            }
        }

        light2.setPosition(sin(glm::radians(time*50.0f)) * 40.0f + 40.0f, 5.0f, cos(glm::radians(time*50.0f)) * 100.0f + 100.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light2.getPosition());
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f)); 
        lightShader.setMatrix4f("model", model);

        light2.draw();

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-20.0f - 5*i, 0.0f, -20.0f - 5*j)); 
                transparentShader.setMatrix4f("model", model);

                grass.drawTransparent(transparentShader);
            }
        }

        //callbacks & buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void Engine::setUniforms(){
    for(Shader s : shaders){
        //skip shaders
        if(s.getID() == lightShader.getID()){
            continue;
        }

        //camera
        s.set3f("camPos", camera.getPosition());
        s.set3f("cameraPointDirection", camera.getDir());

        //lights
        s.set3f("dirLight.direction", light1.getDirection());
        s.set3f("dirLight.color", light1.getColor());
        s.setFloat("dirLight.ambient", light1.getIntensity().ambient);
        s.setFloat("dirLight.diffuse", light1.getIntensity().diffuse);
        s.setFloat("dirLight.specular", light1.getIntensity().specular);
        
        s.set3f("pointLights[0].position", light2.getPosition());
        s.set3f("pointLights[0].color", light2.getColor());
        s.setFloat("pointLights[0].constant", light2.getAtt().constant);
        s.setFloat("pointLights[0].linear", light2.getAtt().linear);
        s.setFloat("pointLights[0].quadratic", light2.getAtt().quadratic);
        s.setFloat("pointLights[0].ambient", light2.getIntensity().ambient);
        s.setFloat("pointLights[0].diffuse", light2.getIntensity().diffuse);
        s.setFloat("pointLights[0].specular", light2.getIntensity().specular);
        
        if(light3.isOn()){
            s.set3f("spotLight.color", light3.getColor());
            s.setFloat("spotLight.cosInnerCutoff", light3.getInnerCosCutoff());
            s.setFloat("spotLight.cosOuterCutoff", light3.getOuterCosCutoff());
            s.setFloat("spotLight.ambient", light3.getIntensity().ambient);
            s.setFloat("spotLight.diffuse", light3.getIntensity().diffuse);
            s.setFloat("spotLight.specular", light3.getIntensity().specular);
        } else {
            s.set3f("spotLight.color", light3.getColor());
            s.setFloat("spotLight.cosInnerCutoff", light3.getInnerCosCutoff());
            s.setFloat("spotLight.cosOuterCutoff", light3.getOuterCosCutoff());
            s.setFloat("spotLight.ambient", 0.0f);
            s.setFloat("spotLight.diffuse", 0.0f);
            s.setFloat("spotLight.specular", 0.0f);
        }
    }
}

void Engine::setSpaces(){
    for(Shader s : shaders){
        s.use();
        s.setMatrix4f("view", viewMatrix);
        s.setMatrix4f("proj", projectionMatrix);
    }
}

void Engine::setupGLFW(){
    // initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Engine::createWindow(){
    // make window
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to make GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    // initialize GLAD (after window)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD" << std::endl;
    }

    // one time calls
    // set viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    //set callback functions
    glfwSetFramebufferSizeCallback(window, inputManager.framebuffer_size_callback);
    glfwSetCursorPosCallback(window, inputManager.mouse_callback);
    glfwSetScrollCallback(window, inputManager.scroll_callback);
    glfwSetKeyCallback(window, inputManager.key_callback);
    
    //enable buffers
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    //enable vsync
    glfwSwapInterval(1);

    //capture cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

int Engine::getWindowWidth(){
    return WINDOW_WIDTH;
}

int Engine::getWindowHeight(){
    return WINDOW_HEIGHT;
}

GLFWwindow* Engine::getWindow(){
    return window;
}

Camera* Engine::getCamera(){
    return &camera;
}

float Engine::getDeltaTime(){
    return deltaTime;
}

std::vector<Light> Engine::getLights(){
    return lights;
}

void Engine::spotLightSwitch(){
    light3.flipLight();
}