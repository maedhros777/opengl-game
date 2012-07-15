#ifndef MESH_H
#define MESH_H

#include "Animation.h"

const int MAX_INFLUENCES = 4;
const int SKEL_NAME_MAX_LEN = 32;
const float WEIGHT_OFFSET = 0.00001;

struct Vertex
{
	float x, y, z, anim_x, anim_y, anim_z;
	unsigned int boneIDs[MAX_INFLUENCES];
	float weights[MAX_INFLUENCES];
};

struct Face
{
	unsigned int v1, v2, v3;
};

class Mesh
{
private:
	unsigned int num_verts, num_faces;
	Vertex *vertices;
	Face *faces;
	Animation animation;
public:
	Mesh() : num_verts(0), num_faces(0), vertices(0), faces(0)
	{
	}

	ERR_RET animate(const std::string &anim_name, bool anim_loop = false)
	{
		return animation.animate(anim_name, anim_loop);
	}

	~Mesh();
	void update();
	void render() const;
	ERR_RET load(const std::string &filename);
};

#endif
