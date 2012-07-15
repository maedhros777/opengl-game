#include "Animation.h"

ERR_CODE Skeleton::load(const std::string &skelname)
{
	name = skelname;
	FILE *file = file_open("Skeletons/" + skelname + ".skeleton", "rb");
	
	if (!file)
	{
		badfile = "Skeletons/" + skelname + ".skeleton";
		return FILE_NOT_FOUND;
	}

	root = new Bone;
	read_bone(file, root, 0);
	return NONE;
}

void Skeleton::read_bone(FILE *file, Bone *bone, Bone *parent)
{
	fread(&bone->length, 4, 1, file);
	fread(&bone->id, 4, 1, file);
	float quat[4];
	fread(quat, 4, 4, file);
	bone->quat = Quaternion(quat[0], quat[1], quat[2], quat[3]);
	fread(&bone->num_children, 2, 1, file);
	bone->parent = parent;

	for (int i = 0; i < bone->num_children; ++i)
	{
		bone->children[i] = new Bone;
		read_bone(file, bone->children[i], bone);
	}
}

void Skeleton::destroy_bone(Bone *bone)
{
	if (bone)
	{
		for (int i = 0; i < bone->num_children; ++i)
			destroy_bone(bone->children[i]);

		delete bone;
	}
}

void Skeleton::render(Bone *bone) const
{
	glPushMatrix();
	mult_quat(bone->quat);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, bone->length, 0.0);
	glEnd();
	glTranslatef(0.0, bone->length, 0.0);

	for (int i = 0; i < bone->num_children; ++i)
		render(bone->children[i]);

	glPopMatrix();
}

ERR_CODE Skeleton::add_animation(const std::string &anim_name)
{
	if (animations.find(anim_name) != animations.end())
	{
		badfile = "Animations/" + name + '/' + anim_name + ".animation";
		return ANIM_ALREADY_LOADED;
	}

	animations[anim_name] = AnimationData();
	return animations[anim_name].load(name + '/' + anim_name + ".animation");
}

SkeletonManager::~SkeletonManager()
{
	for (std::map<std::string, Skeleton *>::iterator it = skeletons.begin(); it != skeletons.end(); ++it)
		delete it->second;
}

const Skeleton *SkeletonManager::get(const std::string &name)
{
	std::map<std::string, Skeleton *>::iterator it = skeletons.find(name);

	if (it != skeletons.end())
		return it->second;
	else
		return 0;
}

ERR_CODE Animation::link_skeleton(const std::string &skel_name)
{
	ASSERT(!skeleton);
	skeleton = skeletonManager.get(skel_name);

	if (!skeleton)
	{
		badfile = skel_name;
		return SKELETON_NOT_FOUND;
	}

	return NONE;
}

ERR_CODE Animation::init_data(const Bone *bone)
{
	ASSERT(skeleton);
	BoneData bdata;
	bdata.keyframe = 0;
	bdata.quat = skeleton->animations.find(name)->second.data.find(bone->id)->second.keyframes[0].quat;

	/*	if (bdata.quat != bone->quat)
	{
		disp(bdata.quat);
		disp(bone->quat);
		baddescr = "first keyframe of animation for bone #" + itostr(bone->id) + " must be equal to skeleton default";
		return ANIMATION_ERROR;
		}*/ //TODO: adjust

	glPushMatrix();
	mult_quat(bdata.quat);
	get_matrix_inverse(bdata.inv_orientation);
	glTranslatef(0.0, bone->length, 0.0);
	data[bone->id] = bdata;
	ERR_CODE ret;

	for (unsigned int i = 0; i < bone->num_children; ++i)
		if ((ret = init_data(bone->children[i])) != NONE)
			return ret;

	glPopMatrix();
	return NONE;
}

