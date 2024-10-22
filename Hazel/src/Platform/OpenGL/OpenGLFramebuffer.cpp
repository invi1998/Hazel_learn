#include "hzpch.h"

#include "OpenGLFramebuffer.h"
#include <glad/gl.h>

namespace Hazel
{
	static const uint32_t s_MaxFrameBufferSize = 8192;

	namespace Utils
	{
		static GLenum TextureTarget(bool multiSampled)
		{
			return multiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multiSampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multiSampled), count, outID);
		}

		static void BindTexture(bool multiSampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSampled), id);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::None: return false;
			case FrameBufferTextureFormat::RGBA8: return false;
			case FrameBufferTextureFormat::RED_INTEGER: return false;
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			default: return false;
			}
		}

		static void AttachColorTexture(uint32_t id, uint32_t samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, size_t index)
		{
			const bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, index);
			}
			else
			{
				// 设置纹理的宽度和高度，边框的宽度，纹理的格式，纹理的数据类型，纹理的数据
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t uint32, uint32_t samples, int i, int i1, uint32_t width, uint32_t height)
		{
			
		}

		static GLenum HazelTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::RGBA8: return GL_RGBA8;
			case FrameBufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			case FrameBufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
			default: break;
			}

			HZ_CORE_ASSERT(false, "Unknown format!");
			return 0;
		}

	}

	

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& specification)
		: m_Specification(specification)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
			{
				m_ColorAttachmentSpecification.emplace_back(spec);
			}
			else
			{
				m_DepthAttachSpecification = spec;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(1, &m_ColorAttachment);
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(1, &m_ColorAttachment);
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		const bool multiSampled = m_Specification.Samples > 1;

		// Attachments
		if (!m_ColorAttachmentSpecification.empty())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecification.size());
			Utils::CreateTextures(multiSampled, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++)
			{
				Utils::BindTexture(multiSampled, m_ColorAttachments[i]);

				switch (m_ColorAttachmentSpecification[i].TextureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER: 
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
					break;
				default: break;
				}
			}
		}

		if (m_DepthAttachSpecification.TextureFormat != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multiSampled, &m_DepthAttachment, 1);
			Utils::BindTexture(multiSampled, m_DepthAttachment);

			switch (m_DepthAttachSpecification.TextureFormat)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
				break;
			}
		}
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		// 当进行放大和缩小操作时，可以设置纹理过滤选项，这里再纹理被放大和缩小时都使用邻近过滤（GL_LINEAR），避免产生像素颗粒
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		// glTexImage2D(GL_IMAGE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!")

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);

		// 清除颜色附件的数据

		constexpr int value = -1;
		glClearTexImage(m_ColorAttachments[1], 0, GL_RED_INTEGER, GL_INT, &value);		// 将当前格式（GL_RED_INTEGER）的纹理数据设置为value
	}

	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize)
		{
			HZ_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		HZ_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);		// 读取屏幕上指定位置的像素数据
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		HZ_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Attachment index out of range");

		auto& spec = m_ColorAttachmentSpecification[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::HazelTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
}
