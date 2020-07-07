#include <imgui.h>
#include <imgui_internal.h>

#include "Kanan.hpp"
#include "Mabinogi.hpp"
#include "KCamera.hpp"

namespace kanan {
	std::optional<Matrix4x4> KCamera::getViewMatrix() const {
		Matrix4x4 m = state->transformMatrix;		
		Matrix4x4 inv;
		float det;
		int i;

		inv[0] = m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		inv[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		inv[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		inv[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		inv[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		inv[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		inv[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		inv[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		inv[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		inv[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		inv[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		inv[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		inv[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		inv[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		inv[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		inv[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		det = double(m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12]);

		if (det == 0)
			return Matrix4x4{};

		det = 1.0 / det;

		Matrix4x4 invOut{};

		for (i = 0; i < 16; i++)
			invOut[i] = inv[i] * det;

		return invOut;
	}
	std::optional<Matrix4x4> KCamera::getProjectionMatrix() const {
		float n = state->zNear;
		float f = state->zFar;
		float l, r, t, b;
		double scale = tan((state->fov) * 0.5 * 3.1415 / 180) * n;
		r = state->aspectRatio * scale;
		l = -r;
		t = scale;
		b = -t;
		Matrix4x4 m{};
		//Calculate projection matrix
		m[0] = (2 * n) / (r - l);
		m[1] = 0;
		m[2] = (r + l) / (r - l); // 0
		m[3] = 0;

		m[4] = 0;
		m[5] = (2 * n) / (t - b);
		m[6] = (t + b) / (t - b); //0
		m[7] = 0;

		m[8] = 0;
		m[9] = 0;
		m[10] = -(f + n) / (f - n);
		m[11] = (2 * f * n) / (f - n); //had to change this from the standard opengl projection matrix

		m[12] = 0;
		m[13] = 0;
		m[14] = -1;
		m[15] = 0;

		return m;
	}
	std::optional<Vector3> KCamera::worldToScreen(Vector3 pos) const {
		float x = pos[0];
		float y = pos[1];
		float z = pos[2];
		bool a = true;

		Matrix4x4 vm = getViewMatrix().value();
		Matrix4x4 pm = getProjectionMatrix().value();

		//convert world space point to eye space point
		float vv1 = double(vm[0]) * x + double(vm[1]) * y + double(vm[2]) * z + vm[3]; //x
		float vv2 = double(vm[4]) * x + double(vm[5]) * y + double(vm[6]) * z + vm[7]; //y
		float vv3 = double(vm[8]) * x + double(vm[9]) * y + double(vm[10]) * z + vm[11]; //z
		//w = 1
		//convert eye space point to clip space point
		float pv1 = double(pm[0]) * vv1 + double(pm[1]) * vv2 + double(pm[2]) * vv3 + double(pm[3]); //x
		float pv2 = double(pm[4]) * vv1 + double(pm[5]) * vv2 + double(pm[6]) * vv3 + double(pm[7]); //y
		float pv3 = double(pm[8]) * vv1 + double(pm[9]) * vv2 + double(pm[10]) * vv3 + double(pm[11]); //z
		float pv4 = -vv3; //w' = -z
		//normalize and convert to raster space
		return Vector3{ pv1 / pv4 * state->screenWidth, pv2 / pv4 * state->screenHeight, pv3 / pv4};
	}
}