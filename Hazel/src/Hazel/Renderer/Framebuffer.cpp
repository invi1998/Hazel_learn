#include "hzpch.h"

#include "Framebuffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel
{
	std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			HZ_CORE_ASSERT(false, "RendererAPI is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLFrameBuffer>(specification);
		}
		}

		HZ_CORE_ASSERT(false, "Unkonwn RendererAPI!");
		return nullptr;
	}
}
