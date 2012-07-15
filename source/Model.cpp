#include "Model.h"

ERR_CODE OBJModel::load(const std::string &filename)
{
	int ret;
	char buffer[50], str[10];
	float x, y, z;
	int v1, v2, v3, t1, t2, t3, n1, n2, n3;
	FILE *file = file_open("Models/" + filename, "r"); //TODO: change all file loading from C-style to C++-style

	if (!file)
	{
		badfile = "Models/" + filename;
		return FILE_NOT_FOUND;
	}

	while (fgets(buffer, 50, file))
	{
		ret = sscanf(buffer, "%s %f %f %f", str, &x, &y, &z);

		if (strcmp(str, "v") == 0 && ret == 4)
			vertices.push_back(Point3f(x, y, z));

		else if (strcmp(str, "vt") == 0 && ret == 3)
			tex_coords.push_back(Point2f(x, y));

		else if (strcmp(str, "vn") == 0 && ret == 4)
			normals.push_back(Vector3f(x, y, z));

		else if (strcmp(str, "f") == 0 && ret == 4)
			faces.push_back(Face(x, y, z, 0, 0, 0, 0, 0, 0));

		else if (sscanf(buffer, "f %d/%d %d/%d %d/%d", &v1, &v2, &v3, &t1, &t2, &t3) == 6)
			faces.push_back(Face(v1, v2, v3, t1, t2, t3, 0, 0, 0));

		else if (sscanf(buffer, "f %d//%d %d//%d %d//%d", &v1, &v2, &v3, &n1, &n2, &n3) == 6)
			faces.push_back(Face(v1, v2, v3, 0, 0, 0, n1, n2, n3));

		else if (sscanf(buffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &v2, &v3, &t1, &t2, &t3, &n1, &n2, &n3) == 9)
			faces.push_back(Face(v1, v2, v3, t1, t2, t3, n1, n2, n3));
	}

	fclose(file);
	return NONE;
}

void OBJModel::render() const
{
	glBegin(GL_TRIANGLES);
	Face face;

	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		face = faces[i];

		if (face.t1 != -1)
			glTexCoord2f(tex_coords[face.t1].getX(), tex_coords[face.t1].getY());
		if (face.n1 != -1)
			normal3f(normals[face.n1]);
		vertex3f(vertices[face.v1]); //TODO: handle out-of-bounds faces in loading code

		if (face.t2 != -1)
			glTexCoord2f(tex_coords[face.t2].getX(), tex_coords[face.t2].getY());
		if (face.n2 != -1)
			normal3f(normals[face.n2]);
		vertex3f(vertices[face.v2]);

		if (face.t3 != -1)
			glTexCoord2f(tex_coords[face.t3].getX(), tex_coords[face.t3].getY());
		if (face.n3 != -1)
			normal3f(normals[face.n3]);
		vertex3f(vertices[face.v3]);
	}

	glEnd();
}

ERR_CODE MS3DModel::load(const std::string &filename)
{
	FILE *file = file_open("Models/" + filename, "rb");
	word i;

	if (!file)
	{
		badfile = "Models/" + filename;
		return FILE_NOT_FOUND;
	}

	MS3DHeader header;
	fread(header.id, 1, 10, file);
	fread(&header.version, 4, 1, file);

	if (strncmp(header.id, "MS3D000000", 10) != 0 || header.version != 4)
		return CORRUPTED_FILE;

	fread(&num_vertices, sizeof(word), 1, file);
	vertices = new MS3DVertex[num_vertices];

	for (i = 0; i < num_vertices; ++i)
	{
		fread(&vertices[i].flags, sizeof(byte), 1, file);
		fread(vertices[i].vertex, 4, 3, file);
		fread(&vertices[i].boneId, 1, 1, file);
		fread(&vertices[i].referenceCount, sizeof(byte), 1, file);
	}

	fread(&num_triangles, sizeof(word), 1, file);
	triangles = new MS3DTriangle[num_triangles];

	for (i = 0; i < num_triangles; ++i)
	{
		fread(&triangles[i].flags, sizeof(word), 1, file);
		fread(triangles[i].vertexIndices, sizeof(word), 3, file);

		for (int j = 0; j < 3; ++j)
			fread(triangles[i].vertexNormals[j], 4, 3, file);

		fread(triangles[i].s, 4, 3, file);
		fread(triangles[i].t, 4, 3, file);
		fread(&triangles[i].smoothingGroup, sizeof(byte), 1, file);
		fread(&triangles[i].groupIndex, sizeof(byte), 1, file);
	}

	fread(&num_groups, sizeof(word), 1, file);
	groups = new MS3DGroup[num_groups];

	for (i = 0; i < num_groups; ++i)
	{
		fread(&groups[i].flags, sizeof(byte), 1, file);
		fread(groups[i].name, 1, 32, file);
		fread(&groups[i].numtriangles, sizeof(word), 1, file);
		groups[i].triangleIndices = new word[groups[i].numtriangles];
		fread(groups[i].triangleIndices, sizeof(word), groups[i].numtriangles, file);
		fread(&groups[i].materialIndex, 1, 1, file);
	}

	fread(&num_materials, sizeof(word), 1, file);
	materials = new std::pair<MS3DMaterial, Texture>[num_materials];

	for (i = 0; i < num_materials; ++i)
	{
		fread(materials[i].first.name, 1, 32, file);
		fread(materials[i].first.ambient, 4, 4, file);
		fread(materials[i].first.diffuse, 4, 4, file);
		fread(materials[i].first.specular, 4, 4, file);
		fread(materials[i].first.emissive, 4, 4, file);
		fread(&materials[i].first.shininess, 4, 1, file);
		fread(&materials[i].first.transparency, 4, 1, file);
		fread(&materials[i].first.mode, 1, 1, file);
		fread(materials[i].first.texture, 1, 128, file);
		fread(materials[i].first.alphamap, 1, 128, file);

		if (strlen(materials[i].first.texture) > 0)
		{
			TEX_LOAD(materials[i].second, materials[i].first.texture, return TEX_ERR);
		}
	}

	fclose(file);
	return NONE;
}

void MS3DModel::render() const
{
	glPushAttrib(GL_ENABLE_BIT);

	for (word i = 0; i < num_groups; ++i)
	{
		if (groups[i].materialIndex < 0)
			glDisable(GL_TEXTURE_2D);
		else
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, materials[groups[i].materialIndex].first.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[groups[i].materialIndex].first.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, materials[groups[i].materialIndex].first.specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, materials[groups[i].materialIndex].first.emissive);
			glMaterialf(GL_FRONT, GL_SHININESS, materials[groups[i].materialIndex].first.shininess);

			if (materials[groups[i].materialIndex].second.get_name().empty())
				glDisable(GL_TEXTURE_2D);
			else
			{
				textureManager.bind(materials[groups[i].materialIndex].second);
				glEnable(GL_TEXTURE_2D);
			}
		}

		glBegin(GL_TRIANGLES);
		MS3DTriangle triangle;

		for (word j = 0; j < groups[i].numtriangles; ++j)
		{
			triangle = triangles[groups[i].triangleIndices[j]];

			for (int k = 0; k < 3; ++k)
			{
				glNormal3fv(triangle.vertexNormals[k]);
				glTexCoord2f(triangle.s[k], triangle.t[k]);
				glVertex3fv(vertices[triangle.vertexIndices[k]].vertex);
			}
		}

		glEnd();
	}

	glPopAttrib();
}
