#pragma once

#include "Scene.h"

namespace scene {
	class SceneImGuiDemo : public Scene {
	public:
		SceneImGuiDemo(GLFWwindow*& window);
		~SceneImGuiDemo();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}


