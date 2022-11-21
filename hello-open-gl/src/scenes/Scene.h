#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace scene {

	class Scene {
	public:
		Scene() {}
		Scene(GLFWwindow*& window): m_Window(window) {
			glfwGetWindowSize(m_Window, &m_Width, &m_Height);
		}
		virtual ~Scene() {}
		
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

	protected:
		GLFWwindow* m_Window;
		int m_Width;
		int m_Height;
	};

	class SceneMenu : public Scene {
	public:
		SceneMenu(Scene*& currentScene);

		void OnImGuiRender() override;

		template <typename T>
		void RegisterScene(const std::string& name, GLFWwindow*& window) {
			std::cout << "Registering Scene: " << name << std::endl;
			m_Scenes.push_back(std::make_pair(name, [&]() { return new T(window); }));
		}

	private:
		Scene*& m_CurrentScene;
		std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
	};
}