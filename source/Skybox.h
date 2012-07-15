#ifndef SKYBOX_H
#define SKYBOX_H
#include <GL/gl.h>
#include "Image.h"
#include "Texture.h"
#include "Vector.h"

class Skybox
{
private:
	Texture textures[6];
public:
    Skybox()  { }

	Skybox(const Image &front, const Image &left, const Image &back, const Image &right, const Image &up, const Image &down)
	{
	    init(front, left, back, right, up, down);
	}

	void init(const Image &front, const Image &left, const Image &back, const Image &right, const Image &up, const Image &down);
	void render(const Vector3f &) const;
};

#endif
