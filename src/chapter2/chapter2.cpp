#include <sb7.h>
#include <math.h>

class l21 : public sb7::application
{
	GLuint programId;
	GLuint vaoId;
	GLfloat bgColor[4];
public:

	void startup() override
	{
		programId = compile_shaders();
		glCreateVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
		glPointSize(40.0f);
	}

	void shutdown() override
	{
		glDeleteVertexArrays(1, &vaoId);
		glDeleteProgram(programId);
	}

	static GLuint compile_shaders()
	{
		GLuint vertexShaderId;
		GLuint fragmentShaderId;
		GLuint programId;

		static const GLchar * vShaderSource[] = {
			"#version 450 core\n"
			"void main(void)\n"
			"{\n"
			"    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);\n"
			"}\n"
		};

		static const GLchar * fShaderSource[] = {
			"#version 450 core\n"
			"out vec4 color;\n"
			"void main(void)\n"
			"{\n"
			"    color = vec4(0.3, 0.6, 1.0, 1.0);\n"
			"}\n"
		};

		vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderId, 1, vShaderSource, nullptr);
		glCompileShader(vertexShaderId);

		fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderId, 1, fShaderSource, nullptr);
		glCompileShader(fragmentShaderId);

		programId = glCreateProgram();
		glAttachShader(programId, vertexShaderId);
		glAttachShader(programId, fragmentShaderId);
		glLinkProgram(programId);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return programId;
	}

	void render(double currentTime) override
	{
		bgColor[0] = (float)(sin(currentTime) + 1.0f) * 0.5f;
		bgColor[1] = (float)(cos(currentTime) + 1.0f) * 0.5f;
		bgColor[2] = (float)(sin(currentTime * 0.32) + 1.0f) * 0.5f;
		bgColor[3] = 1.0f;
		glClearBufferfv(GL_COLOR, 0, bgColor);

		glUseProgram(programId);

		glDrawArrays(GL_POINTS, 0, 1);
	}
};

DECLARE_MAIN(l21);
