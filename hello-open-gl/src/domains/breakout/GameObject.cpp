#include "GameObject.h"

namespace breakout {

	GameObject::GameObject()
		: Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsActive(true) {}

	GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture* sprite, float rotation, glm::vec3 color, glm::vec2 velocity) 
		: Position(position), Size(size), Velocity(velocity), Color(color), Rotation(rotation), Sprite(sprite), IsActive(true) {}

	void GameObject::Draw(SpriteRenderer& spriteRenderer) {
		spriteRenderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	}

	// AABB collision check
	bool GameObject::IsColliding(GameObject& other) {

		bool collisonX = this->Position.x + this->Size.x >= other.Position.x && other.Position.x + other.Size.x >= this->Position.x;
		if (!collisonX) {
			return false;
		}

		bool collisionY = this->Position.y + this->Size.y >= other.Position.y && other.Position.y + other.Size.y >= this->Position.y;
		if (!collisionY) {
			return false;
		}

		return true;
	}

	void GameObject::Destroy() {
		this->IsActive = false;
	}

}