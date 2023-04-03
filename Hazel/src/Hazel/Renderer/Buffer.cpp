#include "hzpch.h"

#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

//#if HZ_PLATFORM_WINDOWS
//#include "Platform/Direct3D/Direct3DBuffer.h"
//#endif

namespace Hazel
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
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
			return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}
		}

		HZ_CORE_ASSERT(false, "Unkonwn RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
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
			return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}
		}

		HZ_CORE_ASSERT(false, "Unkonwn RendererAPI!");
		return nullptr;
	}
}
