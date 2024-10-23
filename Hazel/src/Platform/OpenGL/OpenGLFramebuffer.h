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
		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;
		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRenderer2D(uint32_t index = 0) const override;

	private:
		uint32_t m_RendererID = 0;			// 帧缓冲对象
		uint32_t m_DepthAttachment = 0;		// 当前帧缓冲的深度附件
		FrameBufferSpecification m_Specification;	// 帧缓冲规格

		std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecification;
		FrameBufferTextureSpecification m_DepthAttachSpecification = FrameBufferTextureFormat::None;

		std::vector<uint32_t> m_ColorAttachments;

	};
}
