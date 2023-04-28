#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		// color
		RGBA8,

		// Depth/stencil
		DEPTH24STENCIL8,

		// Default
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat format) : TextureFormat(format) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;
		// TODO: Filtering/wrap
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments) : Attachments(attachments){}

		std::vector<FrameBufferTextureSpecification> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
		FrameBufferAttachmentSpecification Attachments;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		
		virtual const FrameBufferSpecification& GetSpecification() const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRenderer2D() const = 0;

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& specification);
	};
}
