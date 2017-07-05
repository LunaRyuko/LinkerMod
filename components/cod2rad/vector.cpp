#include "stdafx.h"

void Vec3Normalize(vec3* v)
{
	float m2 = v->x * v->x + v->y * v->y + v->z * v->z;

	// Avoid sqrt of values <= 0
	float m = m2 > 0.0f ? (float)sqrt(m2) : 1.0f;

	// Avoid division by 0
	if (-m >= 0.0f)
		m = 1.0f;

	*v /= m;
}

void Vec3Normalize(float* v)
{
	Vec3Normalize((vec3*)v);
}

float Vec3Dot(vec3* a, vec3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

float Vec3Dot(float* a, float* b)
{
	return Vec3Dot((vec3*)a, (vec3*)b);
}

double Vec2Variance(vec2* a, vec2* b)
{
	return abs(a->x - b->x) + abs(a->y - b->y);
}

double Vec3Variance(vec3* a, vec3* b)
{
	return abs(a->x - b->x) + abs(a->y - b->y) + abs(a->z - b->z);
}