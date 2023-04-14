#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel
{
	struct FrameBufferSpecification
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
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