ERR_CODE Animation::animate(const std::string &anim_name, bool anim_loop)
{
	ASSERT(skeleton);

	if (skeleton->animations.find(anim_name) == skeleton->animations.end())
	{
		badfile = anim_name;
		return ANIMATION_NOT_FOUND;
	}

	done = false;
	name = anim_name;
	time = 0;
	loop = anim_loop;
	glPushMatrix();
	glLoadIdentity();
	ERR_CODE ret = init_data(skeleton->root);
	glPopMatrix();
	return ret;
}

void Animation::update()
{
	ASSERT(skeleton);

	if (!done)
	{
		update_bone(skeleton->root);

		if (++time >= skeleton->animations.find(name)->second.get_length())
		{
			time = 0;

			if (!loop)
				done = true;
		}
	}
}

void Animation::update_bone(const Bone *bone)
{
	ASSERT(skeleton);
	KeyframeData keyframe_data = skeleton->animations.find(name)->second.get_keyframe_data(bone->id);

	if (time == 0)
	{
		data[bone->id].keyframe = 0;
		data[bone->id].quat = bone->quat;
	}

	else if (data[bone->id].keyframe < keyframe_data.num_keyframes - 1)
	{
		if (keyframe_data.keyframes[data[bone->id].keyframe + 1].time == time)
			data[bone->id].quat = keyframe_data.keyframes[++data[bone->id].keyframe].quat;

		else
		{
			float time_passed = time - keyframe_data.keyframes[data[bone->id].keyframe].time;
			float total_time = keyframe_data.keyframes[data[bone->id].keyframe + 1].time -
				keyframe_data.keyframes[data[bone->id].keyframe].time;
			data[bone->id].quat = slerp(keyframe_data.keyframes[data[bone->id].keyframe].quat,
										keyframe_data.keyframes[data[bone->id].keyframe + 1].quat, time_passed / total_time);
		}
	}

	glPushMatrix();
	mult_quat(data[bone->id].quat);
	float matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	data[bone->id].abs_pos.set(matrix[12], matrix[13], matrix[14]);
	data[bone->id].abs_quat = to_quat(matrix);
	glTranslatef(0.0, bone->length, 0.0);

	for (unsigned int i = 0; i < bone->num_children; ++i)
		update_bone(bone->children[i]);

	glPopMatrix();
}

BoneData Animation::get_bone_data(unsigned int id) const
{
	ASSERT(data.find(id) != data.end());
	return data.find(id)->second;
}

void Animation::render() const
{
	render_bone(skeleton->root);
}

void Animation::render_bone(const Bone *bone) const
{
	glPushMatrix();
	mult_quat(data.find(bone->id)->second.quat);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, bone->length, 0.0);
	glEnd();
	glTranslatef(0.0, bone->length, 0.0);

	for (int i = 0; i < bone->num_children; ++i)
		render_bone(bone->children[i]);

	glPopMatrix();
}
	
ERR_CODE AnimationData::load(const std::string &filename)
{
	FILE *file = file_open("Animations/" + filename, "rb");
	
	if (!file)
	{
		badfile = "Animations/" + filename;
		return FILE_NOT_FOUND;
	}

	unsigned int num_bones, id;
	float quat[4];
	KeyframeData keyframe_data;
	fread(&length, 4, 1, file);
	fread(&num_bones, 4, 1, file);

	for (unsigned int i = 0; i < num_bones; ++i)
	{
		fread(&id, 4, 1, file);
		fread(&keyframe_data.num_keyframes, 4, 1, file);
		keyframe_data.keyframes = new Keyframe[keyframe_data.num_keyframes];

		for (unsigned int j = 0; j < keyframe_data.num_keyframes; ++j)
		{
			fread(quat, 4, 4, file);
			keyframe_data.keyframes[j].quat = Quaternion(quat[0], quat[1], quat[2], quat[3]);
			fread(&keyframe_data.keyframes[j].time, 4, 1, file);
		}

		data[id] = keyframe_data;
	}

	return NONE;
}

AnimationData::~AnimationData()
{
	for (std::map<unsigned int, KeyframeData>::iterator it = data.begin(); it != data.end(); ++it)
		delete[] it->second.keyframes;
}

