#include <sb7.h>
#include <math.h>

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
		programId = compile_shaders();
		glCreateVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);
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
			"    const vec4 verts[] = vec4[] (\n"
			"        vec4( 0.62, -0.62, 0.5, 1.0),"
			"        vec4(-0.62, -0.62, 0.5, 1.0),"
			"        vec4( 0.62,  0.62, 0.5, 1.0)"
			"    );\n"
			"    gl_Position = verts[gl_VertexID];\n"
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
		static const GLfloat bgColor[] = { 0.16f, 0.16f, 0.16f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, bgColor);

		glUseProgram(programId);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
};

DECLARE_MAIN(chapter3);
