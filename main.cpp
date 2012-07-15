#include <SDL/SDL.h>
#include <GL/glu.h>
#include <set>
#include <algorithm>
#include "Image.h"
#include "Camera.h"
#include "Skybox.h"
#include "LSystem.h"
#include "ParticleSystem.h"
#include "Animation.h"
#include "Logger.h"
#include "Settings.h"
#include "Texture.h"
#include "Utils.h"
#include "Mesh.h"

#define ANIMATION

const int FPS = 30; //TODO: fix FPS problems
Texture grass, snow_tex;
Camera cam, oldcam;
Skybox box;
//Plant plant;
//Fountain fountain;
//Fireworks fireworks;
Snow snow;
#ifdef ANIMATION
Animation animation;
#else
Mesh mesh;
#endif
bool cam_saved = false, snow_falling = true;
const int WIDTH = 700, HEIGHT = 550, BPP = 16;
const std::string CONFIG = "settings.config";
float zoom = 1.0;
std::set<SDLKey> pressed_keys;

GLfloat fog_color[] = { 0.5, 0.5, 0.5, 1.0 };
bool fog_enabled = false, fog_held = false;

void cleanUp()
{
	delete skeletonManagerPtr;
    delete loggerPtr;
	delete settingsPtr;
    delete textureManagerPtr;
}

void setPerspective()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0 * zoom, 1, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void init()
{
    new Logger;
	new Settings;
	ERR_CODE ret = settings.load(CONFIG);

    if (ret != NONE)
    {
		logger.logln(LOG_USER | LOG_FILE, "An error occurred while loading the default settings file (" + CONFIG + "): " + errString(ret));
		delete loggerPtr;
		delete settingsPtr;
		exit(0);
	}

    new TextureManager;
	new SkeletonManager;
	atexit(cleanUp);
    srand(time(0));
    //plant.parse();
	Skeleton *skel = new Skeleton;
	ret = skel->load("HumanSkel");

	if (ret != NONE)
	{
		delete skel;
		die("An error occurred while loading skeleton file: " + errString(ret));
	}

	ret = skel->add_animation("idle");

	if (ret != NONE)
	{
		delete skel;
		die("An error occurred while loading idle animation file: " + errString(ret));
	}

	skeletonManager.add_skeleton(skel);
#ifdef ANIMATION
	ret = animation.link_skeleton("HumanSkel");

	if (ret != NONE)
		die("An error occurred while linking skeleton to animation: " + errString(ret));

	ret = animation.animate("idle", true);

	if (ret != NONE)
	die("An error occurred in starting idle animation: " + errString(ret));
#else
	ret = mesh.load("human.mesh");

	if (ret != NONE)
		die("An error occurred while loading mesh file: " + errString(ret));

	ret = mesh.animate("idle", true);

	if (ret != NONE)
		die("An error occurred while animating mesh: " + errString(ret));
#endif

    cam.advance(-15.0);
    setPerspective();
    glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fog_color);
	glFogf(GL_FOG_DENSITY, 0.25f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 5.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    Image front, back, right, left, up, down;
#define CHECK_RET_NAME(x, name) if ((ret = x.load(name ".tga")) != NONE) die("Error while loading " name ".tga: " + errString(ret));
#define CHECK_RET(x) CHECK_RET_NAME(x, #x)
    CHECK_RET(front);
    CHECK_RET(back);
    CHECK_RET(right);
    CHECK_RET(left);
    CHECK_RET(up);
    CHECK_RET(down);

    Image img_grass, img_snow;
    CHECK_RET_NAME(img_grass, "grass");
	CHECK_RET_NAME(img_snow, "snow");
    grass = textureManager.load(img_grass);
	snow_tex = textureManager.load(img_snow);
    box.init(front, left, back, right, up, down);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cam.orient_before();
    box.render(Vector3f(5.0, 5.0, 5.0));
    cam.orient();
    textureManager.bind(grass);
    /*glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000.0, -1.0, -1000.0);
    glTexCoord2f(0.0, 200.0);
    glVertex3f(-1000.0, -1.0, 1000.0);
    glTexCoord2f(200.0, 200.0);
    glVertex3f(1000.0, -1.0, 1000.0);
    glTexCoord2f(200.0, 0.0);
    glVertex3f(1000.0, -1.0, -1000.0);
    glEnd();*/
    glPushMatrix();
	glPushMatrix();
    glTranslatef(0.0, -1.0, -5.0);
    //plant.render();
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_TEXTURE_2D);
	//glTranslatef(-3.0, 0.0, 0.0);
	//fireworks.render();
	//glTranslatef(6.0, 0.0, 0.0);
	//fountain.render();
	//glTranslatef(4.0, 0.0, 0.0);
	glColor3f(1.0, 0.6, 0.3);
	glDisable(GL_TEXTURE_2D);
glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
#ifdef ANIMATION
 animation.render();
#else
mesh.render();
#endif
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glPopAttrib();
	glPopMatrix();

	if (snow_falling)
	{
		glScalef(50.0, 50.0, 50.0);
		textureManager.bind(snow_tex);
		snow.render();
	}

	glPopMatrix();
    glFlush();
	SDL_GL_SwapBuffers();
}

void keys()
{
    bool p = false;
	std::set<SDLKey>::iterator end = pressed_keys.end();
#define IF_DOWN(key) if (pressed_keys.find(key) != end)
	IF_DOWN(SDLK_ESCAPE)
		exit(0);
    IF_DOWN(SDLK_a)
        cam.rotateY(-2.5);
	else IF_DOWN(SDLK_d) //TODO: most recent
		cam.rotateY(2.5);
	IF_DOWN(SDLK_UP)
        cam.rotateX(-2.5);
    else IF_DOWN(SDLK_DOWN) //TODO: most recent
		cam.rotateX(2.5);
	//IF_DOWN(SDLK_z)
	//		cam.strafe(-0.5);
	//else IF_DOWN(SDLK_x)
	//1cam.strafe(0.5);
	IF_DOWN(SDLK_t)
	{
        if (zoom > 0.4)
            zoom -= 0.02;
        p = true;
	}
    IF_DOWN(SDLK_g)
	{
        if (zoom < 1.6)
            zoom += 0.02;
        p = true;
	}
	IF_DOWN(SDLK_v)
	{
		if (!fog_held)
		{
			fog_held = true;
			if ((fog_enabled = !fog_enabled))
				glEnable(GL_FOG);
			else
				glDisable(GL_FOG);
		}
	}
	else
		fog_held = false;
	/*IF_DOWN(SDLK_f)
		if (fireworks.dead())
			fireworks.init();
	IF_DOWN(SDLK_p)
		if ((snow_falling = !snow_falling))
		snow.init();*/
	IF_DOWN(SDLK_w)
		cam.advance(0.5);
	IF_DOWN(SDLK_s)
		cam.advance(-0.5);

    if (p)
        setPerspective();
}

void update()
{
	/*fireworks.update();
	fountain.update();

	if (snow_falling)
	snow.update();*/
#ifdef ANIMATION
	animation.update();
#else
	mesh.update();
#endif
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    atexit(SDL_Quit);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(WIDTH, HEIGHT, BPP, SDL_OPENGL);
    SDL_WM_SetCaption("3D Engine", "3D Engine");
    std::string path = argv[0];
    std::replace(path.begin(), path.end(), '\\', '/');
    set_path(getpath(path));
    init();
	SDL_Event event;
	bool done = false, first = true;
	unsigned int start = SDL_GetTicks();

	while (!done)
	{
		while (SDL_PollEvent(&event))
        {
			if (event.type == SDL_QUIT)
				done = true;
			else if (event.type == SDL_KEYDOWN)
				pressed_keys.insert(event.key.keysym.sym);
			else if (event.type == SDL_KEYUP)
				pressed_keys.erase(event.key.keysym.sym);
		}

		if (done)
			break;

		keys();
		update();
		render();

		if (first)
		{
			first = false;
			start = SDL_GetTicks();
		}

		unsigned int ticks = SDL_GetTicks() - start;

		if (ticks < 1000.0 / FPS)
		{
			SDL_Delay((1000.0 / FPS) - ticks);
			start = SDL_GetTicks();
		}

		else
		{
			std::cerr << "FPS too high" << std::endl;
			return 1;
		}
	}

    return 0;
}
