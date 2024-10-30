#include "ContentBrowserPanel.h"

#include <string>

#include "imgui/imgui.h"

// m_CurrentDirectory /= path.filename();
// 用于将一个路径追加到当前路径的末尾。这个操作符重载使得路径拼接变得非常方便和直观。
// 更新当前目录，进入子目录，即点击目录按钮后，进入该目录（filesystem重载了/=操作符，实现了目录的拼接）

namespace Hazel
{
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);

			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(relativePath.string().c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				ImGui::Text("%s", relativePath.string().c_str());
			}
		}

		ImGui::End();
	}
}

