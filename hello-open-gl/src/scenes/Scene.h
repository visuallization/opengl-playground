#pragma once

#include <vector>
#include <string>
#include <functional>
#include <iostream>

namespace scene {

	class Scene {
	public:
		Scene() {}
		virtual ~Scene() {}
		
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class SceneMenu : public Scene {
	public:
		SceneMenu(Scene*& currentScene);

		void OnImGuiRender() override;

		template <typename T>
		void RegisterScene(const std::string& name) {
			std::cout << "Regsitering Scene: " << name << std::endl;
			m_Scenes.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Scene*& m_CurrentScene;
		std::vector<std::pair<std::string, std::function<Scene*()>>> m_Scenes;
	};
}