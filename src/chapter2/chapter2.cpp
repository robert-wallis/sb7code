#include <sb7.h>
#include <math.h>

class l21 : public sb7::application
{
	GLfloat bgColor[4];
public:
	void render(double currentTime) override
	{
		bgColor[0] = (float)(sin(currentTime) + 1.0f) * 0.5f;
		bgColor[1] = (float)(cos(currentTime) + 1.0f) * 0.5f;
		bgColor[2] = (float)(sin(currentTime * 0.32) + 1.0f) * 0.5f;
		bgColor[3] = 1.0f;
		glClearBufferfv(GL_COLOR, 0, bgColor);
	}
};

DECLARE_MAIN(l21);
