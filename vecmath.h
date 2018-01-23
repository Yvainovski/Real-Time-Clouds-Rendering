#pragma once
#include <cmath>
#include <algorithm>
#include <vector>
//take a vector with one end at the origin and calculate its magnitued
float vec_magnitude(float* vec) {
	return sqrt(vec[0]*vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

 float vec_distance(float* vec1, float* vec2)
{
	return (float)sqrtf(
		  (vec1[0] - vec2[0]) * (vec1[0] - vec2[0]) 
		+ (vec1[1] - vec2[1]) * (vec1[1] - vec2[1]) 
		+ (vec1[2] - vec2[2]) * (vec1[2] - vec2[2]));
 }


 class Vector2
 {
 public:
	 Vector2()
	 {
		 x = y = 0.0f;
	 }

	 Vector2(float px, float py)
	 {
		 x = px; y = py;
	 }

	 Vector2 operator+(Vector2 vVector2)
	 {
		 return Vector2(vVector2.x + x, vVector2.y + y);
	 }

	 Vector2 operator-(Vector2 vVector2)
	 {
		 return Vector2(x - vVector2.x, y - vVector2.y);
	 }

	 Vector2 operator*(float num)
	 {
		 return Vector2(x * num, y * num);
	 }

	 Vector2 operator/(float num)
	 {
		 return Vector2(x / num, y / num);
	 }

	 Vector2 operator-()
	 {
		 return Vector2(-x, -y);
	 }

	 float* operator!()
	 {
		 return (float*)this;
	 }

	 Vector2& operator *=(const float Scalar)
	 {
		 x *= Scalar; y *= Scalar;
		 return *this;
	 }

	 Vector2& operator +=(const Vector2 Other)
	 {
		 x += Other.x;	y += Other.y;
		 return *this;
	 }

	 Vector2& operator -=(const Vector2 Other)
	 {
		 x -= Other.x;	y -= Other.y;
		 return *this;
	 }

	 float x, y;
 };


 class Vector3 {
 public:

	 Vector3()
	 {
		 x = y = z = 0.0f;
	 }

	 Vector3(float px, float py, float pz)
	 {
		 x = px; y = py; z = pz;
	 }

	 Vector3 operator+(Vector3 vVector3)
	 {
		 return Vector3(vVector3.x + x, vVector3.y + y, vVector3.z + z);
	 }

	 Vector3 operator-(Vector3 vVector3)
	 {
		 return Vector3(x - vVector3.x, y - vVector3.y, z - vVector3.z);
	 }

	 Vector3 operator*(float num)
	 {
		 return Vector3(x * num, y * num, z * num);
	 }

	 Vector3 operator/(float num)
	 {
		 return Vector3(x / num, y / num, z / num);
	 }

	 Vector3 operator-()
	 {
		 return Vector3(-x, -y, -z);
	 }

	 float* operator!()
	 {
		 return (float*)this;
	 }

	 Vector3& operator *=(const float Scalar)
	 {
		 x *= Scalar; y *= Scalar; z *= Scalar;
		 return *this;
	 }

	 Vector3& operator +=(const Vector3 Other)
	 {
		 x += Other.x;	y += Other.y;	z += Other.z;
		 return *this;
	 }

	 Vector3& operator -=(const Vector3 Other)
	 {
		 x -= Other.x;	y -= Other.y;	z -= Other.z;
		 return *this;
	 }

	 float x, y, z;
 };

 inline float Magnitude(Vector3 v)
 {
	 return (float)sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
 }

 inline float Magnitude(Vector2 v)
 {
	 return (float)sqrtf(v.x * v.x + v.y * v.y);
 }

 inline float Dot(Vector3 v1, Vector3 v2)
 {
	 return (float)(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
 }
 inline float Angle(Vector3 v1, Vector3 v2)
 {
	 return Dot(v1, v2) / (Magnitude(v1) * Magnitude(v2));
 }