/*void get_parent_matrix(AnimatedBone *bone, unsigned int time)
{
	if (bone->parent)
	{
		get_parent_matrix(bone->parent);
		glTranslatef(0.0, bone->parent->length, 0.0);
	}

	Quaternion quat = bone->keyframes[0].quaternion;

	for (unsigned int i = 0; i < bone->num_keyframes; ++i)
	{
		if (bone->keyframes[i].time == time)
		{
			quat = bone->keyframes[i].quaternion;
			break;
		}

		else if (bone->keyframes[i].time > time)
		{
			quat = slerp(bone->keyframes[i - 1].quaternion, bone->keyframes[i].quaternion,
						 (float)(time - bone->keyframes[i - 1].time) / (bone->keyframes[i].time - bone->keyframes[i - 1].time));
			break;
		}
	}

	float x = quat.getX(), y = quat.getY(), z = quat.getZ(), w = quat.getW();
	float x2 = x * x, y2 = y * y, z2 = z * z;
	float xy = x * y, xz = x * z, yz = y * z, wx = w * x, wy = w * y, wz = w * z;
	float matrix[16] = { 1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
						 2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
						 2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
						 0.0, 0.0, 0.0, 1.0 };
	glMultMatrixf(matrix);
}
	
void get_matrix(AnimatedBone *bone, unsigned int time, float mat[16])
{
	Quaternion quat = bone->keyframes[0].quaternion;

	for (unsigned int i = 0; i < bone->num_keyframes; ++i)
	{
		if (bone->keyframes[i].time == time)
		{
			quat = bone->keyframes[i].quaternion;
			break;
		}

		else if (bone->keyframes[i].time > time)
		{
			quat = slerp(bone->keyframes[i - 1].quaternion, bone->keyframes[i].quaternion,
						 (float)(time - bone->keyframes[i - 1].time) / (bone->keyframes[i].time - bone->keyframes[i - 1].time));
			break;
		}
	}

	glPushMatrix();
	glLoadIdentity();

	if (bone->parent)
	{
		get_parent_matrix(bone->parent, time);
		glTranslatef(0.0, bone->parent->length, 0.0);
	}

	float x = quat.getX(), y = quat.getY(), z = quat.getZ(), w = quat.getW();
	float x2 = x * x, y2 = y * y, z2 = z * z;
	float xy = x * y, xz = x * z, yz = y * z, wx = w * x, wy = w * y, wz = w * z;
	float matrix[16] = { 1.0 - 2.0 * (y2 + z2), 2.0 * (xy + wz), 2.0 * (xz - wy), 0.0,
						 2.0 * (xy - wz), 1.0 - 2.0 * (x2 + z2), 2.0 * (yz + wx), 0.0,
						 2.0 * (xz + wy), 2.0 * (yz - wx), 1.0 - 2.0 * (x2 + y2), 0.0,
						 0.0, 0.0, 0.0, 1.0 };
	glMultMatrixf(matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();
}
*/

