#include "hzpch.h"

#include "SubTexture2D.h"

namespace Hazel
{
	SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	std::shared_ptr<SubTexture2D> SubTexture2D::CreateFromCoords(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize, float interval)
	{
		const float x = coords.x, y = coords.y;
		const float sheetWidth = texture->GetWidth();
		const float sheetHeight = texture->GetHeight();
		const float spriteWidth = cellSize.x;
		const float spriteHeight = cellSize.y;

		// 纹理间隔增量
		const float intervalWidth = interval / sheetWidth;
		const float intervalHeight = interval / sheetHeight;

		/*constexpr glm::vec2 textureCoords[] = {
			{(x * spriteWidth) / sheetWidth + intervalWidth * x, (y * spriteHeight) / sheetHeight + intervalHeight * y},
			{((x + 1) * spriteWidth) / sheetWidth + intervalWidth * x, (y * spriteHeight) / sheetHeight + intervalHeight * y},
			{((x + 1) * spriteWidth) / sheetWidth + intervalWidth * x, ((y + 1) * spriteHeight) / sheetHeight + intervalHeight * y},
			{(x * spriteWidth) / sheetWidth + intervalWidth * x, ((y + 1) * spriteHeight) / sheetHeight + intervalHeight * y}
		};*/

		glm::vec2 min = { (x * spriteWidth) / sheetWidth + intervalWidth * x, (y * spriteHeight) / sheetHeight + intervalHeight * y };
		glm::vec2 max = { ((x + spriteSize.x) * spriteWidth) / sheetWidth + intervalWidth * x, ((y + spriteSize.y) * spriteHeight) / sheetHeight + intervalHeight * y };

		return std::make_shared<SubTexture2D>(texture, min, max);

	}
}
