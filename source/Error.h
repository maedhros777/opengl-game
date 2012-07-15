#ifndef ERROR_H
#define ERROR_H
#include <string>

enum ERR_CODE
{
    NONE,
    FILE_NOT_FOUND,
    CORRUPTED_FILE,
    INVALID_EXTENSION,
    UNSUPPORTED_FILE_TYPE,
    BAD_BYTES_PER_PIXEL,
	ANIM_ALREADY_LOADED,
	SKELETON_NOT_FOUND,
	ANIMATION_NOT_FOUND,
	ANIMATION_ERROR
};

#define CHECK __attribute__((warn_unused_result))
#define ERR_RET ERR_CODE CHECK

std::string errString(ERR_CODE code);
extern std::string badfile, baddescr;

#endif
