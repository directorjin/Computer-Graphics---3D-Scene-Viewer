#include <stdlib.h>
#include <gl/glut.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <cmath> //power를 쓰기위함.
#include <math.h>
#define Pi 3.141592
using namespace std;

//전체각도
double AngleX = 0;
double AngleY = 0;
double AngleZ = 0;
double Angle = 0.0;
int start = 0; //0일때 뗀상태 안클릭한상태 1일때 클릭한상태
int k = -50;
int count_k = 0;

double xi = 0;
double yi = 0;
float Axis[3] = { 1.0, 0.0, 0.0 };

int Width = 800, Height = 600; //크기 만듬

float RotMat[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

struct Vertex
{
	double P[3];
	double N[3];
};
struct Face
{
	int vIdx[3];
};
struct Object
{
	vector<Vertex> VertList;
	vector<Face> FaceList;
};
Object MyModel;
void Cross(double out[3], double a[3], double b[3])
{
	out[0] = a[1] * b[2] - a[2] * b[1];
	out[1] = a[2] * b[0] - a[0] * b[2];
	out[2] = a[0] * b[1] - a[1] * b[0];
}
void Sub(double out[3], double a[3], double b[3])
{
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}
void Add(double out[3], double a[3], double b[3])
{
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}
void LoadModel(char *fname)
{
	FILE *fp;

	fopen_s(&(fp), fname, "r");

	int vnum, fnum;
	fscanf_s(fp, "%d", &vnum);
	fscanf_s(fp, "%d", &fnum);

	for (int i = 0; i < vnum; ++i)
	{
		Vertex v;
		fscanf_s(fp, "%lf%lf%lf", &v.P[0], &v.P[1], &v.P[2]);
		v.N[0] = v.N[1] = v.N[2] = 0.0;
		MyModel.VertList.push_back(v);
	}

	for (int i = 0; i < fnum; ++i)
	{
		int vidx0, vidx1, vidx2;
		fscanf_s(fp, "%d%d%d", &vidx0, &vidx1, &vidx2);

		Face f;
		f.vIdx[0] = vidx0;
		f.vIdx[1] = vidx1;
		f.vIdx[2] = vidx2;
		MyModel.FaceList.push_back(f);

		double e1[3], e2[3], n[3];
		double *p0 = MyModel.VertList[vidx0].P;
		double *p1 = MyModel.VertList[vidx1].P;
		double *p2 = MyModel.VertList[vidx2].P;
		Sub(e1, p1, p0);
		Sub(e2, p2, p0);
		Cross(n, e1, e2);

		double *n0 = MyModel.VertList[vidx0].N;
		double *n1 = MyModel.VertList[vidx1].N;
		double *n2 = MyModel.VertList[vidx2].N;
		Add(n0, n0, n);
		Add(n1, n1, n);
		Add(n2, n2, n);
	}
	for (int i = 0; i < vnum; ++i)
	{
		double *n = MyModel.VertList[i].N;
		double norm = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
		n[0] /= norm;
		n[1] /= norm;
		n[2] /= norm;
	}

}
void RenderModleAsPoint()
{
	glDisable(GL_LIGHTING);

	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POINTS);
	for (int i = 0; i < MyModel.VertList.size(); ++i)
	{
		glVertex3dv(MyModel.VertList[i].P);
	}
	glEnd();

	glEnable(GL_LIGHTING);

}

void RenderModelAsWire()
{
	glDisable(GL_LIGHTING);

	glColor3f(1.0, 1.0, 1.0);

	for (int i = 0; i < MyModel.FaceList.size(); ++i)
	{
		int idx0, idx1, idx2;
		idx0 = MyModel.FaceList[i].vIdx[0];
		idx1 = MyModel.FaceList[i].vIdx[1];
		idx2 = MyModel.FaceList[i].vIdx[2];

		glBegin(GL_LINE_LOOP);
		glVertex3dv(MyModel.VertList[idx0].P);
		glVertex3dv(MyModel.VertList[idx1].P);
		glVertex3dv(MyModel.VertList[idx2].P);
		glEnd();
	}

	glEnable(GL_LIGHTING);
}

