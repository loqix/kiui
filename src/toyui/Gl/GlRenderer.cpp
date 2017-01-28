//  Copyright (c) 2016 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <toyui/Config.h>
#include <toyui/Gl/GlRenderer.h>

#ifdef NANOVG_GLEW
	#include <GL/glew.h>
#elif defined NANOVG_EMSCRIPTEN
	#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#include <GL/glext.h>
#endif

#include <toyui/Frame/Layer.h>
#include <toyui/UiWindow.h>

namespace toy
{
	GlRenderer::GlRenderer(const string& resourcePath)
		: NanoRenderer(resourcePath)
		, m_clock()
	{}

	GlRenderer::~GlRenderer()
	{}

	void GlRenderer::setupContext()
	{
		this->initGlew();

#if NANOVG_GL2
		m_ctx = nvgCreateGL2(NVG_ANTIALIAS);
#elif NANOVG_GL3
		m_ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#elif NANOVG_GLES2
		m_ctx = nvgCreateGLES2(NVG_STENCIL_STROKES);
#endif

		if(m_ctx == nullptr)
		{
			printf("Could not init nanovg.\n");
			return;
		}
	}

	void GlRenderer::releaseContext()
	{
#if NANOVG_GL2
		nvgDeleteGL2(m_ctx);
#elif NANOVG_GL3
		nvgDeleteGL3(m_ctx);
#elif NANOVG_GLES2
		nvgDeleteGLES2(m_ctx);
#endif

		m_ctx = nullptr;
	}

	void GlRenderer::initGlew()
	{
#ifdef NANOVG_GLEW
		glewExperimental = true;

		if(glewInit() != GLEW_OK) {
			printf("Could not init glew.\n");
			return;
		}
#endif
	}

	void GlRenderer::render(MasterLayer& masterLayer)
	{
		this->logFPS();

		if(masterLayer.target().gammaCorrected())
			glDisable(GL_FRAMEBUFFER_SRGB);

		// Update and render
		glViewport(0, 0, masterLayer.width(), masterLayer.height());

		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		NanoRenderer::render(masterLayer);

		if(masterLayer.target().gammaCorrected())
			glEnable(GL_FRAMEBUFFER_SRGB);
	}

	void GlRenderer::logFPS()
	{
		static size_t frames = 0;
		static double prevtime;

		double time = m_clock.read();
		double delta = time - prevtime;
		if(time - prevtime >= 4.f)
		{
			printf("fps %f\n", (frames / (time - prevtime)));
			prevtime = time;
			frames = 0;
		}

		++frames;
	}
}
