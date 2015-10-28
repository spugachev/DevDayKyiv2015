#include <stdbool.h>
#include <SDL.h>
#include "SDL_opengl.h"

#ifdef main
#undef main
#endif

#ifdef EMSCRIPTEN
#include <math.h>
void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble znearVal, GLdouble zfarVal)
{
	GLdouble xmin, xmax, ymin, ymax;
	ymax = znearVal * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;
	glFrustum(xmin, xmax, ymin, ymax, znearVal, zfarVal);
}
#endif

float width = 800, height = 800;
float bpp = 0;
float nearVal = 10.0, farVal = 100000.0, fovy = 45.0;
float position[3] = { 0,0,-40 };
const float triangle[9] = {
	0,  10, 0,  // top point
	-10, -10, 0,  // bottom left
	10, -10, 0   // bottom right
};
float rotate_degrees = 90;
float rotate_axis[3] = { 0,1,0 };

SDL_Surface * screen = NULL;

void setup()
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = SDL_SetVideoMode(
		width, height, bpp,
		SDL_ANYFORMAT | SDL_OPENGL);

	glViewport(0, 0, width, height);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(fovy, width / height, nearVal, farVal);

	glMatrixMode(GL_MODELVIEW);
}

void render()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		if (event.type == SDL_KEYDOWN ||
			event.type == SDL_KEYUP)
			SDL_Quit();

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();

	glTranslatef(position[0], position[1], position[2]);

	{
		static Uint32 last = 0;

		Uint32    now = SDL_GetTicks();
		float   delta = (now - last) / 1000.0f; // in seconds
		last = now;

		static float angle = 0;
		angle += rotate_degrees * delta;

		// c modulo operator only supports ints as arguments
#define MOD( n, d ) (n - (d * (int) ( n / d )))
		angle = MOD(angle, 360);

		glRotatef(angle, rotate_axis[0], rotate_axis[1], rotate_axis[2]);
	}

	glBegin(GL_TRIANGLES);
	for (int i = 0; i <= 6; i += 3)
	{
		glColor3f(i == 0, i == 3, i == 6); // adding some color
		glVertex3fv(&triangle[i]);
	}
	glEnd();

	SDL_GL_SwapBuffers();
}

#ifdef EMSCRIPTEN
#include <emscripten.h>
#endif

int main()
{
	setup();
#ifdef EMSCRIPTEN
	emscripten_set_main_loop(render, 60, true);
#else
	while (true) {
		render();
	}
#endif
}