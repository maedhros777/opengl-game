#include "Error.h"

std::string badfile, baddescr;

std::string errString(ERR_CODE code)
{
    std::string err;

    switch (code)
    {
	case FILE_NOT_FOUND:
		err = "file \"" + badfile + "\" not found";
		break;
	case CORRUPTED_FILE:
		err = "file \"" + badfile + "\" corrupted";
		break;
	case INVALID_EXTENSION:
		err = "file \"" + badfile + "\" has an invalid file extension";
		break;
	case UNSUPPORTED_FILE_TYPE:
		err = "file \"" + badfile + "\" is of a file type that is not yet supported";
		break;
	case BAD_BYTES_PER_PIXEL:
		err = "file \"" + badfile + "\" has an invalid number of bytes per pixel";
		break;
	case ANIM_ALREADY_LOADED:
		err = "animation file \"" + badfile + "\" has already been loaded";
		break;
	case SKELETON_NOT_FOUND:
		err = "skeleton \"" + badfile + "\" not found";
		break;
	case ANIMATION_NOT_FOUND:
		err = "animation \"" + badfile + "\" not found";
		break;
	case ANIMATION_ERROR:
		err = "animation error";
		break;
	case NONE:
		err = "programming error";
		break;
    }

	if (!baddescr.empty())
		err += (" (" + baddescr + ')');

    return err;
}
