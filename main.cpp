/********************************
CSE328 Project - Cloud Rendering
Author: Yifang Cao
Date: 03-23-2017
********************************/

#include <iostream>
#include <GL/glut.h>

#include "Particle.h"
#include "TgaLoader.h"
#include "CloudLoader.h"
#include "Cloud.h"

#ifndef M_PI
#define M_PI 3.141592654f
#endif // !M_PI

#define SKYBOX_FT 0
#define SKYBOX_BK 1
#define SKYBOX_LF 2
#define SKYBOX_UP 3
#define SKYBOX_DN 4
#define SKYBOX_RT 5

using namespace std;

static int main_window(0);
static int window_width(800);
static int window_height(600);
static float mouse_x(window_width / 2.0);
static float mouse_y(window_height / 2.0);

//varibles for FPS style camera
static float const SPEED(1.0f);
static float const MOUSE_SPEED(0.3f);
static float eyeX(-10.0f); // eye position -10
static float eyeY(25.0f);//25
static float eyeZ(40.5f);//40
static float yaw(0.0f);   // view orientation
static float pitch(8.0f);
static float roll(0.0f);
static const float PERSPECTIVE_FURTHEST = 500.0f;
static const float PERSPECTIVE_NEAREST = 0.001f;

static GLfloat sunPos[] = { -150, 150, -500, 0.0f };

// world boundry
static float x_Max_boundry = 100.0f;
static float x_Min_boundry = -100.0f;
static float y_Max_boundry = 100.0f;
static float y_Min_boundry = -60.0f;
static float z_Max_boundry = 100.0f;
static float z_Min_boundry = -100.0f;

//state var(s)

//colors
static GLfloat color_black[] = { 0.0f, 0.0f, 0.0f ,1.0f };
static GLfloat color_white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat my_color[] = { 0.0f, 0.6f, 0.6f, 1.0f};
static GLfloat color_cloud[] = { 1.0f, 1.0f, 1.0f, 0.8f };

//texture vars
GLuint Skybox[6];
Tga *skybox_tga_lf = new Tga("TropicalSunnyDay/cloudtop_lf.tga");
Tga *skybox_tga_bk = new Tga("TropicalSunnyDay/cloudtop_bk.tga");
Tga *skybox_tga_rt = new Tga("TropicalSunnyDay/cloudtop_rt.tga");
Tga *skybox_tga_up = new Tga("TropicalSunnyDay/cloudtop_up.tga");
Tga *skybox_tga_dn = new Tga("TropicalSunnyDay/cloudtop_dn.tga");
Tga *skybox_tga_ft = new Tga("TropicalSunnyDay/cloudtop_ft.tga");

//clouds data
Cloud* cloudObj = new Cloud();

/******************************
*** Draw all the funtions *****
******************************/
void motionHander(int x, int y);
void passiveMotionHandler(int x, int y);
void mouseHandler(int button, int state, int x, int y);
void displayHandler(void);
void reshapeHandler(int width, int height);
void keyboardHandler(unsigned char key, int x, int y);
void updateOrientation(void);
void initLighting(void);
void normcrossproduct(float v1[3], float v2[3], float out[3]);
void normalize(float v[3]); 
void initTextures(void);
//drawing funtionality

void draw_skybox();

void draw_cloud();

/********************
*		 END	    *
*********************/







void main(int argc, char** argv) {

	render_cloud(cloudObj);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(window_width, window_height);
	main_window = glutCreateWindow("CSE328 Cloud Rendering");

	glutReshapeFunc(reshapeHandler);
	glutDisplayFunc(displayHandler);
	glutMotionFunc(motionHander);
	glutMouseFunc(mouseHandler);
	glutPassiveMotionFunc(passiveMotionHandler);
	glutKeyboardFunc(keyboardHandler);

	//enable shading
	glEnable(GL_DEPTH_TEST);
	initLighting();
	initTextures();

	glutMainLoop();
}



void normcrossproduct(float v1[3], float v2[3], float out[3]) {
	out[0] = v1[1] * v2[2] - v1[2] * v2[1];
	out[1] = v1[2] * v2[0] - v1[0] * v2[2];
	out[2] = v1[0] * v2[1] - v1[1] * v2[0];
	normalize(out);
}

void normalize(float v[3]) {
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if (d == 0.0) {
		cout << ("zero length vector") << endl;;
		exit(1);
	}
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}

