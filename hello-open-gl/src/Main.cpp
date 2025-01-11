#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Renderer.h"

#include "scenes/SceneBreakOut.h"
#include "scenes/SceneClearColor.h";
#include "scenes/SceneComputeDemo.h"
#include "scenes/SceneComputeDemoOptimized.h";
#include "scenes/SceneCube.h";
#include "scenes/SceneForestFire.h"
#include "scenes/SceneFrameBuffer.h"
#include "scenes/SceneGeometryDemo.h";
#include "scenes/SceneImGuiDemo.h"
#include "scenes/SceneModel.h";
#include "scenes/ScenePoint.h";
#include "scenes/SceneSolarSystem.h";
#include "scenes/SceneTexture2D.h";

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Utilize dedicated Nvidia card
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

void HandleKeyboardInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Set the OpenGL version explicitly
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Playground", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // synchronize with refresh rate
    // uncapped fps = 0, capped fps (60 fps) = 1
    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // scope everything so it everything gets cleaned properly before running glfwTerminate
    {        
        Renderer renderer;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        scene::Scene* currentScene = nullptr;
        scene::SceneMenu* menu = new scene::SceneMenu(currentScene);
        currentScene = menu;

        menu->RegisterScene<scene::SceneBreakOut>("Break Out", window);
        menu->RegisterScene<scene::SceneFrameBuffer>("FrameBuffer", window);
        menu->RegisterScene<scene::SceneClearColor>("Clear Color", window);
        menu->RegisterScene<scene::SceneTexture2D>("2D Texture",  window);
        menu->RegisterScene<scene::SceneCube>("3D Cube", window);
        menu->RegisterScene<scene::SceneImGuiDemo>("ImGui Demo", window);
        menu->RegisterScene<scene::SceneModel>("Model", window);
        menu->RegisterScene<scene::ScenePoint>("Point Rendering", window);
        menu->RegisterScene<scene::SceneGeometryDemo>("Geometry Demo", window);
        menu->RegisterScene<scene::SceneComputeDemo>("Compute Demo", window);
        menu->RegisterScene<scene::SceneComputeDemoOptimized>("Compute Demo Optimized", window);
        menu->RegisterScene<scene::SceneForestFire>("Forest Fire", window);
        menu->RegisterScene<scene::SceneSolarSystem>("Solar System", window);

        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            renderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            //renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentScene) {
                currentScene->OnUpdate(deltaTime);
                currentScene->OnRender();

                ImGui::Begin("Settings");
                if (currentScene != menu && ImGui::Button("<")) {
                    delete currentScene;
                    currentScene = menu;
                }
                currentScene->OnImGuiRender();
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Handle keyboard input
            HandleKeyboardInput(window);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        if (currentScene != menu) {
            delete menu;
        }
        delete currentScene;

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}