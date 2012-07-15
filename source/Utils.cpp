#include "Utils.h"
#include "Logger.h"
#include "Texture.h"
#include "Quaternion.h"
#include "Point.h"

std::string PATH;
std::pair<Texture, ERR_CODE> dummy_tex_pair;

bool strieq(const std::string &str1, const std::string &str2)
{
    unsigned int len = str1.length();

    if (len != str2.length())
        return false;

    for (unsigned int i = 0; i < len; ++i)
        if (toupper(str1[i]) != toupper(str2[i]))
            return false;
    return true;
}

std::string itostr(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

void die(const std::string &msg)
{
	logger.logln(LOG_USER | LOG_FILE, msg);
	exit(0);
}
//TODO: delete these
void disp(const Quaternion &q)
{
	std::cout << "quat (";
	if (fabs(q.getW()) < 0.000001)
		std::cout << "0, ";
	else
		std::cout << q.getW() << ", ";
	if (fabs(q.getX()) < 0.000001)
		std::cout << "0, ";
	else
		std::cout << q.getX() << ", ";
	if (fabs(q.getY()) < 0.000001)
		std::cout << "0, ";
	else
		std::cout << q.getY() << ", ";
	if (fabs(q.getZ()) < 0.000001)
		std::cout << "0)";
	else
		std::cout << q.getZ() << ")";
	std::cout << std::endl;
}

void disp_mat_mult(float mat[16], float x, float y, float z)
{
	std::cout << "(" << (mat[0] * x + mat[4] * y + mat[8] * z + mat[12]) << ", ";
	std::cout << (mat[1] * x + mat[5] * y + mat[9] * z + mat[13]) << ", ";
	std::cout << (mat[2] * x + mat[6] * y + mat[10] * z + mat[14]) << ")" << std::endl;
}

void disp_mat(float mat[16])
{
	std::cout << "matrix:\n";
	for (int i = 0; i < 4; ++i)
	{
		std::cout << "[ ";
		for (int j = 0; j < 4; ++j)
		{
			if (fabs(mat[j * 4 + i]) > 0.0001)
				std::cout << mat[j * 4 + i] << ' ';
			else
				std::cout << "0 ";
		}
		std::cout << "]\n";
	}
}
