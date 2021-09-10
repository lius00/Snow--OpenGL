#include <gl/glut.h>
//#include <GL\glew.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<iostream>
#include "vector"
#include<SOIL.h>
using namespace std;
#define PI 3.1415926535

bool mouseflag = true;
struct SnowPoint {
	float x;
	float y;
	float z;
}snowPoint;

//SnowPoint* points = new SnowPoint[10];
vector<SnowPoint>points;

double point_z = 0.0;

static int du = 90, OriX = -1, OriY = -1;   //du是视点和x轴的夹角
static float ry = 1.5, h = 0.0;   //r是视点绕y轴的半径，h是视点高度即在y轴上的坐标
static float c = PI / 180.0;    //弧度和角度转换参数

GLfloat fov = 130.0f;

GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLfloat diffuseLight1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat ambientLight1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat specularLight1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat a[] = { 0.0f, 0.0f, 8.0f, 1.0f };//聚光灯位置
GLfloat spot_dir[] = { 0.0, 0.0, -1.0 };//聚光灯方向
GLfloat global_ambient[] = { 1.0, 1.0, 1.0, 1 };//全局环境光

GLfloat diffuseLight2[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat ambientLight2[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat specularLight2[] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat a2[] = { 0.0f, 1.0f, 0.0f, 0.0f };//方向光
int dirlight = 0;//控制方向光开关

GLuint TexID[2];
GLuint SkyBoxTexID[6];

static int globalx = -1, globaly = -1;

GLvoid loadTexture() {
	/* load an image file directly as a new OpenGL texture */
	TexID[0] = SOIL_load_OGL_texture
	(
		"..//Debug//images//create.bmp",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (TexID[0] == 0)
		printf("loading texture failed\n");

	TexID[1] = SOIL_load_OGL_texture
	(
		"..//Debug//images//spheretexture.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (TexID[1] == 0)
		printf("loading texture failed\n");

	SkyBoxTexID[0] = SOIL_load_OGL_texture
	(
		"..//Debug//images//top.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (SkyBoxTexID[0] == 0)
		printf("loading texture failed\n");

	SkyBoxTexID[1] = SOIL_load_OGL_texture
	(
		"..//Debug//images//bottom.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (SkyBoxTexID[1] == 0)
		printf("loading texture failed\n");

	SkyBoxTexID[2] = SOIL_load_OGL_texture
	(
		"..//Debug//images//left.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (SkyBoxTexID[2] == 0)
		printf("loading texture failed\n");

	SkyBoxTexID[3] = SOIL_load_OGL_texture
	(
		"..//Debug//images//right.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (SkyBoxTexID[3] == 0)
		printf("loading texture failed\n");

	SkyBoxTexID[4] = SOIL_load_OGL_texture
	(
		"..//Debug//images//front.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (SkyBoxTexID[4] == 0)
		printf("loading texture failed\n");

	SkyBoxTexID[5] = SOIL_load_OGL_texture
	(
		"..//Debug//images//back.jpg",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_INVERT_Y
	);
	if (SkyBoxTexID[5] == 0)
		printf("loading texture failed\n");


}

void Initialize() {
	glClearColor(0.0, 0.0, 0.0, 0.0); /*Specify the red, green, blue, and alpha values used when the
									  color buffers are cleared*/
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	gluPerspective(fov, 1.0f, 0.1f, 10.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(ry * cos(c * du), h, ry * sin(c * du), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);   //从视点看远点
	struct SnowPoint sp, sp1, sp2, sp3, sp4, sp5, sp6;
	sp.x = 0.0;
	sp.y = 1.0;
	sp.z = 0.0;
	points.push_back(sp);
	sp1.x = 4.0;
	sp1.y = 0.0;
	sp1.z = 0.0;
	/*points.push_back(sp1);
	sp2.x = -4.0;
	sp2.y = 0.0;
	sp2.z = 0.0;
	points.push_back(sp2);
	sp3.x = 0.0;
	sp3.y = 4.0;
	sp3.z = 0.0;
	points.push_back(sp3);
	sp4.x = 0.0;
	sp4.y = -4.0;
	sp4.z = 0.0;
	points.push_back(sp4);
	sp5.x = 0.0;
	sp5.y = 1.0;
	sp5.z = 0.0;
	points.push_back(sp5);*/

	//法向量
	glEnable(GL_NORMALIZE);
	glNormal3f(0.0, 0.0, 1.0);

	//全局环境光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

	//材质
	//glMaterialfv(GL_FRONT,GL_AMBIENT, ambientMaterial);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	//glMaterialfv(GL_FRONT,GL_SPECULAR, specularMaterial);

	//光照 聚光灯
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, a);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0);

	//加一个方向光
	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight2);
	glLightfv(GL_LIGHT2, GL_POSITION, a2);

	//纹理映射参数
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	loadTexture();

	//glEnable(GL_TEXTURE_CUBE_MAP);
}


void DrawCube() {
	//上
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[0]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-19.0f, 19.0f, 19.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(19.0f, 19.0f, 19.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(19.0f, 19.0f, -19.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-19.0f, 19.0f, -19.0f);
	glEnd();
	//下
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[1]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-19.0f, -19.0f, 19.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(19.0f, -19.0f, 19.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(19.0f, -19.0f, -19.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-19.0f, -19.0f, -19.0f);
	glEnd();
	//左
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[2]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-19.0f, 19.0f, -19.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-19.0f, 19.0f, 19.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-19.0f, -19.0f, 19.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-19.0f, -19.0f, -19.0f);
	glEnd();
	//右
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[3]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(19.0f, 19.0f, -19.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(19.0f, 19.0f, 19.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(19.0f, -19.0f, 19.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(19.0f, -19.0f, -19.0f);
	glEnd();
	//前
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[4]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-19.0f, 19.0f, 19.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-19.0f, -19.0f, 19.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(19.0f, -19.0f, 19.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(19.0f, 19.0f, 19.0f);
	glEnd();
	//后
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[5]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-19.0f, 19.0f, -19.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-19.0f, -19.0f, -19.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(19.0f, -19.0f, -19.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(19.0f, 19.0f, -19.0f);
	glEnd();
}

void DrawPoints()//两点一直线画基础形状
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	//	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.09f, 0.05f, 0.0f);
	glVertex3f(0.35f, 0.05f, 0.0f);
	glVertex3f(0.35f, 0.05f, 0.0f);
	glVertex3f(0.55f, 0.25f, 0.0f);
	glVertex3f(0.55f, 0.25f, 0.0f);
	glVertex3f(0.6f, 0.25f, 0.0f);
	glVertex3f(0.6f, 0.25f, 0.0f);
	glVertex3f(0.4f, 0.05f, 0.0f);
	glVertex3f(0.4f, 0.05f, 0.0f);
	glVertex3f(0.6f, 0.05f, 0.0f);
	glVertex3f(0.6f, 0.05f, 0.0f);
	glVertex3f(0.75f, 0.15f, 0.0f);
	glVertex3f(0.75f, 0.15f, 0.0f);
	glVertex3f(0.8f, 0.15f, 0.0f);
	glVertex3f(0.8f, 0.15f, 0.0f);
	glVertex3f(0.65f, 0.05f, 0.0f);
	glVertex3f(0.65f, 0.05f, 0.0f);
	glVertex3f(0.85f, 0.05f, 0.0f);
	glVertex3f(0.85f, 0.05f, 0.0f);
	glVertex3f(0.9f, 0.0f, 0.0f);
	glEnd();
}

void DrawWithColor()//颜色填充
{
	glBindTexture(GL_TEXTURE_2D, TexID[0]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.08f, 0.05f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.85f, 0.05f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.9f, 0.0f, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, TexID[0]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.35f, 0.05f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.55f, 0.25f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.6f, 0.25f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.4f, 0.05f, 0.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, TexID[0]);
	//	glColor3f(0.0f, 0.5f, 1.0f);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.6f, 0.05f, 0.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.75f, 0.15f, 0.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.8f, 0.15f, 0.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.65f, 0.05f, 0.0f);
	glEnd();
}

void fanzhuan() {//基础形状翻转
	glPushMatrix();
	glScalef(-1.0f, 1.0f, 1.0f);
	//DrawPoints();
	DrawWithColor();
	glPopMatrix();
}

void DrawSnow() {//画出一朵雪花
	//DrawPoints();
	DrawWithColor();
	int angle = 0;
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
		angle += 60;
		glRotatef(angle, 0, 0, 10);
		//DrawPoints();
		DrawWithColor();
		glPopMatrix();
	}

	int angle2 = 0;
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
		angle2 += 60;
		glRotatef(angle2, 0, 0, -10);
		//DrawPoints();
		fanzhuan();
		glPopMatrix();
	}
}

GLfloat rtri = 0.0f;
void DrawRotatedSnow() {//连续旋转
	rtri += 0.001f;
	glRotatef(rtri, 0.0f, 0.0f, 1.0f);
	//DrawRandom();
	DrawSnow();
	//	DrawDownSnow();
	glutPostRedisplay();

}

GLfloat down = 0.0f;
void DrawDownSnow() {//每朵雪花下降
	down -= 0.00000001f;
	glTranslated(0.0f, down, 0.0f);
	//	DrawSnow();
	DrawRotatedSnow();
	glutPostRedisplay();
}

void DrawRandom() {//随机生成雪花
	srand(2);
	//glMatrixMode(GL_MODELVIEW); //subsequent matrix operations are on the MODELVIEW matrix
	//glLoadIdentity();// load the identity matrix
	//glBindTexture(GL_TEXTURE_2D, TexID[0]);
	//glutSolidSphere(0.5, 40, 50);//球体

	//建立一个二次曲面对象用来绘制球体
	GLUquadricObj* quadObj1;
	quadObj1 = gluNewQuadric();
	//允许二次曲面对象生成纹理坐标
	gluQuadricTexture(quadObj1, GL_TRUE);
	//启动纹理
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, SkyBoxTexID[5]);
	//绘制球体,OpenGL内部会给你自动计算纹理坐标的
	gluSphere(quadObj1, 0.5, 50, 50);

	//glutSolidTeapot(1.0);//茶壶

	//cout << points.size() << endl;
	for (int i = 0; i < points.size(); i++) {
		//glLoadIdentity();
		float scale = (rand() % 10) / 10.f;
		float scalere = 1 / scale;
		//glScalef(scale, scale, scale);

		//float traslatex = (rand() % 10) / 3 - 1;
		//float traslatey = (rand() % 10) / 3 - 1;
		points[i].z = point_z;
		/*struct SnowPoint sp;
		sp.x = traslatex;
		sp.y = traslatey;
		sp.z = traslatez;
		points.push_back(sp);*/
		//cout << "point x:" << points[i].x << "y:" << points[i].y << "z:" << points[i].z << endl;
		glTranslatef(points[i].x, points[i].y, 0.0f);

		//float r = (rand() % 10) / 10.f;
		//float g = (rand() % 10) / 10.f;
		//float b = (rand() % 10) / 10.f;
		//glColor3f(r, g, b);

		DrawSnow();
		//DrawRotatedSnow();
		//DrawDownSnow();
		//glScalef(scalere, scalere, scalere);
		glTranslatef(-1 * points[i].x, -1 * points[i].y, 0.0f);

	}
}

void NormalKeys(unsigned char normalkey, int x, int y) {
	/*if (normalkey == 27) {
		exit(0);
	}*/
	switch (normalkey) {
	case 27:
		exit(0);
		break;
	case 'L':
		if (dirlight == 0) {
			glDisable(GL_LIGHT2);
			dirlight = 1;
		}
		else if (dirlight == 1) {
			glEnable(GL_LIGHT2);
			dirlight = 0;
		}
	}
}

GLfloat r = 0.0;

void RotateCameraY() {//相机绕y轴旋转

	//r += 0.01;

	r = fmod(r + 0.1, 360);

	double x = 10 * cos(r * 3.14 / 180.0);
	double z = 10 * sin(r * 3.14 / 180.0);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	point_z = -1 * z;
	//gluLookAt(x, 0.0, z, -1 * x, 0, -1 * z, 0.0, 1.0, 0.0);
	DrawCube();
	DrawRandom();
	glutPostRedisplay();
}



GLfloat r1 = 0.0;
void RotateLight() {//聚光灯和相机一起旋转，捆绑效果

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//r1 += 0.01;
	r1 = fmod(r1 + 0.1, 360);

	//glPushMatrix();
	a[0] = 8 * cos(r1 * 3.14 / 180.0);
	a[2] = 8 * sin(r1 * 3.14 / 180.0);
	if (r1 > 180)
		glNormal3f(0.0, 0.0, -1.0);//更改法向量，使得雪花在背面也能反射光
	if (r1 <= 180)
		glNormal3f(0.0, 0.0, 1.0);//连续变换
	glLightfv(GL_LIGHT1, GL_POSITION, a);
    spot_dir[0] = -10 * cos(r1 * 3.14 / 180.0);
	spot_dir[2] = -10 * sin(r1 * 3.14 / 180.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);

	//方向光模拟24小时太阳光


	if (r1 <= 180) {
		diffuseLight2[0] = fmod(diffuseLight2[0] + 0.1 / 180.0, 1.0);
		diffuseLight2[1] = fmod(diffuseLight2[1] + 0.1 / 180.0, 1.0);
		diffuseLight2[2] = fmod(diffuseLight2[2] + 0.1 / 180.0, 1.0);
		//cout << "diffuseLight2[0]" << diffuseLight2[0] << endl;
	}
	else if (r1 > 180) {
		diffuseLight2[0] = fmod(diffuseLight2[0] - 0.1 / 180.0, 1.0);
		diffuseLight2[1] = fmod(diffuseLight2[1] - 0.1 / 180.0, 1.0);
		diffuseLight2[2] = fmod(diffuseLight2[2] - 0.1 / 180.0, 1.0);
		//cout << "diffuseLight2[0]" << diffuseLight2[0] << endl;
	}
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, diffuseLight2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, diffuseLight2);

	//glPopMatrix();
	glutPostRedisplay();

}


void MouseClick(int button, int state, int x, int y) {
	if (state == GLUT_DOWN)
	{

		if (button == GLUT_RIGHT_BUTTON)//鼠标右键点击处绘制雪花
		{
			mouseflag = false;
			struct SnowPoint sp;
			sp.x = x / 96.0 - 5.0;
			sp.y = (701 - y) / 70.0 - 5.0;
			sp.z = 0.0;
			cout << "mouse x:" << x << "y:" << y << endl;
			cout << "snow x:" << sp.x << "y:" << sp.y << "z:" << sp.z << endl;
			points.push_back(sp);
			glFlush();

		}
		else if (button == GLUT_LEFT_BUTTON) {
			OriX = x, OriY = y;//记录鼠标左键按下位置
			mouseflag = true;
		}
	}

}

void onMouseMove(int x, int y)   //处理鼠标拖动
{
	if (mouseflag) {
		du += x - OriX; //鼠标在窗口x轴方向上的增量加到视点与x轴的夹角上，就可以左右转
		h += 0.03 * (y - OriY);  //鼠标在窗口y轴方向上的改变加到视点y的坐标上，就可以上下转
		if (h > 1.0)   h = 1.0;  //对视点y坐标作一些限制，不会使视点太奇怪
		else if (h < -1.0) h = -1.0;
		OriX = x, OriY = y;  //将此时的坐标作为旧值，为下一次计算增量做准备
		cout << "du:" << du << "h:" << h << "OriX:" << OriX << "OriY:" << OriY << endl;
	}
}

//void reshape(int w, int h)
//{
//	glViewport(0, 0, w, h);    //截图;1、2为视口的左下角;3、4为视口的宽度和高度
//	glMatrixMode(GL_PROJECTION);    //将当前矩阵指定为投影矩阵
//	glLoadIdentity();
//	gluPerspective(75.0, (float)w / h, 1.0, 1000.0); //1、视野在Y-Z平面的角度[0,180];2、投影平面宽度与高度的比率;3、近截剪面到视点的距离;4、远截剪面到视点的距离
//	glMatrixMode(GL_MODELVIEW);     //对模型视景矩阵堆栈应用随后的矩阵操作.
//}


void Draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* clear the color buffer,the buffers enabled for color
														writing*/

	glColor3f(1.0, 1.0, 1.0);// use white color to draw objects 
	glMatrixMode(GL_MODELVIEW); //subsequent matrix operations are on the MODELVIEW matrix
	glLoadIdentity();// load the identity matrix

	//DrawCube();
	//DrawRandom();

	gluLookAt(ry * cos(c * du), h, ry * sin(c * du), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);   //从视点看远点

	RotateCameraY();
	glutSwapBuffers();                                      //交换两个缓冲区指针

	RotateLight();

	glMatrixMode(GL_PROJECTION); //subsequent matrix operations are on the PROJECTION matrix
	glLoadIdentity();// load the identity matrix for the projection matrix first
	GLsizei width = glutGet(GLUT_WINDOW_WIDTH);
	GLsizei height = glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(fov, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glFlush();//force execution of GL commands
}

void SpecialKeys(GLint specialkey, int x, int y) {//利用翻页键控制相机视野
	switch (specialkey) {
	case GLUT_KEY_UP:
		fov -= 10.0;
		break;
	case  GLUT_KEY_DOWN:
		fov += 10.0;
		break;
	}
	if (fov > 180.0)
		fov = 0.0;
	if (fov < 0.0)
		fov = 180.0;
	Draw();
	cout << "fov" << fov << endl;
}

void MyIdle() {
	glutPostRedisplay();
}

void Reshape(int w, int h) {
	h = (h == 0) ? 1 : h;
	cout << w << " " << h << endl;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	/*视口的左下角为（0,0），宽高与窗口当前参数相
	同*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);
}

int main(int iArgc, char** cppArgv) {
	glutInit(&iArgc, cppArgv);//initialize glut
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);/*use single (as opposed to double) buffer and RGB (as apposed to INDEX) color*/
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(960, 720); // set window size, (width, height)
	glutInitWindowPosition(0, 0);  // set window position, (0,0) is the top left corner
	/*glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);*/
	glutCreateWindow("SNOW");//  name the window
	Initialize();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Draw);
	glutIdleFunc(Draw);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(NormalKeys);
	glutMouseFunc(MouseClick);
	glutMotionFunc(onMouseMove);
	//glutReshapeFunc(Reshape);
	glutMainLoop();//put the program in an infinite event loop
	return 0;
}

