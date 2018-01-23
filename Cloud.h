#pragma once
#include <iostream>
#include <vector>
#include "Particle.h"
using namespace std;

class Cloud {
public:
	static GLuint PuffTexture;

	Vector3* VertexBuffer;
	Vector2* TexCoordBuffer;
	Color4* ColorBuffer;

	float radius;
	float posX;
	float posY;
	float posZ;
	vector<Particle> particles;

	Cloud() {
		radius = posX = posY = posZ  = 0;
	}
	~Cloud() {
	}
};

GLuint Cloud::PuffTexture = 99;
