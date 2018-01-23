#pragma once
#include <cstdio>
#include "vecmath.h" 
#include "Cloud.h"
using namespace std;
const char filename[] = "CloudsData/cloud4.dat";

//load a data set of cloud
void LoadClouadData(Cloud* cloud) {

	FILE* fp;
	if (fopen_s(&fp, filename, "rb")) {
		cout << "ERROR: File Loading Error." << endl;
		exit(1);
	}
	int Num;
	fread(&Num, sizeof(int), 1, fp);
	cloud->VertexBuffer = new Vector3[Num * 4];
	cloud->TexCoordBuffer = new Vector2[Num * 4];
	cloud->ColorBuffer = new Color4[Num * 4];
	float temp[3];
	for (int i = 0; i < Num; i++) {
		fread(&temp, sizeof(float)*3, 1, fp);
		Particle particle(temp[0], temp[1], temp[2]);
		cloud->particles.push_back(particle);
		//update current cloud radius 
		if (vec_magnitude(temp) > cloud->radius)
			cloud->radius = vec_magnitude(temp);
	}
	
	for (int i = 0; i < Num; i++)
	{
		fread(&cloud->particles[i].size, sizeof(float), 1, fp);
	}
	for (int i = 0; i < Num; i++)
	{
		fread(&cloud->particles[i].color, sizeof(Color4), 1, fp);
	}
	fclose(fp);
	cout << "Particle Num: " << Particle::getCount() << endl;
	cout << "Cloud("<<cloud->posX<<","<< cloud->posY<<","<< cloud->posZ<<") Size: " << cloud->radius << endl;
			/*
		//compute the bounding box, used for rendering impostors and frustum culling
		Cloud->BoundingBox1 = Vector3(-1000, -1000, -1000);
		Cloud->BoundingBox2 = Vector3(1000, 1000, 1000);

		if (puff.Position.x > Cloud->BoundingBox1.x) Cloud->BoundingBox1.x = puff.Position.x;
		if (puff.Position.y > Cloud->BoundingBox1.y) Cloud->BoundingBox1.y = puff.Position.y;
		if (puff.Position.z > Cloud->BoundingBox1.z) Cloud->BoundingBox1.z = puff.Position.z;
		if (puff.Position.x < Cloud->BoundingBox2.x) Cloud->BoundingBox2.x = puff.Position.x;
		if (puff.Position.y < Cloud->BoundingBox2.y) Cloud->BoundingBox2.y = puff.Position.y;
		if (puff.Position.z < Cloud->BoundingBox2.z) Cloud->BoundingBox2.z = puff.Position.z;

		puff.Position += Position;
		puff.Angle = 0.0f;
		puff.ID = 0;
		puff.Life = 1.0f;

		Cloud->Puffs.push_back(puff);
		
	}
	for (int i = 0; i < Num; i++)
	{
		fread(&Cloud->Puffs[i].Size, sizeof(float), 1, fp);
	}
	for (int i = 0; i < Num; i++)
	{
		fread(&Cloud->Puffs[i].Color, sizeof(Color4), 1, fp);
	}
	fclose(fp);

	//allocate buffers for rendering
	Cloud->VertexBuffer = new Vector3[Num * 4];
	Cloud->TexCoordBuffer = new Vector2[Num * 4];
	Cloud->ColorBuffer = new Color4[Num * 4];
	*/
}


//cloud texture generator 
void generateCloudTexture() {
	int N = 64;
	unsigned char *B = new unsigned char[4 * N*N];
	float X, Y, Dist;
	float Incr = 2.0f / N;
	int i = 0, j = 0;
	float value;

	Y = -1.0f;
	for (int y = 0; y < N; y++)
	{
		X = -1.0f;
		for (int x = 0; x<N; x++, i++, j += 4)
		{
			Dist = (float)sqrt(X*X + Y*Y);
			if (Dist > 1) Dist = 1;

			//our magical interpolation polynomical
			value = 2 * Dist*Dist*Dist - 3 * Dist*Dist + 1;
			value *= 0.4f;

			B[j + 3] = B[j + 2] = B[j + 1] = B[j] = (unsigned char)(value * 255);
			X += Incr;
		}
		Y += Incr;
	}
	glGenTextures(1, &Cloud::PuffTexture);
	glBindTexture(GL_TEXTURE_2D, Cloud::PuffTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, N, N, 0, GL_RGBA, GL_UNSIGNED_BYTE, B);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	cout << "Pfftexture " << Cloud::PuffTexture << endl;
	/*
	for (int i = 0; i < (4 * N*N); i++)
	{
		if(i%4==0)	printf("\n");
		printf("%d: %d\n",i, B[i]);
		if (B[i] > 101) break;
	}*/

	delete[] B;
}

class	DescSortComparison{
public:
	bool operator () (Particle particle1, Particle particle2)
	{
		return particle1.distanceToEye > particle2.distanceToEye;
	}
} DescSort;

void sort_cloud_particle(Cloud* cloud, float* loc) {
	for (int i = 0; i < (int)cloud->particles.size(); i++) {
		float particle_loc[] = { cloud->particles[i].x, cloud->particles[i].y,cloud->particles[i].z};
		cloud->particles[i].distanceToEye = vec_distance(loc, particle_loc);
	}
	sort(cloud->particles.begin(),cloud->particles.end(), DescSort);
}


extern float eyeX;
extern float eyeY;
extern float eyeZ;
void render_cloud(Cloud* cloud) {
	LoadClouadData(cloud);
	generateCloudTexture();
	float eyepos[] = { eyeX ,eyeY,eyeZ };
	sort_cloud_particle(cloud,eyepos);
}