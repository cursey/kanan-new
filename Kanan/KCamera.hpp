#pragma once

#include <optional>

#include "Mabinogi.hpp"

namespace kanan {
	class KCamera : public CCamera {
	public:
		std::optional<Matrix4x4> getViewMatrix() const; //returns the view matrix
		std::optional<Matrix4x4> getProjectionMatrix() const; //calculates and returns the projection matrix
		std::optional<Vector3> worldToScreen(Vector3 pos) const; //transforms a point in world space to screen coordinates (in pixels)
	};
}