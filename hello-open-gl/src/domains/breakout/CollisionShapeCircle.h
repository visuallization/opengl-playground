#pragma once

#include "./CollisionShape.h"

namespace breakout {
	class CollisionShapeCircle: public CollisionShape {
	public:
		float Radius;

		CollisionShapeCircle();
		CollisionShapeCircle(glm::vec2 position, glm::vec2 size, float radius);

		void Draw(SpriteRenderer& renderer) override;

		glm::vec2 GetCenter() const;
	};
}