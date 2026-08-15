#include "core/engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

/*

TO DO
1. Update input method system - DONE
1.5. Make most variables private and use getters setters - DONE
1.75. Fix materials class and stuff - DONE
2. Abstract camera uniforms - DONE
3. Fix the light class to have a model for point lights - DONE
4. Fix up shaders to have a single colour shader unaffected by light - just uniform the colour
5. Add stencil testing for outlining objects
6. Make an army of cars and optimize

*/

Engine::Engine(){
    setupGLFW();
    createWindow();

    //create camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 20.0f, 0.1f);

    //create input manager
    inputManager = InputManager(camera);

    createShaders();
    createObjects();
}

void Engine::createShaders(){
    objectShader = Shader("../shaders/newShader.vs", "../shaders/newShader.fs");
    shaders.push_back(objectShader);

    lightShader = Shader("../shaders/lightShader.vs", "../shaders/lightShader.fs");
    shaders.push_back(lightShader);
}

void Engine::createObjects(){
    //create lights
    light1 = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), {0.0f, 0.05f, 0.1f}, glm::vec3(0.0f, -1.0f, 0.0f));
    light2 = PointLight(&lightShader, "../res/car1/Pony_cartoon.obj", glm::vec3(1.0f, 5.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), {0.0f, 0.5f, 1.0f}, {1.0f, 0.022f, 0.0019f});
    light3 = SpotLight(0.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f), {0.0f, 1.0f, 1.0f});

    //create models
    car1 = Model("../res/car1/Pony_cartoon.obj");
    car2 = Model("../res/car2/lambo.obj");
}

void Engine::render(){
    while (!glfwWindowShouldClose(window)) {
        //global time updates
        time = glfwGetTime();
        deltaTime = time-lastTime;
        lastTime = time;

        //input
        inputManager.updateInputData(camera, deltaTime);
        inputManager.processInput(window);

        //clear buffers
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //set view and proj matrices
        viewMatrix = camera.getView();
        projectionMatrix = glm::perspective(glm::radians(camera.getFov()), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, camera.getNearPlane(), camera.getFarPlane());
        setSpaces();

        //set uniforms
        setUniforms();

        //render objects

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	
        objectShader.setMatrix4f("model", model);

        car1.draw(objectShader);

        //car army
        for(int i = 0; i < 20; i++){
            for(int j = 0; j < 20; j++){
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(5.0f * i, 0.0f, 10.0f * j));
                model = glm::rotate(model, glm::radians(time*100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f)); 
                objectShader.setMatrix4f("model", model);

                car2.draw(objectShader);
            }
        }

        light2.setPosition(sin(glm::radians(time*100.0f)) * 40.0f + 40.0f, 5.0f, cos(glm::radians(time*100.0f)) * 100.0f + 100.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light2.getPosition());
        model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f)); 
        lightShader.setMatrix4f("model", model);

        light2.draw();

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
        
        s.set3f("spotLight.color", light3.getColor());
        s.setFloat("spotLight.cosInnerCutoff", light3.getInnerCosCutoff());
        s.setFloat("spotLight.cosOuterCutoff", light3.getOuterCosCutoff());
        s.setFloat("spotLight.ambient", light3.getIntensity().ambient);
        s.setFloat("spotLight.diffuse", light3.getIntensity().diffuse);
        s.setFloat("spotLight.specular", light3.getIntensity().specular);
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
    
    //enable buffers
    glEnable(GL_DEPTH_TEST);

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