void mouseHandler(int button, int state, int x, int y) {
	//if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	//mouse_x = (float)x;
	//mouse_y = (float)y;
	//}
}
void passiveMotionHandler(int x, int y) {
	//update current mouse postion
	mouse_x = x;
	mouse_y = y;
}


void motionHander(int x, int y) {
	//calculate delt x and y
	float mouse_dx = x - mouse_x;
	float mouse_dy = y - mouse_y;
	mouse_x = x; //set lastx to the current x position
	mouse_y = y; //set lasty to the current y position
	pitch += (float)mouse_dy * MOUSE_SPEED; //set the xrot to xrot with the additionof the difference in the y position
	yaw += (float)mouse_dx * MOUSE_SPEED;    //set the xrot to yrot with the additionof the difference in the x position

											 //only 90 degree pitch is allowed
	if (fabs(pitch) > 90) {
		if (pitch < 0)
			pitch = -90;
		else
			pitch = 90;
	}
	if (yaw < 0) {
		yaw += 360;
	}
	if (yaw > 360) {
		yaw -= 360;
	}


	glutPostRedisplay(); // for redraw the orientation;
}



void initLighting(void) {
	//enable lighting

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	//glEnable(GL_COLOR_MATERIAL);

	//set light intensity and color
	GLfloat AmbientLt[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat DiffuseLt[] = { 1.8f, 1.8f, 1.8f, 1.0f };
	GLfloat SpecularLt[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLt);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLt);
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLt);
	//set position at initial eye position
	glLightfv(GL_LIGHT0, GL_POSITION, sunPos);

	GLfloat ltpos1[] = {-sunPos[0],-sunPos[1] ,-sunPos[2], 1.0f };
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLt);
	glLightfv(GL_LIGHT1, GL_POSITION, ltpos1);


	glShadeModel(GLU_SMOOTH);
}
void keyboardHandler(unsigned char key, int x, int y) {
	//set boundries of the world
	if (eyeX > x_Max_boundry) {			//x boundry
		eyeX = x_Max_boundry;
	}
	if (eyeX < x_Min_boundry) {
		eyeX = x_Min_boundry;
	}
	if (eyeY > y_Max_boundry) {					//y boundry
		eyeY = y_Max_boundry;
	}
	if (eyeY < y_Min_boundry) {
		eyeY = y_Min_boundry;
	}
	if (eyeZ > z_Max_boundry) {			//z boundry
		eyeZ = z_Max_boundry;
	}
	if (eyeZ < z_Min_boundry) {			//z boundry
		eyeZ = z_Min_boundry;
	}
	// orientation in radian
	float pitchrad(0.0f);
	float rollrad(0.0f);
	float yawrad(0.0f);
	switch (key) {
	case 'w':
		yawrad = (yaw / 180 * M_PI);
		pitchrad = (pitch / 180 * M_PI);
		eyeX += float(sin(yawrad))* SPEED;
		eyeY -= float(sin(pitchrad))* SPEED;
		eyeZ -= float(cos(yawrad))* SPEED;
		break;
	case 's':
		yawrad = (yaw / 180 * M_PI);
		pitchrad = (pitch / 180 * M_PI);
		eyeX -= float(sin(yawrad))* SPEED;
		eyeY += float(sin(pitchrad))* SPEED;
		eyeZ += float(cos(yawrad))* SPEED;
		break;
	case 'a':
		yawrad = (yaw / 180 * M_PI);
		eyeX -= float(cos(yawrad)) * SPEED;
		eyeZ -= float(sin(yawrad)) * SPEED;
		break;
	case 'd':
		yawrad = (yaw / 180 * M_PI);
		eyeX += float(cos(yawrad)) * SPEED;
		eyeZ += float(sin(yawrad)) * SPEED;
		break;
	case 'f':
		cout << "f" << endl;
		break;
	case 27:

		glDeleteTextures(6,Skybox);
		glutDestroyWindow(main_window);
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void reshapeHandler(int width, int height) {
	window_width = width;
	window_height = height;
	glViewport(0, 0, (GLsizei)window_width, (GLsizei)window_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)window_width / (double)window_height, PERSPECTIVE_NEAREST, PERSPECTIVE_FURTHEST);
	gluLookAt(0, 0, 1,  //eye position
		0, 0, 0,		//at center
		0, 1.0, 0);		//look Y up
	glutPostRedisplay();
}

void updateOrientation(void) {
	glRotatef(pitch, 1.0, 0.0, 0.0);
	glRotatef(yaw, 0.0, 1.0, 0.0);
	glTranslated(-eyeX, -eyeY, -eyeZ);
}

void drawTriangle(float *v1, float *v2, float *v3) {
	glBegin(GL_TRIANGLES);
	glNormal3fv(v1);
	glVertex3fv(v1);
	glNormal3fv(v2);
	glVertex3fv(v2);
	glNormal3fv(v3);
	glVertex3fv(v3);
	glEnd();
}


void initTextures(void) {

	glEnable(GL_TEXTURE_2D);
	glGenTextures(6, Skybox);

	cout << "INFO: Loading Texture 1" << endl;
	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_FT]);
	glTexImage2D(GL_TEXTURE_2D, 0,
		skybox_tga_ft->HasAlphaChannel() ? GL_RGBA : GL_RGB, skybox_tga_ft->GetWidth(), skybox_tga_ft->GetWidth(),
		0, GL_RGB, GL_UNSIGNED_BYTE,
		skybox_tga_ft->GetPixels().data());

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_BK]);
	glTexImage2D(GL_TEXTURE_2D, 0,
		skybox_tga_bk->HasAlphaChannel() ? GL_RGBA : GL_RGB, skybox_tga_bk->GetWidth(), skybox_tga_bk->GetWidth(),
		0, GL_RGB, GL_UNSIGNED_BYTE,
		skybox_tga_bk->GetPixels().data());


	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_DN]);
	glTexImage2D(GL_TEXTURE_2D, 0,
		skybox_tga_dn->HasAlphaChannel() ? GL_RGBA : GL_RGB, skybox_tga_dn->GetWidth(), skybox_tga_dn->GetWidth(),
		0, GL_RGB, GL_UNSIGNED_BYTE,
		skybox_tga_dn->GetPixels().data());

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_LF]);
	glTexImage2D(GL_TEXTURE_2D, 0,
		skybox_tga_lf->HasAlphaChannel() ? GL_RGBA : GL_RGB, skybox_tga_lf->GetWidth(), skybox_tga_lf->GetWidth(),
		0, GL_RGB, GL_UNSIGNED_BYTE,
		skybox_tga_lf->GetPixels().data());

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_UP]);
	glTexImage2D(GL_TEXTURE_2D, 0,
		skybox_tga_up->HasAlphaChannel() ? GL_RGBA : GL_RGB, skybox_tga_up->GetWidth(), skybox_tga_up->GetWidth(),
		0, GL_BGR_EXT, GL_UNSIGNED_BYTE,
		skybox_tga_up->GetPixels().data());

	cout << "INFO: Loading Texture 5" << endl;

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_RT]);
	glTexImage2D(GL_TEXTURE_2D, 0,
		skybox_tga_rt->HasAlphaChannel() ? GL_RGBA : GL_RGB, skybox_tga_rt->GetWidth(), skybox_tga_rt->GetWidth(),
		0, GL_RGB, GL_UNSIGNED_BYTE,
		skybox_tga_rt->GetPixels().data());

	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, Skybox[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	}
	delete skybox_tga_lf;
	delete skybox_tga_rt;
	delete skybox_tga_bk;
	delete skybox_tga_up;
	delete skybox_tga_dn;
	delete skybox_tga_ft;
}


