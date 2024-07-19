#pragma once
#include "Math.h"

constexpr Vector2 V2_LEFT{ 0.0f, -1.0f };
constexpr Vector2 V2_RIGHT{ 0.0f, 1.0f };
constexpr Vector2 V2_UP{ 0.0f, -1.0f };
constexpr Vector2 V2_DOWN{ 0.0f, 1.0f };

// Screen-space coordinates so don't expect gravity to mirror the real world & also look correct.
constexpr Vector2 GRAVITY{ 0.0f, 100.0f };

struct Rigidbody
{
	Vector2 vel{};
	Vector2 acc{};
};

// Usage: vel` = integrate(vel, acc, dt)
inline Vector2 Integrate(Vector2 value, Vector2 change, float time)
{
	return value + change * time;
}

// MTV resolves circle from line
inline bool LineCircle(Vector2 start, Vector2 end, Vector2 circle, float radius, Vector2* mtv = nullptr)
{
    Vector2 nearest = ProjectPointLine(start, end, circle);
    bool collision = DistanceSqr(nearest, circle) <= radius * radius;
    if (collision && mtv != nullptr)
    {
        Vector2 resolutionDirection = Normalize(circle - nearest);
        float resolutionLength = radius - Distance(nearest, circle);
        *mtv = resolutionDirection * resolutionLength;
    }
    return collision;
}
