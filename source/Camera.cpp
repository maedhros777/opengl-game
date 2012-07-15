#include "Camera.h"

void Camera::orient(bool translate) const
{
	glLoadIdentity();
	glRotatef(x_rot, 1.0, 0.0, 0.0);
	glRotatef(y_rot, 0.0, 1.0, 0.0);
	translatef(-pos);

    if (!translate)
    {
        float mat[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, mat);
        mat[12] = 0.0;
        mat[13] = 0.0;
        mat[14] = 0.0;
        glLoadMatrixf(mat);
    }
}

void Camera::rotateX(float rot)
{
	if ((x_rot += rot) >= 360.0)
		x_rot -= 360.0;
	else if (x_rot <= 360.0)
		x_rot += 360.0;
}

void Camera::rotateY(float rot)
{
	if ((y_rot += rot) >= 360.0)
		y_rot -= 360.0;
	else if (y_rot <= -360.0)
		y_rot += 360.0;
}

void Camera::advance(float amount)
{
	pos.setX(pos.getX() + dsin(y_rot) * amount);
	pos.setY(pos.getY() - dsin(x_rot) * amount);
	pos.setZ(pos.getZ() - dcos(y_rot) * amount);
}

void FPCamera::strafe(float amount)
{
	pos.setX(pos.getX() + dcos(y_rot) * amount);
	pos.setZ(pos.getZ() + dsin(y_rot) * amount);
}

void FPCamera::advance(float amount)
{
	pos.setX(pos.getX() + dsin(y_rot) * amount);
	pos.setZ(pos.getZ() - dcos(y_rot) * amount);
}