void get_matrix_inverse(float mat[16])
{
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	mat[0] = m[5]  * m[10] * m[15] - 
             m[5]  * m[11] * m[14] - 
             m[9]  * m[6]  * m[15] + 
             m[9]  * m[7]  * m[14] +
             m[13] * m[6]  * m[11] - 
             m[13] * m[7]  * m[10];

    mat[4] = -m[4]  * m[10] * m[15] + 
              m[4]  * m[11] * m[14] + 
              m[8]  * m[6]  * m[15] - 
              m[8]  * m[7]  * m[14] - 
              m[12] * m[6]  * m[11] + 
              m[12] * m[7]  * m[10];

    mat[8] = m[4]  * m[9] * m[15] - 
             m[4]  * m[11] * m[13] - 
             m[8]  * m[5] * m[15] + 
             m[8]  * m[7] * m[13] + 
             m[12] * m[5] * m[11] - 
             m[12] * m[7] * m[9];

    mat[12] = -m[4]  * m[9] * m[14] + 
               m[4]  * m[10] * m[13] +
               m[8]  * m[5] * m[14] - 
               m[8]  * m[6] * m[13] - 
               m[12] * m[5] * m[10] + 
               m[12] * m[6] * m[9];

    mat[1] = -m[1]  * m[10] * m[15] + 
              m[1]  * m[11] * m[14] + 
              m[9]  * m[2] * m[15] - 
              m[9]  * m[3] * m[14] - 
              m[13] * m[2] * m[11] + 
              m[13] * m[3] * m[10];

    mat[5] = m[0]  * m[10] * m[15] - 
             m[0]  * m[11] * m[14] - 
             m[8]  * m[2] * m[15] + 
             m[8]  * m[3] * m[14] + 
             m[12] * m[2] * m[11] - 
             m[12] * m[3] * m[10];

    mat[9] = -m[0]  * m[9] * m[15] + 
              m[0]  * m[11] * m[13] + 
              m[8]  * m[1] * m[15] - 
              m[8]  * m[3] * m[13] - 
              m[12] * m[1] * m[11] + 
              m[12] * m[3] * m[9];

    mat[13] = m[0]  * m[9] * m[14] - 
              m[0]  * m[10] * m[13] - 
              m[8]  * m[1] * m[14] + 
              m[8]  * m[2] * m[13] + 
              m[12] * m[1] * m[10] - 
              m[12] * m[2] * m[9];

    mat[2] = m[1]  * m[6] * m[15] - 
             m[1]  * m[7] * m[14] - 
             m[5]  * m[2] * m[15] + 
             m[5]  * m[3] * m[14] + 
             m[13] * m[2] * m[7] - 
             m[13] * m[3] * m[6];

    mat[6] = -m[0]  * m[6] * m[15] + 
              m[0]  * m[7] * m[14] + 
              m[4]  * m[2] * m[15] - 
              m[4]  * m[3] * m[14] - 
              m[12] * m[2] * m[7] + 
              m[12] * m[3] * m[6];

    mat[10] = m[0]  * m[5] * m[15] - 
              m[0]  * m[7] * m[13] - 
              m[4]  * m[1] * m[15] + 
              m[4]  * m[3] * m[13] + 
              m[12] * m[1] * m[7] - 
              m[12] * m[3] * m[5];

    mat[14] = -m[0]  * m[5] * m[14] + 
               m[0]  * m[6] * m[13] + 
               m[4]  * m[1] * m[14] - 
               m[4]  * m[2] * m[13] - 
               m[12] * m[1] * m[6] + 
               m[12] * m[2] * m[5];

    mat[3] = -m[1] * m[6] * m[11] + 
              m[1] * m[7] * m[10] + 
              m[5] * m[2] * m[11] - 
              m[5] * m[3] * m[10] - 
              m[9] * m[2] * m[7] + 
              m[9] * m[3] * m[6];

    mat[7] = m[0] * m[6] * m[11] - 
             m[0] * m[7] * m[10] - 
             m[4] * m[2] * m[11] + 
             m[4] * m[3] * m[10] + 
             m[8] * m[2] * m[7] - 
             m[8] * m[3] * m[6];

    mat[11] = -m[0] * m[5] * m[11] + 
               m[0] * m[7] * m[9] + 
               m[4] * m[1] * m[11] - 
               m[4] * m[3] * m[9] - 
               m[8] * m[1] * m[7] + 
               m[8] * m[3] * m[5];

    mat[15] = m[0] * m[5] * m[10] - 
              m[0] * m[6] * m[9] - 
              m[4] * m[1] * m[10] + 
              m[4] * m[2] * m[9] + 
              m[8] * m[1] * m[6] - 
              m[8] * m[2] * m[5];

    float det = 1.0 / (m[0] * mat[0] + m[1] * mat[4] + m[2] * mat[8] + m[3] * mat[12]);

    for (int i = 0; i < 16; i++)
        mat[i] *= det;
}
