#include <sb7.h>
#include <math.h>
#include <shader.h>

class chapter3 : public sb7::application
{
	GLuint programId = 0;
	GLuint vaoId = 0;

public:

	void init() override
	{
		static const char title[] = "Chapter 3";
		sb7::application::init();
		memcpy(info.title, title, sizeof(title));
	}

	void startup() override
	{
		GLuint vertexShaderId = sb7::shader::load("chapter3/chapter3.vs.glsl", GL_VERTEX_SHADER);
		GLuint tessControlShaderId = sb7::shader::load("chapter3/chapter3.tcs.glsl", GL_TESS_CONTROL_SHADER);
		GLuint tessEvalShaderId = sb7::shader::load("chapter3/chapter3.tes.glsl", GL_TESS_EVALUATION_SHADER);
		GLuint fragmentShaderId = sb7::shader::load("chapter3/chapter3.fs.glsl", GL_FRAGMENT_SHADER);
		GLuint shaders[] = {
			vertexShaderId,
			tessControlShaderId,
			tessEvalShaderId,
			fragmentShaderId
		};

		programId = sb7::program::link_from_shaders(shaders, sizeof(shaders) / sizeof(shaders[0]), true, true);

		glCreateVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(1, &vaoId);
		glDeleteProgram(programId);
	}

	void render(double currentTime) override
	{
		static const GLfloat bgColor[] = {0.16f, 0.16f, 0.16f, 1.0f};
		glClearBufferfv(GL_COLOR, 0, bgColor);

		glUseProgram(programId);

		GLfloat offset[] = {
			(float)sin(currentTime) * 0.33f,
			(float)cos(currentTime) * 0.2f,
			0.0f,
			0.0f
		};
		glVertexAttrib4fv(0, offset);

		GLfloat triColor[] = {
			(float)sin(currentTime),
			(float)cos(currentTime),
			(float)sin(currentTime * 0.1),
			1.0f
		};
		glVertexAttrib4fv(1, triColor);

		glDrawArrays(GL_PATCHES, 0, 3);
	}
};

DECLARE_MAIN(chapter3);
