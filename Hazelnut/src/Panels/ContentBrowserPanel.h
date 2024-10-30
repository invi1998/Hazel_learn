#pragma once

#include <filesystem>

#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();
		
		void OnImGuiRender();

	private:

		std::filesystem::path m_CurrentDirectory;		// 当前目录

		std::shared_ptr<Texture2D> m_DirectoryIcon;			// 目录图标
		std::shared_ptr<Texture2D> m_EmptyDirectoryIcon;	// 空文件夹目录图标
		std::shared_ptr<Texture2D> m_FileIcon;				// 文件图标;

	};
}
