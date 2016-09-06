/*
 * Copyright © 2012-2015 Graham Sellers
 *
 * This code is part of the OpenGL SuperBible, 6th Edition.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <sb7.h>

sb7::application * sb7::application::app = 0;

//#include <GL/glext.h>

#include <string.h>

GL3WglProc sb6GetProcAddress(const char * funcname)
{
	return gl3wGetProcAddress(funcname);
}

int sb6IsExtensionSupported(const char * extname)
{
	GLint numExtensions;
	GLint i;

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	for (i = 0; i < numExtensions; i++)
	{
		const GLubyte * e = glGetStringi(GL_EXTENSIONS, i);
		if (!strcmp((const char *)e, extname))
		{
			return 1;
		}
	}

	return 0;
}
void APIENTRY sb7::application::debug_callback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	GLvoid* userParam)
{
	reinterpret_cast<application *>(userParam)->onDebugMessage(source, type, id, severity, length, message);
}

void sb7::application::startup()
{
}

void sb7::application::render(double)
{
}

void sb7::application::shutdown()
{
}

void sb7::application::onKey(int, int)
{
}

void sb7::application::onMouseButton(int, int)
{
}

void sb7::application::onMouseMove(int, int)
{
}

void sb7::application::onMouseWheel(int)
{
}

void sb7::application::run(sb7::application* the_app)
{
	bool running = true;
	app = the_app;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

	init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
	if (info.flags.debug)
#endif /* _DEBUG */
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
	if (info.flags.robust)
	{
		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
	//        if (info.flags.fullscreen)
	//        {
	//            if (info.windowWidth == 0 || info.windowHeight == 0)
	//            {
	//                GLFWvidmode mode;
	//                glfwGetDesktopMode(&mode);
	//                info.windowWidth = mode.Width;
	//                info.windowHeight = mode.Height;
	//            }
	//
	//            glfwOpenWindow(info.windowWidth, info.windowHeight, 8, 8, 8, 0, 32, 0, GLFW_FULLSCREEN);
	//            glfwSwapInterval((int)info.flags.vsync);
	//        }
	//        else
	{
		window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!window)
		{
			fprintf(stderr, "Failed to open window\n");
			return;
		}
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, glfw_onResize);
	glfwSetKeyCallback(window, glfw_onKey);
	glfwSetMouseButtonCallback(window, glfw_onMouseButton);
	glfwSetCursorPosCallback(window, glfw_onMouseMove);
	glfwSetScrollCallback(window, glfw_onMouseWheel);
	if (!info.flags.cursor)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	// info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

	gl3wInit();

#ifdef _DEBUG
	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

	if (info.flags.debug)
	{
		if (gl3wIsSupported(4, 3))
		{
			glDebugMessageCallback((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		} else if (sb6IsExtensionSupported("GL_ARB_debug_output"))
		{
			glDebugMessageCallbackARB((GLDEBUGPROC)debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		}
	}

	startup();

	do
	{
		render(glfwGetTime());

		glfwSwapBuffers(window);
		glfwPollEvents();

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while (running);

	shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void sb7::application::init()
{
	strcpy(info.title, "OpenGL SuperBible Example");
	info.windowWidth = 800;
	info.windowHeight = 600;
#ifdef __APPLE__
	info.majorVersion = 3;
	info.minorVersion = 2;
#else
	info.majorVersion = 4;
	info.minorVersion = 3;
#endif
	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;
#ifdef _DEBUG
	info.flags.debug = 1;
#endif
}

void sb7::application::setWindowTitle(const char* title)
{
	glfwSetWindowTitle(window, title);
}

void sb7::application::onResize(int w, int h)
{
	info.windowWidth = w;
	info.windowHeight = h;
}

void sb7::application::onDebugMessage(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar* message)
{
#ifdef _WIN32
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
#endif /* _WIN32 */
}

void sb7::application::getMousePosition(int& x, int& y)
{
	double dx, dy;
	glfwGetCursorPos(window, &dx, &dy);

	x = static_cast<int>(floor(dx));
	y = static_cast<int>(floor(dy));
}


void sb7::application::glfw_onResize(GLFWwindow* window, int w, int h)
{
	app->onResize(w, h);
}

void sb7::application::glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	app->onKey(key, action);
}

void sb7::application::glfw_onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	app->onMouseButton(button, action);
}

void sb7::application::glfw_onMouseMove(GLFWwindow* window, double x, double y)
{
	app->onMouseMove(static_cast<int>(x), static_cast<int>(y));
}

void sb7::application::glfw_onMouseWheel(GLFWwindow* window, double xoffset, double yoffset)
{
	app->onMouseWheel(static_cast<int>(yoffset));
}

void sb7::application::setVsync(bool enable)
{
	info.flags.vsync = enable ? 1 : 0;
	glfwSwapInterval((int)info.flags.vsync);
}
