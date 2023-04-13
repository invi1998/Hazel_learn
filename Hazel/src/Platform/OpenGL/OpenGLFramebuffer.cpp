#include "hzpch.h"

#include "OpenGLFramebuffer.h"
#include <glad/gl.h>

namespace Hazel
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification)
		: m_Specification(specification)
	{
		Create();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	const FrameBufferSpecification& OpenGLFrameBuffer::GetSpecification() const
	{
		return m_Specification;
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		glCreateFramebuffers(1, &m_RendererID);
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, )
		glBindFramebuffer()
	}
}
