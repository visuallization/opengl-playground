#include "SceneBreakOut.h"

#include "imgui/imgui.h"
#include "Renderer.h"

namespace scene {
	SceneBreakOut::SceneBreakOut(GLFWwindow*& window) : Scene::Scene(window)
	{

	}

	SceneBreakOut::~SceneBreakOut()
	{
	
	}

	void SceneBreakOut::OnUpdate(float deltaTime)
	{

	}

	void SceneBreakOut::OnRender()
	{
		GLCall(glClearColor(1, 0, 0, 1));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void SceneBreakOut::OnImGuiRender()
	{

	}
}