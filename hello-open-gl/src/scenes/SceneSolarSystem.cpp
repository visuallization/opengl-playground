#include "SceneSolarSystem.h"

#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"

#include "Model.h"

namespace scene {
	const float GRAVITY = 100;

	SceneSolarSystem::SceneSolarSystem(GLFWwindow*& window) :
		Scene(window),
		m_Model(glm::mat4(1.0f)), m_View(glm::mat4(1.0f)), m_Projection(glm::mat4(1.0f)),
		m_Camera(window, glm::vec3(0.0f, 0.0f, 10.0f))
	{
		// Enable depth testing
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Shader = std::make_shared<Shader>("res/shaders/Model.shader");
		m_3DModel = std::make_unique<Model>("res/models/world/world.obj");

		float positionScale = 1.f;
		float sizeScale = 2.f;
		float massScale = 0.0001f;

		// Sun
		m_Sun = {
			glm::vec3(0, 0, 0),
			1.f,
			333000.f * massScale,
			glm::vec4(1, 1, 0.6, 1)
		};

		// Earth
		m_Earth = {
			glm::vec3(1.5f * positionScale, 0, 0),
			0.01f * sizeScale,
			1.f * massScale,
			glm::vec4(0, 0.6, 1, 1)
		};
		m_Earth.velocity += (glm::vec3(0, 1, 0) * 0.01f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(m_Earth.position, m_Sun.position));

		// Mercury
		m_Mercury = { 
			glm::vec3(0.6f + 0.4f * positionScale, 0, 0),
			0.0023f * sizeScale,
			0.05f * massScale,
			glm::vec4(1, 0.6, 0.3, 1)
		};
		m_Mercury.velocity += (glm::vec3(0, 1, 0) * 0.0023f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(m_Mercury.position, m_Sun.position));

		// Venus
		m_Venus = { 
			glm::vec3(1.1f * positionScale, 0, 0), 
			0.009f * sizeScale,
			0.8f * massScale, 
			glm::vec4(0.9, 0.6, 0.2, 1)
		};
		m_Venus.velocity += (glm::vec3(0, 1, 0) * 0.009f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(m_Venus.position, m_Sun.position));

		// Mars
		m_Mars = {
			glm::vec3(2.5f * positionScale, 0, 0),
			0.005f * sizeScale,
			0.1f * massScale,
			glm::vec4(1, 0.6, 0.6, 1)
		};
		m_Mars.velocity += (glm::vec3(0, 1, 0) * 0.0035f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(m_Mars.position, m_Sun.position));

		// Jupiter
		m_Jupiter = {
			glm::vec3(7.5f * positionScale, 0, 0),
			0.1f * sizeScale,
			317.f * massScale,
			glm::vec4(0.9, 0.8, 0.7, 1)
		};
		m_Jupiter.velocity += (glm::vec3(0, 1, 0) * 0.18f) * glm::sqrt((GRAVITY * m_Sun.mass) / glm::distance(m_Jupiter.position, m_Sun.position));
	}

	SceneSolarSystem::~SceneSolarSystem() {
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void SceneSolarSystem::OnUpdate(float deltaTime) {
		m_Camera.OnUpdate(deltaTime);

		{
			float r = glm::distance(m_Earth.position, m_Sun.position);
			glm::vec3 force = glm::normalize(m_Sun.position - m_Earth.position) * (GRAVITY * (m_Earth.mass * m_Sun.mass) / (r * r));
			m_Earth.acceleration = force;

			m_Earth.velocity += m_Earth.acceleration * deltaTime;
			m_Earth.position += m_Earth.velocity * deltaTime;
		}

		{
			float r = glm::distance(m_Mercury.position, m_Sun.position);
			glm::vec3 force = glm::normalize(m_Sun.position - m_Mercury.position) * (GRAVITY * (m_Mercury.mass * m_Sun.mass) / (r * r));
			m_Mercury.acceleration = force;

			m_Mercury.velocity += m_Mercury.acceleration * deltaTime;
			m_Mercury.position += m_Mercury.velocity * deltaTime;
		}

		{
			float r = glm::distance(m_Venus.position, m_Sun.position);
			glm::vec3 force = glm::normalize(m_Sun.position - m_Venus.position) * (GRAVITY * (m_Venus.mass * m_Sun.mass) / (r * r));
			m_Venus.acceleration = force;

			m_Venus.velocity += m_Venus.acceleration * deltaTime;
			m_Venus.position += m_Venus.velocity * deltaTime;
		}

		{
			float r = glm::distance(m_Mars.position, m_Sun.position);
			glm::vec3 force = glm::normalize(m_Sun.position - m_Mars.position) * (GRAVITY * (m_Mars.mass * m_Sun.mass) / (r * r));
			m_Mars.acceleration = force;

			m_Mars.velocity += m_Mars.acceleration * deltaTime;
			m_Mars.position += m_Mars.velocity * deltaTime;
		}

		{
			float r = glm::distance(m_Jupiter.position, m_Sun.position);
			glm::vec3 force = glm::normalize(m_Sun.position - m_Jupiter.position) * (GRAVITY * (m_Jupiter.mass * m_Sun.mass) / (r * r));
			m_Jupiter.acceleration = force;

			m_Jupiter.velocity += m_Jupiter.acceleration * deltaTime;
			m_Jupiter.position += m_Jupiter.velocity * deltaTime;
		}
	}

	void SceneSolarSystem::OnRender() {
		Renderer renderer;

		RenderPlanet(renderer, m_Sun);
		RenderPlanet(renderer, m_Earth);
		RenderPlanet(renderer, m_Mercury);
		RenderPlanet(renderer, m_Venus);
		RenderPlanet(renderer, m_Mars);
		RenderPlanet(renderer, m_Jupiter);

		//// Saturn
		//Planet saturn = {
		//	glm::vec3(14.0f * positionScale, 0, 0),
		//	0.08f * sizeScale,
		//	95.f * weightScale,
		//	glm::vec4(0.5, 1, 0.4, 1)
		//};
		//RenderPlanet(renderer, saturn);
		//
		//// Uranus
		//Planet uranus = {
		//	glm::vec3(29.0f * positionScale, 0, 0),
		//	0.04f * sizeScale,
		//	14.f * weightScale,
		//	glm::vec4(0.5, 0.8, 1, 1)
		//};
		//RenderPlanet(renderer, uranus);

		//// Neptune
		//Planet neptune = {
		//	glm::vec3(44.0f * positionScale, 0, 0),
		//	0.04f * sizeScale,
		//	17.f * weightScale,
		//	glm::vec4(0.6, 0.3, 0.6, 1)
		//};
		//RenderPlanet(renderer, neptune);
	}

	void SceneSolarSystem::OnImGuiRender() {
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
