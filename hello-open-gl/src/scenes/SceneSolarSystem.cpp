#include "SceneSolarSystem.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Model.h"

namespace scene {
	const float GRAVITY = 100.f;

	SceneSolarSystem::SceneSolarSystem(GLFWwindow*& window) :
		Scene(window),
		m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f)),
		m_TimeMultiplier(1.f)
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Shader = std::make_shared<Shader>("res/shaders/Model.shader");
		m_3DModel = std::make_unique<Model>("res/models/world/world.obj");

		float positionScale = 1.f;
		float sizeScale = 4.f;
		float massScale = 0.0001f;

		// Sun
		m_Sun = {
			glm::vec3(0, 0, 0),
			1.f,
			333000.f * massScale,
			glm::vec4(1, 1, 0.6, 1)
		};

		// Earth
		Planet earth = {
			glm::vec3(1.5f * positionScale, 0, 0),
			0.01f * sizeScale,
			1.f * massScale,
			glm::vec4(0, 0.6, 1, 1)
		};
		earth.velocity += (glm::vec3(0, 1, 0) * 0.01f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(earth.position, m_Sun.position));
		m_Planets.push_back(earth);

		// Mercury
		Planet mercury = { 
			glm::vec3(0.6f + 0.4f * positionScale, 0, 0),
			0.0023f * sizeScale,
			0.05f * massScale,
			glm::vec4(1, 0.6, 0.3, 1)
		};
		mercury.velocity += (glm::vec3(0, 1, 0) * 0.0023f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(mercury.position, m_Sun.position));
		m_Planets.push_back(mercury);

		// Venus
		Planet venus = { 
			glm::vec3(1.1f * positionScale, 0, 0), 
			0.009f * sizeScale,
			0.8f * massScale, 
			glm::vec4(0.9, 0.6, 0.2, 1)
		};
		venus.velocity += (glm::vec3(0, 1, 0) * 0.009f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(venus.position, m_Sun.position));
		m_Planets.push_back(venus);

		// Mars
		Planet mars = {
			glm::vec3(2.5f * positionScale, 0, 0),
			0.005f * sizeScale,
			0.1f * massScale,
			glm::vec4(1, 0.6, 0.6, 1)
		};
		mars.velocity += (glm::vec3(0, 1, 0) * 0.0035f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(mars.position, m_Sun.position));
		m_Planets.push_back(mars);

		// Jupiter
		Planet jupiter = {
			glm::vec3(7.5f * positionScale, 0, 0),
			0.1f * sizeScale,
			317.f * massScale,
			glm::vec4(0.9, 0.8, 0.7, 1)
		};
		jupiter.velocity += (glm::vec3(0, 1, 0) * 0.18f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(jupiter.position, m_Sun.position));
		m_Planets.push_back(jupiter);

		// Saturn
		Planet saturn = {
			glm::vec3(14.0f * positionScale, 0, 0),
			0.08f * sizeScale,
			95.f * massScale,
			glm::vec4(0.5, 1, 0.4, 1)
		};
		saturn.velocity += (glm::vec3(0, 1, 0) * 0.1f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(saturn.position, m_Sun.position));
		m_Planets.push_back(saturn);

		// Uranus
		Planet uranus = {
			glm::vec3(29.0f * positionScale, 0, 0),
			0.04f * sizeScale,
			14.54f * massScale,
			glm::vec4(0.5, 0.8, 1, 1)
		};
		uranus.velocity += (glm::vec3(0, 1, 0) * 0.04f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(uranus.position, m_Sun.position));
		m_Planets.push_back(uranus);

		// Neptune
		Planet neptune = {
			glm::vec3(44.0f * positionScale, 0, 0),
			0.04f * sizeScale,
			17.15f * massScale,
			glm::vec4(0.6, 0.3, 0.6, 1)
		};
		neptune.velocity += (glm::vec3(0, 1, 0) * 0.04f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(neptune.position, m_Sun.position));
		m_Planets.push_back(neptune);
	}

	SceneSolarSystem::~SceneSolarSystem() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneSolarSystem::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);

		for (Planet& planet : m_Planets) {
			float r = glm::distance(planet.position, m_Sun.position);
			glm::vec3 force = glm::normalize(m_Sun.position - planet.position) * (GRAVITY * (planet.mass * m_Sun.mass) / (r * r));
			planet.acceleration = force;

			planet.velocity += planet.acceleration * deltaTime * m_TimeMultiplier;
			planet.position += planet.velocity * deltaTime * m_TimeMultiplier;
		}
	}

	void SceneSolarSystem::OnRender() {
		Renderer renderer;

		RenderPlanet(renderer, m_Sun);

		for (Planet& planet : m_Planets) {
			RenderPlanet(renderer, planet);
		}
	}

	void SceneSolarSystem::OnImGuiRender() {
		ImGui::SliderFloat("Time Multiplier", &m_TimeMultiplier, 1, 100);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

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
