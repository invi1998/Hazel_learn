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

		static std::shared_ptr<FrameBuffer> Create(const FrameBufferSpecification& specification);
	};
}