void draw_skybox() {

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	//rotation only
	glRotatef(pitch, 1.0, 0.0, 0.0);
	glRotatef(yaw, 0.0, 1.0, 0.0);

	glScaled(PERSPECTIVE_FURTHEST/2.0f, PERSPECTIVE_FURTHEST / 2.0f, PERSPECTIVE_FURTHEST / 2.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_BK]);
	//glBindTexture(GL_TEXTURE_2D, Cloud::PuffTexture); 
	glBegin(GL_QUADS); 
		glTexCoord2f(0, 0); glVertex3f(-1, -1, -1);  
		glTexCoord2f(1, 0);	glVertex3f(1, -1, -1); 
		glTexCoord2f(1, 1);	glVertex3f(1, 1, -1);  
		glTexCoord2f(0, 1);	glVertex3f(-1, 1, -1);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_LF]);
	glBegin(GL_QUADS);
		glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
		glTexCoord2f(1, 1);	glVertex3f(-1, 1, -1);
		glTexCoord2f(0, 1);	glVertex3f(-1, 1, 1);
		glTexCoord2f(0, 0);	glVertex3f(-1, -1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_UP]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(-1, 1, -1);
		glTexCoord2f(0, 0);	glVertex3f(1, 1, -1);
		glTexCoord2f(1, 0);	glVertex3f(1, 1, 1);
		glTexCoord2f(1, 1);	glVertex3f(-1, 1, 1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_RT]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(1, 1, -1);
		glTexCoord2f(1, 1);	glVertex3f(1, 1, 1);
		glTexCoord2f(1, 0);	glVertex3f(1, -1, 1);
		glTexCoord2f(0, 0);	glVertex3f(1, -1, -1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_FT]);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1); glVertex3f(1, 1, 1);
		glTexCoord2f(1, 1);	glVertex3f(-1, 1, 1);
		glTexCoord2f(1, 0);	glVertex3f(-1, -1, 1);
		glTexCoord2f(0, 0);	glVertex3f(1, -1, 1);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


