#pragma once

#define _USE_MATH_DEFINES
#include <math.h>

#include "App.h"

App::App()
{
    m_WindowState = WindowState(1000, 1000, "Skybox");
    lastPosition.x = 1000 / 2.0f;
    lastPosition.y = 1000 / 2.0f;
    newPosition = lastPosition;
    path = "assets/hdr/Beach.hdr";
    //path = "assets/hdr/Snowstreet.hdr";
    //path = "assets/hdr/Test.hdr";
    //path = "assets/hdr/Beach.hdr";
    exposure = 0.18;
    specularOn = 1;
    filePath = path.c_str();
    kd = 1e-5f;
    ks = 1e-5f;
    kt = 1.0;
    km = 0.9;
    mode = 1;
    Init();
    PrepareScene();
}

App::~App()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();

}

void App::Init()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    if (!glfwInit())
    {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_WindowState.width, m_WindowState.height, "Skybox", NULL, NULL);
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            App* app = (App*)glfwGetWindowUserPointer(window);
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                switch (key)
                {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(window, true);
                    break;
                case GLFW_KEY_W:
                    app->bladeHeight *= 1.1;
                    app->m_Object.m_Shader->SetUniform1f("bladeHeight", app->bladeHeight);
                    break;
                case GLFW_KEY_S:
                    if (app->bladeHeight > 0.1)
                    {
                        app->bladeHeight *= (1.0f / 1.1f);
                    }                    
                    app->m_Object.m_Shader->SetUniform1f("bladeHeight", app->bladeHeight);
                    break;
                case GLFW_KEY_E:
                    if (app->color > 0.0)
                    {
                        app->color -= 0.05;
                        app->m_Object.m_Shader->SetUniform1f("color", app->color);
                    }                   
                    break;
                case GLFW_KEY_R:
                    if (app->color < 1.0)
                    {
                        app->color += 0.05;
                        app->m_Object.m_Shader->SetUniform1f("color", app->color);
                    }
                    break;
                case GLFW_KEY_A:
                    if (app->windStrength > 0.0)
                    {
                        app->windStrength -= 0.05;
                        app->m_Object.m_Shader->SetUniform1f("wind", app->windStrength);
                    }
                    break;
                case GLFW_KEY_D:
                    if (app->windStrength < 1.85)
                    {
                        app->windStrength += 0.05;
                        app->m_Object.m_Shader->SetUniform1f("wind", app->windStrength);
                    }
                    break;
                case GLFW_KEY_B:
                    printf("Wind Strength: %f\n", app->windStrength);
                    break;
                default:
                    break;
                }
                
            }
        });


    if (!m_Window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(m_Window);
    // glfwSwapInterval(1);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        exit(-1);
    }

    //char rendererInfo[512] = { 0 };
    //strcpy(rendererInfo, (const char*)glGetString(GL_RENDERER)); // Use strcpy_s on Windows, strcpy on Linux
    //strcat(rendererInfo, " - "); 
    //strcat(rendererInfo, (const char*)glGetString(GL_VERSION)); // Use strcpy_s on Windows, strcpy on Linux
    //glfwSetWindowTitle(m_Window, rendererInfo);
    glfwSetWindowUserPointer(m_Window, this);

    // glfw state
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // GLFW Events
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
            App* app = (App*)glfwGetWindowUserPointer(window);
            app->m_WindowState.width = width;
            app->m_WindowState.height = height;
            // Update Projection matrix on resize
            app->m_Camera->UpdateProjectionMatrix(width, height);
        });

    // Mouse button callback
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xposIn, double yposIn)
        {
            App* app = (App*)glfwGetWindowUserPointer(window);
            app->newPosition = glm::vec2(xposIn, yposIn);
            if (!app->mouseInitialized)
            {
                app->lastPosition = app->newPosition;
                app->mouseInitialized = true;
            }
        });
    initPerlin();
}

void App::initPerlin()
{
    glGenTextures(1, &perlinNoise);
    glBindTexture(GL_TEXTURE_2D, perlinNoise);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/noiseTexture.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}



void App::PrepareScene()
{
    m_Object.Load();

    m_ground.LoadGround();

    m_Camera = std::make_shared<Camera>(m_WindowState.width, m_WindowState.height);
    m_Skybox.SetupSkybox(filePath);
}

void App::Run()
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    double currentFrame = glfwGetTime();
    double lastFrame = currentFrame;
    double deltaTime;


    while (!glfwWindowShouldClose(m_Window))
    {
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;



        glClearColor(0, 0, 0, 1);
        glClearDepth(1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        ProcessInput(deltaTime);


        Render();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void App::Render()
{
    
    glEnable(GL_DEPTH_TEST);


    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    

    float time = glfwGetTime();
    float wind = windStrength;

   
    
    m_Skybox.Render(*m_Camera);
    m_ground.RenderGround(m_Camera);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, perlinNoise);
    m_Object.Render(m_Camera, bladeHeight, color, wind, time, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    glDisable(GL_DEPTH_TEST);
    
}

void App::ProcessInput(float deltaTime)
{
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);

    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        float xoffset = newPosition.x - lastPosition.x;
        float yoffset = newPosition.y - lastPosition.y; //maybe inserve
        m_Camera->ProcessMouseMovement(xoffset, yoffset, deltaTime);
    }
    lastPosition = newPosition;
    m_Camera->processKeyboard(m_Window, deltaTime);

}