#include "Ball.h"

namespace breakout {
	Ball::Ball()
		: GameObject(), Radius(12.f), IsStuck(true) {
		m_Collider = new CollisionShapeCircle(GetPosition(), Size, Radius);
	}

	Ball::Ball(glm::vec2 position, float radius, glm::vec2 velocity, Texture* sprite)
		: GameObject(position, glm::vec2(radius * 2, radius * 2), sprite, 0, glm::vec3(1), velocity), Radius(radius), IsStuck(true) {
		m_Collider = new CollisionShapeCircle(GetPosition(), Size, Radius);
	}

	glm::vec2 Ball::Move(float deltaTime, unsigned int windowWidth, unsigned int windowHeight) {
		if (!this->IsStuck) {
			UpdatePosition(this->Velocity * deltaTime);

			// check bounds
			if (this->GetPosition().x <= 0.0f) {
				this->Velocity.x *= -1;
				SetPosition(glm::vec2(0, GetPosition().y));
			}
			else if (this->GetPosition().x + this->Size.x >= windowWidth) {
				this->Velocity.x *= -1;
				SetPosition(glm::vec2(windowWidth - this->Size.x, GetPosition().y));
			}

			if (this->GetPosition().y <= 0.0f) {
				this->Velocity.y *= -1;
				SetPosition(glm::vec2(GetPosition().x, 0));
			}
			else if (this->GetPosition().y + this->Size.y >= windowHeight) {
				this->Velocity.y *= -1;
				SetPosition(glm::vec2(GetPosition().x, windowHeight - this->Size.y));
			}
		}

		return GetPosition();
	}

	void Ball::Reset(glm::vec2 position, glm::vec2 velocity) {
		SetPosition(position);
		this->Velocity = velocity;
		this->IsStuck = true;
	}

	CollisionShapeCircle* Ball::GetCollider() {
		return static_cast<CollisionShapeCircle*>(m_Collider);
	}
}