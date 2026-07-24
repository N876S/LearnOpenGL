#include "core/engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

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
    lightShader = Shader("../shaders/lightShader.vs", "../shaders/lightShader.fs");
    groundShader = Shader("../shaders/groundShader.vs", "../shaders/groundShader.fs");
    shaders.push_back(objectShader);
    shaders.push_back(lightShader);
    shaders.push_back(groundShader);
}

void Engine::createObjects(){
    //create meshes
    cube = Mesh(data, sizeof(data)/sizeof(data[0]), 8);
    platform = Mesh(data2, sizeof(data2)/sizeof(data2[0]), 6);

    //create lights
    light1 = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), {0.5f, 0.7f, 1.0f}, glm::vec3(0.0f, -1.0f, 0.0f));
    light2 = PointLight(&lightShader, cube, glm::vec3(-4.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), {0.2f, 1.0f, 1.0f}, {1.0f, 0.022f, 0.0019f});

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
        light2.render(time);

        //callbacks & buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}


void Engine::setLights(){
    for(Shader s : shaders){
        if(s.ID == lightShader.ID){
            continue;
        }
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
