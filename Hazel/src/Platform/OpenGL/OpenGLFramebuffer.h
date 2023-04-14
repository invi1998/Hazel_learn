#pragma once
#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& specification);
		~OpenGLFrameBuffer() override;

		const FrameBufferSpecification& GetSpecification() const override { return m_Specification; }

		void Invalidate();

		virtual void Bind() override ;
		virtual void UnBind() override;

		virtual uint32_t GetColorAttachmentRenderer2D() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColorAttachment;
		uint32_t m_DepthAttachment;
		FrameBufferSpecification m_Specification;

	};
}
