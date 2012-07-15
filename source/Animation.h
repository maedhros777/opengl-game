#ifndef ANIMATION_H
#define ANIMATION_H

#include "Quaternion.h"
#include "Error.h"
#include "Utils.h"
#include "Point.h"
#include "Singleton.h"
#include <map>

const int MAX_CHILDREN = 10;

struct Bone
{
	float length;
	unsigned int id;
	Quaternion quat;
	unsigned short num_children;
	Bone *children[MAX_CHILDREN]; //TODO: consider allocating dynamically
	Bone *parent;
};

struct BoneData
{
	Point3f abs_pos;
	unsigned int keyframe;
	Quaternion quat, abs_quat;
	float inv_orientation[16];
};

class Skeleton;

class Animation
{
private:
	const Skeleton *skeleton;
	std::map<unsigned int, BoneData> data;
	std::string name;
	unsigned int time;
	bool loop, done;
	void update_bone(const Bone *bone);
	void render_bone(const Bone *bone) const;
	ERR_RET init_data(const Bone *bone);
public:
	Animation() : skeleton(0), time(0), loop(false), done(false)
	{
	}

	bool is_done() const
	{
		return done;
	}

	ERR_RET link_skeleton(const std::string &skel_name);
	ERR_RET animate(const std::string &anim_name, bool anim_loop = false);
	BoneData get_bone_data(unsigned int id) const;
	void update();
	void render() const;
};

class AnimationData;

class Skeleton
{
	friend class Animation;
private:
	Bone *root;
	std::string name;
	std::map<std::string, AnimationData> animations;
	void read_bone(FILE *file, Bone *bone, Bone *parent);
	void destroy_bone(Bone *bone);
	void render(Bone *bone) const;
public:
	Skeleton() : root(0)
	{
	}

	~Skeleton()
	{
		destroy_bone(root);
	}

	std::string get_name() const
	{
		return name;
	}

	void render() const
	{
		render(root);
	}

	ERR_RET load(const std::string &skelname);
	ERR_RET add_animation(const std::string &anim_name);
};

class SkeletonManager : public Singleton<SkeletonManager>
{
private:
	std::map<std::string, Skeleton *> skeletons;
public:
	~SkeletonManager();
	const Skeleton *get(const std::string &name);

	void add_skeleton(Skeleton *skel)
	{
		skeletons[skel->get_name()] = skel;
	}

	void render(const std::string &name)
	{
		ASSERT(skeletons.find(name) != skeletons.end());
		skeletons.find(name)->second->render();
	}
};

#define skeletonManager SkeletonManager::getInstance()
#define skeletonManagerPtr SkeletonManager::getInstancePtr()

struct Keyframe
{
	Quaternion quat;
	unsigned int time;
};

struct KeyframeData
{
	unsigned int num_keyframes;
	Keyframe *keyframes;
};

class AnimationData
{
	friend class Animation;
private:
	std::map<unsigned int, KeyframeData> data;
	unsigned int length;
public:
	~AnimationData();
	ERR_RET load(const std::string &filename);

	unsigned int get_length() const
	{
		return length;
	}

	KeyframeData get_keyframe_data(unsigned int id) const
	{
		ASSERT(data.find(id) != data.end());
		return data.find(id)->second;
	}

	void set_keyframe_data(unsigned int id, const KeyframeData &keyframe_data)
	{
		data[id] = keyframe_data;
	}
};

void get_matrix_inverse(float mat[16]);

#endif
