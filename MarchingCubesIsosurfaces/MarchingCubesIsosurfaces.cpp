// MarchingCubesIsosurfaces.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <tchar.h>
#include "Poligonizer.h"
#include <GL/glut.h>

using namespace std;

void paintTriangle(TRIANGLE triangle);
void renderScene();
void processSpecialKeys(int key, int x, int y);

double const width = 1;
double const height = 1;
double const depth = 1;
double const gridSize = 50;
double values[50][50][50];
XYZ points[50][50][50];
double const isosurfaceValue = 30;

int main(int argc, char **argv)
{
	// Initialize XYZ and value for each point.
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y <= gridSize; y++) {
			for (int z = 0; z <= gridSize; z++) {
				//	Initialize XYZ
				XYZ p;
				p.x = x * width / gridSize;
				p.y = y * height / gridSize;
				p.z = z * depth / gridSize;
				points[x][y][z] = p;

				// Set value.
				values[x][y][z] = x*y;
			}
		}
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Isosurfaces");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutSpecialFunc(processSpecialKeys);

	// enter GLUT event processing cycle
	glutMainLoop();
}

float xAngle = 0.0f;
float yAngle = 0.0f;

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -20.0f,
		0.0f, 1.0f, 0.0f);

	glRotatef(xAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yAngle, 0.0f, 1.0f, 0.0f);

	// Get triangles and paint them
	Poligonizer poli;
	for (int x = 0; x < gridSize; x++) {
		for (int y = 0; y < gridSize; y++) {
			for (int z = 0; z < gridSize; z++) {
				TRIANGLE triangles[5];
				GRIDCELL cell;
				cell.p[0] = points[x][y][z];
				cell.p[1] = points[x][y + 1][z];
				cell.p[2] = points[x][y + 1][z + 1];
				cell.p[3] = points[x][y][z + 1];
				cell.p[4] = points[x + 1][y][z];
				cell.p[5] = points[x + 1][y + 1][z];
				cell.p[6] = points[x + 1][y + 1][z + 1];
				cell.p[7] = points[x + 1][y][z + 1];

				cell.val[0] = values[x][y][z];
				cell.val[1] = values[x][y + 1][z];
				cell.val[2] = values[x][y + 1][z + 1];
				cell.val[3] = values[x][y][z + 1];
				cell.val[4] = values[x + 1][y][z];
				cell.val[5] = values[x + 1][y + 1][z];
				cell.val[6] = values[x + 1][y + 1][z + 1];
				cell.val[7] = values[x + 1][y][z + 1];

				int amount = poli.Polygonize(cell, isosurfaceValue, triangles);

				for (int ntriangle = 0; ntriangle < amount; ntriangle++)
				{
					paintTriangle(triangles[ntriangle]);
				}
			}
		}
	}

	glutSwapBuffers();
}

void paintTriangle(TRIANGLE triangle)
{
	GLfloat x2, y2, z2;
	x2 = (triangle.p[0].x + triangle.p[1].x + triangle.p[2].x) / 3.0 / width;
	y2 = (triangle.p[0].y + triangle.p[1].y + triangle.p[2].y) / 3.0 / height;
	z2 = (triangle.p[0].z + triangle.p[1].z + triangle.p[2].z) / 3.0 / depth;

	glBegin(GL_TRIANGLES);
	glColor3f(x2, y2, z2);
	glVertex3f(triangle.p[0].x - width / 2, triangle.p[0].y - height / 2, triangle.p[0].z - depth / 2);
	glVertex3f(triangle.p[1].x - width / 2, triangle.p[1].y - height / 2, triangle.p[1].z - depth / 2);
	glVertex3f(triangle.p[2].x - width / 2, triangle.p[2].y - height / 2, triangle.p[2].z - depth / 2);
	glEnd();
}

void processSpecialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_LEFT: yAngle -= 1; break;
	case GLUT_KEY_RIGHT: yAngle += 1; break;
	case GLUT_KEY_UP: xAngle += 1; break;
	case GLUT_KEY_DOWN: xAngle -= 1; break;
	}
}