void RenderModelAsTri()
{
	glEnable(GL_LIGHTING);
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < MyModel.FaceList.size(); ++i)
	{
		int idx0, idx1, idx2;
		idx0 = MyModel.FaceList[i].vIdx[0];
		idx1 = MyModel.FaceList[i].vIdx[1];
		idx2 = MyModel.FaceList[i].vIdx[2];

		glNormal3dv(MyModel.VertList[idx0].N);
		glVertex3dv(MyModel.VertList[idx0].P);

		glNormal3dv(MyModel.VertList[idx1].N);
		glVertex3dv(MyModel.VertList[idx1].P);

		glNormal3dv(MyModel.VertList[idx2].N);
		glVertex3dv(MyModel.VertList[idx2].P);
	}
	glEnd();
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == '1')
	{
		count_k = 1;
	}
	else if (key == '2')
	{
		count_k = 2;
	}
	else if (key == '3')
	{
		count_k = 3;
	}

	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
	{
		AngleX = AngleX + 1;
	}

	if (key == GLUT_KEY_DOWN)
	{
		AngleX = AngleX - 1;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		AngleY = AngleY + 1;
	}
	if (key == GLUT_KEY_LEFT)
	{
		AngleY = AngleY - 1;
	}


	glutPostRedisplay();


}


void MyMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		start = 1;
		xi = x;
		yi = Height - y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		start = 0;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		start = 2;
		xi = x;
		yi = Height - y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		start = 0;
	}

}
void MoveMouse(int x, int y)
{
	float y1 = Height - y;
	float x1 = x;
	float zi = 0;
	float z1 = 0;
	if (start == 1)
	{
		xi = (2.0 * xi - Width) / Width;
		yi = -((-2.0 * yi + Height) / Height);
		float r = xi * xi + yi * yi;
		if (r >= 1.0)
		{
			xi = xi / sqrt(r);
			yi = yi / sqrt(r);
			zi = 0.0;
		}
		else 
		{
			zi = sqrt(1.0 - r);
		}
		//처음위치


		x1 = (2.0 * x1 - Width) / Width;
		y1 = -((-2.0 * y1 + Height) / Height);
		float r1 = x1 * x1 + y1 * y1;
		if (r1 >= 1.0)
		{
			x1 = x1 / sqrt(r1);
			y1 = y1 / sqrt(r1);
			z1 = 0.0; //중요
		}
		else
		{
			z1 = sqrt(1.0 - r1);
		}
		//변하는 값


		//정의
		Angle = (acos(xi * x1 + yi * y1 + zi * z1) * 180.0f / 3.141592f);//내적은 각도
		/*
		Axis[0] = 0; //테스트용
		Axis[1] = 0;
		Axis[2] = 1;
		*/

		
		Axis[0] = (yi * z1 - zi * y1);
		Axis[1] = (zi * x1 - xi * z1);
		Axis[2] = (xi * y1 - yi * x1);
		
		
		

		//외적은 axis
		xi = x;
		xi = y;

	}
	else if (start == 2)
	{
		Angle = 0;
		Axis[0] = 0;
		Axis[1] = 0;
		Axis[2] = 0;
		int y1 = ((Height - y) - yi) / 2;
		k = k + y1;

	}
	xi = x;
	yi = Height - y;

	glutPostRedisplay();
}


void glutSolidSphere(int radius, int slices, int stacks)
{
	;
}


void SetUpViewTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, k); // 0으로 설정시에는 바로 눈 앞에 있으므로 보이지 않음 -10으로 생성해줘야함 (?)
	glRotatef(Angle, Axis[0], Axis[1], Axis[2]);
	glMultMatrixf(RotMat);
	glGetFloatv(GL_MODELVIEW_MATRIX, RotMat);
	RotMat[12] = RotMat[13] = RotMat[14] = 0.0;

}

void SetupViewVolume()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(30.0, (double)Width / (double)Height, 0.1, 100.0);

}
void Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //검은색


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();


	SetUpViewTransform();
	SetupViewVolume();
	glMatrixMode(GL_MODELVIEW);

	if (count_k == 1)
	{
		RenderModleAsPoint();
	}
	else if (count_k == 2)
	{
		RenderModelAsWire();
	}
	else if (count_k == 3)
	{
		RenderModelAsTri();
	}
	glDisable(GL_LIGHTING);

	for (int k = -10; k <= 10; k++)
	{
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_LINES);
		glVertex3f(k, -0.0f, 10);
		glVertex3f(k, -0.0f, -10);
		glEnd();
	}
	for (int k = -10; k <= 10; k++)
	{
		glColor3f(0.5f, 0.5f, 0.5f); //색지정 오류
		glBegin(GL_LINES);
		glVertex3f(10, 0, k);
		glVertex3f(-10, 0, k);
		glEnd();
	}
	glutSwapBuffers();

}


int main(int argc, char **argv)
{



	glutInit(&argc, argv);
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutCreateWindow("문제");
	LoadModel("bunny.txt");

	glEnable(GL_DEPTH_TEST);

	// Setup lights
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float pos0[4] = { 10.0, 10.0, 10.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);

	glutMouseFunc(MyMouse);
	glutMotionFunc(MoveMouse);
	glutDisplayFunc(Render);
	glutSpecialFunc(SpecialKeyboard);
	glutKeyboardFunc(keyboard);


	glutMainLoop();

	return 0;
}
