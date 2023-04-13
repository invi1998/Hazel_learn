#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		~OpenGLFrameBuffer() override;

		const FrameBufferSpecification& GetSpecification() const override;

		void Invalidate();

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		FrameBufferSpecification m_Specification;

	};
}
