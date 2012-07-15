#ifndef MODEL_H
#define MODEL_H
#include "Error.h"
#include "Vector.h"
#include "Point.h"
#include "Utils.h"
#include "Texture.h"
#include <vector>
#include <cstring>
#include <map>

struct Face
{
	int v1, v2, v3, t1, t2, t3, n1, n2, n3;

	Face() : v1(-1), v2(-1), v3(-1), t1(-1), t2(-1), t3(-1), n1(-1), n2(-1), n3(-1)
	{
	}

	Face(int vertex1, int vertex2, int vertex3, int tex1, int tex2, int tex3, int normal1, int normal2, int normal3) :
		v1(vertex1 - 1), v2(vertex2 - 1), v3(vertex3 - 1), t1(tex1 - 1), t2(tex2 - 1), t3(tex3 - 1),
		n1(normal1 - 1), n2(normal2 - 1), n3(normal3 - 1)
	{
	}
};

typedef unsigned char byte;
typedef unsigned short word;

struct MS3DHeader
{
	char id[10];
	int version;
};

struct MS3DVertex
{
    byte flags;
	float vertex[3];
	char boneId;
	byte referenceCount;
};

struct MS3DTriangle
{
	word flags;
	word vertexIndices[3];
	float vertexNormals[3][3];
	float s[3];
	float t[3];
	byte smoothingGroup;
	byte groupIndex;
};

struct MS3DGroup
{
	byte flags;
	char name[32];
	word numtriangles;
	word *triangleIndices;
	char materialIndex;

	MS3DGroup() : triangleIndices(0)
	{
	}

	~MS3DGroup()
	{
		delete[] triangleIndices;
	}
};

struct MS3DMaterial
{
	char name[32];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emissive[4];
    float shininess;
    float transparency;
    char mode;
    char texture[128];
    char alphamap[128];
};

class Model
{
public:
	virtual ERR_RET load(const std::string &filename) = 0;
	virtual void render() const = 0;
};

class OBJModel : public Model
{
private:
	std::vector<Point3f> vertices;
	std::vector<Point2f> tex_coords;
	std::vector<Vector3f> normals;
	std::vector<Face> faces;
public:
	ERR_RET load(const std::string &filename); //TODO: support textures
	void render() const;
};

class MS3DModel : public Model
{
private:
	MS3DGroup *groups;
	std::pair<MS3DMaterial, Texture> *materials;
	MS3DTriangle *triangles;
	MS3DVertex *vertices;
	word num_groups, num_materials, num_triangles, num_vertices, num_joints;
	float fps, time;
	int num_frames;
public:
	MS3DModel() : groups(0), materials(0), triangles(0), vertices(0), num_groups(0), num_materials(0),
				  num_triangles(0), num_vertices(0), fps(0), time(0)
	{
	}

	ERR_RET load(const std::string &filename);
	void render() const;
};

#endif
