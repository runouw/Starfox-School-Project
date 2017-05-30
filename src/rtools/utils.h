#pragma once

#include <GL/glew.h>

#include <gltools\gltools.h>

#define EPSILON 0.000001f

using namespace GLTools;

class RTools{
public:
	static GLProgram* compileShader(const char* vert, const char* frag);

	/* Prepares object to be rendered as a billboard by setting the rotation part (3x3) to an identity matrix. */
	static GsMat billboardify(GsMat mat);

	static void elasticBounds(float& val, float max, float fr, float dt){
		if (val > max){
			float over = val - max;

			val += (-over) * (1 - pow(fr, dt));
		}
		if (val < -max){
			float over = val + max;

			val += (-over) * (1 - pow(fr, dt));
		}

	}

	static float easeInOutQuad(float t, float b, float c, float d) {
		t /= d / 2.0f;
		if (t < 1) return c / 2.0f * t*t + b;
		t--;
		return -c / 2.0f * (t*(t - 2.0f) - 1.0f) + b;
	};


	static bool intersectLineSegmentAABB(const GsVec& start, const GsVec& end, const GsVec& min, const GsVec& max){
		GsVec d = (end - start) * 0.5f;

		GsVec e = (max - min) * 0.5f;

		GsVec c = start + d - (min + max) * 0.5f;

		GsVec ad = d; // Returns same vector with all components positive
		ad.abs();

		if (fabsf(c.x) > e.x + ad.x)
			return false;

		if (fabsf(c.y) > e.y + ad.y)
			return false;

		if (fabsf(c.z) > e.z + ad.z)
			return false;

		if (fabsf(d.y * c.z - d.z * c.y) > e.y * ad.z + e.z * ad.y + EPSILON)
			return false;

		if (fabsf(d.z * c.x - d.x * c.z) > e.z * ad.x + e.x * ad.z + EPSILON)
			return false;

		if (fabsf(d.x * c.y - d.y * c.x) > e.x * ad.y + e.y * ad.x + EPSILON)
			return false;

		return true;
	}


	static float intersectRayTriangle(const GsVec& start, const GsVec& end, const GsVec& p0, const GsVec& p1, const GsVec& p2){
		// returns distance along the ray (if < 0, no intersection happened).
		GsVec u, v, n;
		u = p1 - p0;
		v = p2 - p0;
		n = cross(u, v);
		if (n == GsVec(0, 0, 0)){
			return -1;
		}

		GsVec dir, w0, w;
		dir = end - start;
		dir.normalize();

		w0 = start - p0;

		float a, b;
		a = -dot(n, w0);
		b = dot(n, dir);
		if (abs(a) < .000001f){
			// Ray lies olong same plane as triangle
			return -1;
		}

		float r = a / b;
		if (r < 0){
			// ray is away from triangle
			return -1;
		}

		GsVec I = start + r*dir;

		// is I inside T?
		float uu, uv, vv, wu, wv, D;
		uu = dot(u, u);
		uv = dot(u, v);
		vv = dot(v, v);
		w = I - p0;
		wu = dot(w, u);
		wv = dot(w, v);
		D = uv * uv - uu * vv;

		// get and test parametric coords
		float s, t;
		s = (uv * wv - vv * wu) / D;
		if (s < 0.0 || s > 1.0)         // I is outside T
			return -1;
		t = (uv * wu - uu * wv) / D;
		if (t < 0.0 || (s + t) > 1.0)  // I is outside T
			return -1;

		// TODO: get length of ray
		return r; // inside

	}

};


struct RColor{
	union{
		struct { float r, g, b, a; };
		float e[4];
	};

	RColor() : r(1), g(1), b(1), a(1) {}

	RColor(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

	static const RColor WHITE;
};