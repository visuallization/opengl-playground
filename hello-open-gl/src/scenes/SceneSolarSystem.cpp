#include "SceneSolarSystem.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Model.h"

namespace scene {
	SceneSolarSystem::SceneSolarSystem(GLFWwindow*& window) :
		Scene(window),
		m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f))
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Shader = std::make_shared<Shader>("res/shaders/Model.shader");
		m_3DModel = std::make_unique<Model>("res/models/world/world.obj");
	}

	SceneSolarSystem::~SceneSolarSystem() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneSolarSystem::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);
	}

	void SceneSolarSystem::OnRender() {
		Renderer renderer;
		float positionScale = 1.1f;
		float sizeScale = 2.f;
		float weightScale = 1.f;

		// Sun
		Planet sun = { 
			glm::vec3(0, 0, 0),
			1.f,
			333000.f * weightScale,
			glm::vec4(1, 1, 0.6, 1)
		};
		RenderPlanet(renderer, sun);

		// Mercury
		Planet mercury = { 
			glm::vec3(0.6f + 0.4f * positionScale, 0, 0),
			0.003f * sizeScale,
			0.05f * weightScale,
			glm::vec4(1, 0.6, 0.3, 1)
		};
		RenderPlanet(renderer, mercury);

		// Venus
		Planet venus = { 
			glm::vec3(1.1f * positionScale, 0, 0), 
			0.01f * sizeScale,
			0.8f * weightScale, 
			glm::vec4(0.9, 0.6, 0.2, 1)
		};
		RenderPlanet(renderer, venus);

		// Earth
		Planet earth = {
			glm::vec3(1.5f * positionScale, 0, 0),
			0.01f * sizeScale,
			1.f * weightScale,
			glm::vec4(0, 0.6, 1, 1)
		};
		RenderPlanet(renderer, earth);

		// Mars
		Planet mars = {
			glm::vec3(2.5f * positionScale, 0, 0),
			0.005f * sizeScale,
			0.1f * weightScale,
			glm::vec4(1, 0.6, 0.6, 1)
		};
		RenderPlanet(renderer, mars);

		// Jupiter
		Planet jupiter = {
			glm::vec3(7.5f * positionScale, 0, 0),
			0.1f * sizeScale,
			317.f * weightScale,
			glm::vec4(0.9, 0.8, 0.7, 1)
		};
		RenderPlanet(renderer, jupiter);

		// Saturn
		Planet saturn = {
			glm::vec3(14.0f * positionScale, 0, 0),
			0.08f * sizeScale,
			95.f * weightScale,
			glm::vec4(0.5, 1, 0.4, 1)
		};
		RenderPlanet(renderer, saturn);
		
		// Uranus
		Planet uranus = {
			glm::vec3(29.0f * positionScale, 0, 0),
			0.04f * sizeScale,
			14.f * weightScale,
			glm::vec4(0.5, 0.8, 1, 1)
		};
		RenderPlanet(renderer, uranus);

		// Neptune
		Planet neptune = {
			glm::vec3(44.0f * positionScale, 0, 0),
			0.04f * sizeScale,
			17.f * weightScale,
			glm::vec4(0.6, 0.3, 0.6, 1)
		};
		RenderPlanet(renderer, neptune);
	}

	void SceneSolarSystem::OnImGuiRender() {}

	void SceneSolarSystem::RenderPlanet(const Renderer& renderer, const Planet& planet)
	{
		// camera
		m_View = m_Camera.GetViewMatrix();
		// model
		m_Model = glm::mat4(1.0f);
		m_Model = glm::translate(m_Model, planet.position);
		m_Model = glm::scale(m_Model, glm::vec3(planet.scale, planet.scale, planet.scale));
		// projection
		m_Projection = glm::perspective(glm::radians(m_Camera.FieldOfView), (float)m_Width / (float)m_Height, 0.1f, 100.f);

		for (int i = 0; i < m_3DModel->Meshes.size(); i++) {
			m_Shader->Bind();

			Mesh& mesh = m_3DModel->Meshes[i];

			if (mesh.Textures.size() > 0) {
				m_Shader->SetUniform1i("u_Texture", 0);
				mesh.Textures[0].Texture->Bind();
			}
			m_Shader->SetUniformVec4f("u_Color", planet.color);

			m_Shader->SetUniformMat4f("u_Model", m_Model);
			m_Shader->SetUniformMat4f("u_View", m_View);
			m_Shader->SetUniformMat4f("u_Projection", m_Projection);

			renderer.Draw(*mesh.VAO, *mesh.IBO, *m_Shader);

			m_Shader->Unbind();
		}
	}

}
