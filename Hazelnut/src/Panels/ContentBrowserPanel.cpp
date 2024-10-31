#include "ContentBrowserPanel.h"

#include <string>

#include "Hazel/Core/Log.h"
#include "imgui/imgui.h"

// m_CurrentDirectory /= path.filename();
// 用于将一个路径追加到当前路径的末尾。这个操作符重载使得路径拼接变得非常方便和直观。
// 更新当前目录，进入子目录，即点击目录按钮后，进入该目录（filesystem重载了/=操作符，实现了目录的拼接）

namespace Hazel
{
	extern const std::filesystem::path g_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_BaseDirectory(), m_CurrentDirectory(g_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FullDirectoryIcon.png");
		m_EmptyDirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/EmptyDirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DtFileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		static float padding = 32.0f;
		static float thumbnailSize = 128.0f;
		static float cellSize = thumbnailSize + padding;

		// 左侧滑块区域
		ImGui::BeginChild("LeftPane", ImVec2(36, 0), true); // 固定宽度为50

		// 自定义绘制垂直滑块
		auto DrawVerticalSlider = [](const char* label, float* value, float min, float max)
			{
				float lineHeight = ImGui::GetWindowHeight() - 44.0f;
				
				ImGui::PushID(label);
				ImGui::BeginGroup();
				ImGui::Text(label);

				ImVec2 size = ImVec2(10, lineHeight);
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				// 绘制滑块背景
				draw_list->AddRectFilled(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(34, 34, 24, 255));
				// 绘制滑块边框
				draw_list->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(210, 210, 210, 255));

				// 计算滑块已滑过区域的高度
				float sliderHeight = (size.y - 2) * (*value - min) / (max - min);

				// 绘制渐变色的已滑过区域
				ImU32 col1 = IM_COL32(224, 195, 252, 255); // 红色
				ImU32 col2 = IM_COL32(252, 172, 142, 255); // 蓝色
				draw_list->AddRectFilledMultiColor(ImVec2(pos.x + 1, pos.y + size.y - sliderHeight), ImVec2(pos.x + size.x - 1, pos.y + size.y - 1), col1, col1, col2, col2);

				ImGui::InvisibleButton(label, size);
				if (ImGui::IsItemActive())
				{
					*value = min + (max - min) * (1.0f - (ImGui::GetIO().MousePos.y - pos.y) / size.y);
					if (*value < min) *value = min;
					if (*value > max) *value = max;
				}

				ImGui::EndGroup();
				ImGui::PopID();
			};

		DrawVerticalSlider("T", &thumbnailSize, 16.0f, 512.0f); // T for Thumbnail
		// ImGui::SameLine();
		// DrawVerticalSlider("P", &padding, 0.0f, 32.0f); // P for Padding

		ImGui::EndChild();

		ImGui::SameLine();

		// 右侧内容浏览器区域
		ImGui::BeginChild("ContentBrowser");

		if (m_CurrentDirectory != std::filesystem::path(g_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		const float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = static_cast<int>(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (const auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, g_AssetPath);

			ImGui::PushID(relativePath.string().c_str());
			std::shared_ptr<Texture2D> icon = directoryEntry.is_directory() ? std::filesystem::is_empty(path) ? m_EmptyDirectoryIcon : m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::ImageButton(reinterpret_cast<ImTextureID>(icon->GetRendererID()), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				// 修改鼠标指针为拖拽
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

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

		ImGui::EndChild();

		ImGui::End();
	}
}

