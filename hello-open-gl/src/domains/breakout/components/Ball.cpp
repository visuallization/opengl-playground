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
		if (!IsStuck) {
			UpdatePosition(Velocity * deltaTime);

			// check bounds
			if (GetPosition().x <= 0.0f) {
				Velocity.x *= -1;
				SetPosition(glm::vec2(0, GetPosition().y));
			}
			else if (GetPosition().x + Size.x >= windowWidth) {
				Velocity.x *= -1;
				SetPosition(glm::vec2(windowWidth - Size.x, GetPosition().y));
			}

			if (GetPosition().y <= 0.0f) {
				Velocity.y *= -1;
				SetPosition(glm::vec2(GetPosition().x, 0));
			}
			else if (GetPosition().y + Size.y >= windowHeight) {
				Velocity.y *= -1;
				SetPosition(glm::vec2(GetPosition().x, windowHeight - Size.y));
			}
		}

		return GetPosition();
	}

	void Ball::Reset(glm::vec2 position, glm::vec2 velocity) {
		SetPosition(position);
		Velocity = velocity;
		IsStuck = true;
	}

	CollisionShapeCircle* Ball::GetCollider() {
		return static_cast<CollisionShapeCircle*>(m_Collider);
	}
}