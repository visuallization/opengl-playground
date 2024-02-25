#include "GameObject.h"

#include "./CollisionShapeRectangle.h"

namespace breakout {

	GameObject::GameObject()
		: m_Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsActive(true)
	{
		m_Collider = new CollisionShapeRectangle(m_Position, Size);
	}

	GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture* sprite, float rotation, glm::vec3 color, glm::vec2 velocity) 
		: m_Position(position), Size(size), Velocity(velocity), Color(color), Rotation(rotation), Sprite(sprite), IsActive(true)
	{
		m_Collider = new CollisionShapeRectangle(m_Position, Size);
	}

	void GameObject::Draw(SpriteRenderer& renderer, bool debug) {
		renderer.DrawSprite(Sprite, m_Position, Size, Rotation, Color);

		if (debug) {
			m_Collider->Draw(renderer);
		}
	}

	void GameObject::Destroy() {
		IsActive = false;
	}

	glm::vec2 GameObject::GetPosition() {
		return m_Position;
	}

	void GameObject::SetPosition(glm::vec2 position) {
		m_Position = position;
		m_Collider->Position = m_Position;
	}

	void GameObject::UpdatePosition(glm::vec2 delta) {
		m_Position += delta;
		m_Collider->Position = m_Position;
	}

	CollisionShapeRectangle* GameObject::GetCollider() {
		return static_cast<CollisionShapeRectangle*>(m_Collider);
	}

}