void draw_cloud() {
	generateCloudTexture();
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, Cloud::PuffTexture);
	//glBindTexture(GL_TEXTURE_2D, Skybox[SKYBOX_BK]);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set blending function.
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	float mat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);

	Vector3 vx(mat[0], mat[4], mat[8]);
	Vector3 vy(mat[1], mat[5], mat[9]);
	Vector3 Corner1, Corner2, Corner3, Corner4;
	Corner1 = -vx - vy; Corner2 = vx - vy; Corner3 = vx + vy; Corner4 = vy - vx;

	Vector3* vp; Vector2* tp; Color4* cp;
	vp = cloudObj->VertexBuffer;
	tp = cloudObj->TexCoordBuffer;
	cp = cloudObj->ColorBuffer;
	Vector2 v1(1.0f, 0.0f), v2(0.0f, 0.0f), v3(0.0f, 1.0f), v4(1.0f, 1.0f);
	 
	glTranslatef(cloudObj->posX, cloudObj->posY,cloudObj->posZ);
	for (int i = 0; i < (int)cloudObj->particles.size(); i++) {
		Particle p = cloudObj->particles[i];

		Color4 *curColor = &p.color;
		curColor->R = curColor->G =  curColor->B = 1.0f; 
		curColor->A = 1;
		float curColorf[] = { curColor->R, curColor->G, curColor->B, curColor->A };
		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, curColorf);
		//glColor4fv(curColorf);
		Vector3 particlePos = { p.x,p.y,p.z };
		*(vp) = particlePos + Corner1 * p.size;
		*(tp) = v1;// { 1.0f, 0.0f }; //vertex 1
		*(cp++) = *curColor;
		GLfloat pos1[] = {vp->x,vp->y,vp->z};
		GLfloat tpos1[] = { tp->x,tp->y };
		tp++;
		vp++;

		*(vp) = particlePos + Corner2 * p.size;
		*(tp) = v2;// { 0.0f, 0.0f }; //vertex 2
		*(cp++) = *curColor;
		GLfloat pos2[] = { vp->x,vp->y,vp->z };
		GLfloat tpos2[] = { tp->x,tp->y };
		tp++;
		vp++;

		*(vp) = particlePos + Corner3 * p.size;
		*(tp) = v3;//{ 0.0f, 1.0f };//vertex 4
		*(cp++) = *curColor;
		GLfloat pos3[] = { vp->x,vp->y,vp->z };
		GLfloat tpos3[] = { tp->x,tp->y };
		tp++;
		vp++;

		*(vp) = particlePos + Corner4 * p.size;
		*(tp) = v4;// { 1.0f, 1.0f };//vertex 4
		*(cp++) = *curColor;
		GLfloat pos4[] = { vp->x,vp->y,vp->z };
		GLfloat tpos4[] = { tp->x,tp->y };
		tp++;
		vp++;



		glBindTexture(GL_TEXTURE_2D, Cloud::PuffTexture);
		glPushMatrix();
			updateOrientation();
			glTranslatef(p.x, p.y, p.z-66); 
			Vector3 billboardV(0,0,1);
			Vector3 eyeV(eyeX,eyeY,eyeZ);
			Vector3 rotate_x(0,eyeY, eyeZ);
			Vector3 rotate_y(0, eyeY, eyeZ);
			float rotate_angle_x = 180/Angle(rotate_x, billboardV)*M_PI;
			float rotate_angle_y = 180 / Angle(rotate_y, billboardV)*M_PI;
			//cout << "Angle :" << billboard_angle << endl;
			//glRotated(rotate_angle_x, 1, 0, 0);
			//glRotated(rotate_angle_y,0,1,0);
			//glScaled(p.size, p.size, p.size);

			glDisable(GL_DEPTH_TEST);
			glBegin(GL_QUADS);
				glColor4fv(curColorf);
				glTexCoord2fv(tpos1); glVertex3fv(pos1);
				glTexCoord2fv(tpos2); glVertex3fv(pos2);
				glTexCoord2fv(tpos3); glVertex3fv(pos3);
				glTexCoord2fv(tpos4); glVertex3fv(pos4);
			glEnd();

			/*
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, cloudObj->VertexBuffer);
			glColorPointer(4, GL_FLOAT, 0, cloudObj->ColorBuffer);
			glTexCoordPointer(2, GL_FLOAT, 0, cloudObj->TexCoordBuffer);

			glDrawArrays(GL_QUADS, 0, cloudObj->particles.size() * 4);

			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			*/
		glPopMatrix();

		/*
		glPushMatrix();
		updateOrientation();
		glTranslated(p.x,p.y,p.z);
		glScaled(p.size, p.size, p.size);
		glutSolidIcosahedron();
		glPopMatrix();
		*/
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_COLOR_MATERIAL);

}

