#include "GameObject.h"

namespace breakout {

	GameObject::GameObject()
		: Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsDestroyable(true), IsActive(true) {}

	GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture* sprite, float rotation, glm::vec3 color, bool isDestroyable, glm::vec2 velocity) 
		: Position(position), Size(size), Velocity(velocity), Color(color), Rotation(rotation), Sprite(sprite), IsDestroyable(isDestroyable), IsActive(true) {}

	void GameObject::Draw(SpriteRenderer& spriteRenderer) {
		spriteRenderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
	}

}