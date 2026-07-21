#include "core/engine.h"

Engine::Engine(){
    setupGLFW();
    createWindow();

    //create input manager
    inputManager = InputManager();

    //create camera
    camera = Camera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), 5.0f, 0.1f);

    createShaders();
    createObjects();

    glfwSetWindowUserPointer(window, &inputPointers);
}

void Engine::createShaders(){
    objectShader = Shader("../shaders/shader.vs", "../shaders/shader.fs");
    //lightShader = Shader("../shaders/lightShader.vs", "../shaders/lightShader.fs");
    groundShader = Shader("../shaders/groundShader.vs", "../shaders/groundShader.fs");
    shaders.push_back(objectShader);
    //shaders.push_back(lightShader);
    shaders.push_back(groundShader);
}

void Engine::createObjects(){
    //create meshes
    cube = Mesh(data, sizeof(data)/sizeof(data[0]), 8);
    platform = Mesh(data2, sizeof(data2)/sizeof(data2[0]), 6);

    //create lights
    light = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), {0.5f, 0.8f, 1.0f}, glm::vec3(0.0f, -1.0f, 0.0f));

    //create models
    crate = Model(&objectShader, cube, "../res/container.png", "../res/specularmap.png", MaterialType::Brass);
    ground = Model(&groundShader, platform, glm::vec3(0.2f, 0.2f, 0.2f), MaterialType::Obsidian);
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
        glClearColor(0.1f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //set view and proj matrices
        viewMatrix = camera.getView();
        projectionMatrix = glm::perspective(glm::radians(fov), (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, 0.1f, 100.0f);
        setSpaces();
        glfwSetWindowUserPointer(window, &inputPointers);

        objectShader.set3f("camPos", camera.getPosition());
        groundShader.set3f("camPos", camera.getPosition());
        setLights();

        //render objects
        crate.render(positions, time);
        ground.render(glm::vec3(0.0f, -5.0f, 0.0f), time);

        //callbacks & buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}


void Engine::setLights(){
    objectShader.set3f("dirLight.direction", ((DirectionalLight)light).getDirection());
    objectShader.set3f("dirLight.color", light.getColor());
    objectShader.setFloat("dirLight.ambient", light.getIntensity().ambient);
    objectShader.setFloat("dirLight.diffuse", light.getIntensity().diffuse);
    objectShader.setFloat("dirLight.specular", light.getIntensity().specular);
    /*
    groundShader.set3f("light.pos", light.getPosition());
    groundShader.set3f("light.color", light.getColor());
    groundShader.setFloat("light.ambient", light.getIntensity().ambient);
    groundShader.setFloat("light.diffuse", light.getIntensity().diffuse);
    groundShader.setFloat("light.specular", light.getIntensity().specular);
    */
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