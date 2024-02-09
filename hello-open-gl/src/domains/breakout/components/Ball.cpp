#include "Ball.h"

namespace breakout {
	Ball::Ball()
		: GameObject(), Radius(12.f), IsStuck(true) {}

	Ball::Ball(glm::vec2 position, float radius, glm::vec2 velocity, Texture* sprite)
		: GameObject(position, glm::vec2(radius * 2, radius * 2), sprite, 0, glm::vec3(1), velocity), Radius(radius), IsStuck(true) {}

	glm::vec2 Ball::Move(float deltaTime, unsigned int windowWidth, unsigned int windowHeight) {
		if (!this->IsStuck) {
			this->Position += this->Velocity * deltaTime;

			// check bounds
			if (this->Position.x <= 0.0f) {
				this->Velocity.x *= -1;
				this->Position.x = 0.0f;
			}
			else if (this->Position.x + this->Size.x >= windowWidth) {
				this->Velocity.x *= -1;
				this->Position.x = windowWidth - this->Size.x;
			}

			if (this->Position.y <= 0.0f) {
				this->Velocity.y *= -1;
				this->Position.y = 0.0f;
			}
			else if (this->Position.y + this->Size.y >= windowHeight) {
				this->Velocity.y *= -1;
				this->Position.y = windowHeight - this->Size.y;
			}
		}

		return this->Position;
	}

	void Ball::Reset(glm::vec2 position, glm::vec2 velocity) {
		this->Position = position;
		this->Velocity = velocity;
		this->IsStuck = true;
	}

	// Circle - AABB collision check
	bool Ball::IsColliding(GameObject& other) {
		glm::vec2 center(this->Position + this->Radius);

		glm::vec2 aabbHalf(other.Size.x / 2.f, other.Size.y / 2.f);
		glm::vec2 aabbCenter(other.Position.x + aabbHalf.x, other.Position.y + aabbHalf.y);

		glm::vec2 difference = center - aabbCenter;
		glm::vec2 clamped = glm::clamp(difference, -aabbHalf, aabbHalf);

		glm::vec2 closest = aabbCenter + clamped;
		difference = closest - center;

		return glm::length(difference) < this->Radius;
	}
}