#include "ContentBrowserPanel.h"

#include <string>

#include "Hazel/Core/Log.h"
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
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FullDirectoryIcon.png");
		m_EmptyDirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/EmptyDirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DtFileIcon.png");
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

		static float padding = 32.0f;
		static float thumbnailSize = 128.0f;
		static float cellSize = thumbnailSize + padding;

		const float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);

			ImGui::PushID(relativePath.string().c_str());
			std::shared_ptr<Texture2D> icon = directoryEntry.is_directory() ? std::filesystem::is_empty(path) ? m_EmptyDirectoryIcon : m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* pathStr = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr, (wcslen(pathStr) + 1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{
				if (directoryEntry.is_directory())
				{
					m_CurrentDirectory /= path.filename();
				}
			}

			ImGui::TextWrapped(path.filename().string().c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16.0f, 512.0f);
		ImGui::SliderFloat("Padding", &padding, 0.0f, 32.0f);

		ImGui::End();
	}
}

