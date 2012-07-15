#include "Mesh.h"

Mesh::~Mesh()
{
	delete[] vertices;
	delete[] faces;
}

ERR_CODE Mesh::load(const std::string &filename)
{
	FILE *file = file_open("Meshes/" + filename, "rb");

	if (!file)
	{
		badfile = "Meshes/" + filename;
		return FILE_NOT_FOUND;
	}

	char buffer[SKEL_NAME_MAX_LEN];
	fread(buffer, 1, SKEL_NAME_MAX_LEN, file);
	ERR_CODE ret = animation.link_skeleton(buffer);

	if (ret != NONE)
		return ret;

	fread(&num_verts, 4, 1, file);
	fread(&num_faces, 4, 1, file);
	vertices = new Vertex[num_verts];
	faces = new Face[num_faces];

	for (unsigned int i = 0; i < num_verts; ++i)
	{
		fread(&vertices[i].x, 4, 1, file);
		fread(&vertices[i].y, 4, 1, file);
		fread(&vertices[i].z, 4, 1, file);
		fread(vertices[i].boneIDs, 4, MAX_INFLUENCES, file);
		fread(vertices[i].weights, 4, MAX_INFLUENCES, file); //TODO: ensure that abs(sum(weights) - 1.0) < OFFSET
	}

	for (unsigned int j = 0; j < num_faces; ++j)
	{
		fread(&faces[j].v1, 4, 1, file);
		fread(&faces[j].v2, 4, 1, file);
		fread(&faces[j].v3, 4, 1, file);
	}

	return NONE;
}

void Mesh::update()
{
	BoneData data;
	float mat[16];
	float x, y, z;
	glPushMatrix();
	animation.update();

	for (unsigned int i = 0; i < num_verts; ++i)
	{
		vertices[i].anim_x = 0.0;
		vertices[i].anim_y = 0.0;
		vertices[i].anim_z = 0.0;

		for (int j = 0; j < MAX_INFLUENCES; ++j)
		{
			if (vertices[i].weights[j] < WEIGHT_OFFSET)
				break;

			data = animation.get_bone_data(vertices[i].boneIDs[j]);
			glLoadIdentity();
			glTranslatef(data.abs_pos.getX(), data.abs_pos.getY(), data.abs_pos.getZ());
			mult_quat(data.abs_quat);
			glMultMatrixf(data.inv_orientation);
			glGetFloatv(GL_MODELVIEW_MATRIX, mat);
			x = vertices[i].x;
			y = vertices[i].y;
			z = vertices[i].z;
			vertices[i].anim_x += (mat[0] * x + mat[4] * y + mat[8] * z + mat[12]) * vertices[i].weights[j];
			vertices[i].anim_y += (mat[1] * x + mat[5] * y + mat[9] * z + mat[13]) * vertices[i].weights[j];
			vertices[i].anim_z += (mat[2] * x + mat[6] * y + mat[10] * z + mat[14]) * vertices[i].weights[j];
		}
	}

	glPopMatrix();
}

void Mesh::render() const
{
	glBegin(GL_TRIANGLES);

	for (unsigned int i = 0; i < num_faces; ++i)
	{
		glVertex3f(vertices[faces[i].v1].anim_x, vertices[faces[i].v1].anim_y, vertices[faces[i].v1].anim_z);
		glVertex3f(vertices[faces[i].v2].anim_x, vertices[faces[i].v2].anim_y, vertices[faces[i].v2].anim_z);
		glVertex3f(vertices[faces[i].v3].anim_x, vertices[faces[i].v3].anim_y, vertices[faces[i].v3].anim_z);
	}

	glEnd();
}
