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

double values[3][3][3] = { { { 0, 0, 0 },{ 0, 1, 0 },{ 0, 0, 0 } },{ { 1, 1, 1 },{ 1, 1, 1 },{ 1, 1, 1 } },{ { 0, 0, 0 },{ 0, 1, 0 },{ 0, 0, 0 } } };
//double values[3][3][3] = { { { 1, 1, 1 },{ 0, 0, 0 },{ 0, 0, 0 } },{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } },{ { 0, 0, 0 },{ 0, 0, 0 },{ 0, 0, 0 } } };
XYZ points[3][3][3];

int main(int argc, char **argv)
{
	// Initialize XYZ for each point.
	for (int x = 0; x <= 2; x++) {
		for (int y = 0; y <= 2; y++) {
			for (int z = 0; z <= 2; z++) {
				XYZ p;
				p.x = x / 4.0;
				p.y = y / 4.0;
				p.z = z / 4.0;
				points[x][y][z] = p;
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

	// enter GLUT event processing cycle
	glutMainLoop();
}

float angle = 0.0f;

void renderScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	gluLookAt(0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -20.0f,
		0.0f, 1.0f, 0.0f);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);

	// Get triangles and paint them
	Poligonizer poli;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				TRIANGLE triangles[5];
				GRIDCELL cell;
				cell.p[0] = points[x][y][z];
				cell.p[1] = points[x][y + 1][z];
				cell.p[2] = points[x][y + 1][z + 1];
				cell.p[3] = points[x][z][y + 1];
				cell.p[4] = points[x + 1][y][z];
				cell.p[5] = points[x + 1][y + 1][z];
				cell.p[6] = points[x + 1][y + 1][z + 1];
				cell.p[7] = points[x + 1][z][y + 1];

				cell.val[0] = values[x][y][z];
				cell.val[1] = values[x][y + 1][z];
				cell.val[2] = values[x][y + 1][z + 1];
				cell.val[3] = values[x][z][y + 1];
				cell.val[4] = values[x + 1][y][z];
				cell.val[5] = values[x + 1][y + 1][z];
				cell.val[6] = values[x + 1][y + 1][z + 1];
				cell.val[7] = values[x + 1][z][y + 1];

				int amount = poli.Polygonize(cell, 0.5, triangles);

				for (int ntriangle = 0; ntriangle < amount; ntriangle++)
				{
					paintTriangle(triangles[ntriangle]);
				}
			}
		}
	}

	angle += 0.2f;

	glutSwapBuffers();
}

void paintTriangle(TRIANGLE triangle)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(triangle.p[0].x, triangle.p[0].y, triangle.p[0].z);
	glVertex3f(triangle.p[1].x, triangle.p[1].y, triangle.p[1].z);
	glVertex3f(triangle.p[2].x, triangle.p[2].y, triangle.p[2].z);
	glEnd();
}