/*
void setOrthographicProjection() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, window_width, 0, window_height);
	glScalef(1, -1, 1);
	glTranslatef(0, -window_height, 0);
	glMatrixMode(GL_MODELVIEW);
}
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void renderBitmapString(float x, float y, void *font, const char *string) {
	const char *c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}*/
void drawStrokeText(char*string, float x, float y, float z)
{
	glDisable(GL_DEPTH_TEST);
	char *c;
	glPushMatrix();
		glTranslatef(x,  y, z);

		glScalef(0.0004f, 0.0004f, 0);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color_black);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color_black);
		glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
		glColor4f(0.0f,0.0f,0.3f,1.0f);
		
		for (c = string; *c != '\0'; c++){
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
		}
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void displayHandler(void) {
	
	//background color
	float r = 113 / 255.0;
	float g = 112 / 255.0;
	float b = 117 / 255.0;
	glClearColor(r, g, b, 1.0); //set background
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	/*
	glDisable(GL_DEPTH_TEST);
	glColor4d(1.0, 0.0, 0.0,1.0f);
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(200, 200, GLUT_BITMAP_9_BY_15, "Font Rendering - Programming Techniques");
	renderBitmapString(300, 240, GLUT_BITMAP_9_BY_15, "Esc - Quit");
	glPopMatrix();
	resetPerspectiveProjection();
	glEnable(GL_DEPTH_TEST);
	*/

	glPushMatrix();
		updateOrientation();
		initLighting();
	glPopMatrix();

	/*
	glPushMatrix();
	glTranslatef(0, -25, -10);
	updateOrientation();
	glScalef(0.5, 0.01, 20);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, my_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, my_color);
	glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
	//specify specular effect
	glutSolidCube(20);
	glPopMatrix();
	*/

	glPushMatrix();
		draw_skybox();
	glPopMatrix();


	glPushMatrix();
	updateOrientation(); 
		glTranslatef(cloudObj->posX, cloudObj->posY, cloudObj->posZ);
		glutWireSphere(cloudObj->radius,30,30);
	glPopMatrix();



	//draw flying object
	glPushMatrix();
	updateOrientation();
		glTranslatef(0, 0, -10);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, my_color);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color_white);
		glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
		//specify specular effect
		glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
		glRotatef(pitch, 1.0, 0.0, 0.0);
		glRotatef(yaw, 0.0, 1.0, 0.0);
		glTranslatef(sunPos[0] / 10.0f, sunPos[1] / 10.0f, sunPos[2] / 10.0f);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, my_color);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color_white);
		glMaterialf(GL_FRONT, GL_SHININESS, 40.0);
		//specify specular effect
		glutSolidTeapot(1);
	glPopMatrix();
	
	//stat string on the screen
	char string[255];
	sprintf_s(string,255,"Pitch:%.2f  Yaw:%.2f  Roll:%.2f", pitch,yaw,roll);
	drawStrokeText(string, -1.5f, 1.0f, -1.0f);
	sprintf_s(string, 255, "Eye Position(%.2f, %.2f, %.2f)", eyeX, eyeY, eyeZ);
	drawStrokeText(string, -1.5f, 1.08f, -1.0f);


	glPushMatrix();
	draw_cloud();
	glPopMatrix();

	glutSwapBuffers();
}