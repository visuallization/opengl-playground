#include "SceneClearColor.h"

#include "imgui/imgui.h"
#include "Renderer.h"

namespace scene {
	SceneClearColor::SceneClearColor() : m_ClearColor{ 0.2f, 0.3f, 0.8f, 1.0f } {
	
	}

	SceneClearColor::~SceneClearColor() {

	}

	void SceneClearColor::OnUpdate(float deltaTime) {

	}

	void SceneClearColor::OnRender() {
		GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void SceneClearColor::OnImGuiRender() {
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}

}