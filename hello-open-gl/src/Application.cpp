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

#include "scenes/SceneClearColor.h";
#include "scenes/SceneCube.h";
#include "scenes/ScenePoint.h";
#include "scenes/SceneTexture2D.h";

const unsigned int WINDOW_WIDTH = 960;
const unsigned int WINDOW_HEIGHT = 540;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // Set the OpenGL version explicitly
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // synchronize with refresh rate
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    // scope everything so it everythings gets cleaned properly before running glfwTerminate
    {        
        Renderer renderer;

        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();

        scene::Scene* currentScene = nullptr;
        scene::SceneMenu* menu = new scene::SceneMenu(currentScene);
        currentScene = menu;

        menu->RegisterScene<scene::SceneClearColor>("Clear Color");
        menu->RegisterScene<scene::SceneTexture2D>("2D Texture");
        menu->RegisterScene<scene::SceneCube>("3D Cube");
        menu->RegisterScene<scene::ScenePoint>("Point Rendering");

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentScene) {
                currentScene->OnUpdate(0);
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