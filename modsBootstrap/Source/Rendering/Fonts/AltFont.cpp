#include "Rendering\Fonts\AltFont.h"

#include "IncludeGLFW.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <vector>

namespace mods
{
	template<typename T>
	T to_nearest_pow2(const T num) {
		T nearest = 2;

		while ((nearest <<= 1) < num);

		return nearest;
	}

	AltFont::AltFont(const std::string& path, int32 size)
		: m_Atlas(0)
		, m_Height(0)
		, m_VAO(0)
	{
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "Error: Failed to initialise FreeType library" << std::endl;
			return;
		}

		FT_Face face;
		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
			std::cout << "Error: Failed to open font\nPath: " << path << std::endl;
			return;
		}

		m_Size = !size ? 16 : size;
		if (FT_Set_Pixel_Sizes(face, 0, m_Size))
		{
			std::cout << "Error: Failed to set size " << size << " for font" << std::endl;
		}

		int32 numChars = 128;
		std::vector<FT_GlyphSlotRec_> glyphslots;
		std::vector<std::vector<unsigned char>> bitmaps;
		std::vector<unsigned char> characters;

		bool bFail = false;
		for (unsigned char c = 0; c < 128; ++c)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "Error: Failed to load glyph for character " << c << std::endl;
				bFail = true;
				continue;
			}

			glyphslots.push_back(*face->glyph);
			bitmaps.push_back(std::vector<unsigned char>());

			auto& buf = bitmaps.back();
			buf.assign(face->glyph->bitmap.buffer, face->glyph->bitmap.buffer + (face->glyph->bitmap.width * face->glyph->bitmap.rows));

			characters.push_back(c);
		}

		uint32 pow = 6;
		RectangleBinPack bin;
		bin.Init(1 << pow, 1 << pow);
		bool fits = false;

		std::vector<uint32> indices;

		while (!fits)
		{
			fits = true;
			for (const auto& glyph : glyphslots)
			{
				uint32 index = bin.Insert(0, glyph.bitmap.width + 2, glyph.bitmap.rows + 2);

				if (index != -1)
				{
					indices.push_back(index);
				}
				else
				{
					fits = false;
					break;
				}
			}

			if (!fits)
			{
				++pow;
				bin.Init(1 << pow, 1 << pow);
				indices.clear();
			}
		}

		std::vector<unsigned char> buffer(bin.Width * bin.Height, 0);

		for (uint32 i = 0; i < glyphslots.size(); ++i)
		{
			const auto& glyph = glyphslots[i];
			const BinNode& node = bin.Nodes[indices[i]];
			const auto& bitmap = bitmaps[i];

			AltGlyph newglyph;
			newglyph.Position = glm::ivec2(node.X, node.Y);
			newglyph.Size = glm::ivec2(node.Width, node.Height);
			newglyph.Bearing = glm::ivec2(glyph.bitmap_left, glyph.bitmap_top);
			newglyph.Advance = glyph.advance.x;

			m_Glyphs[characters[i]] = newglyph;

			//if (bitmap.empty())
			//	continue;

			for (uint32 srcX = 0; srcX < glyph.bitmap.width; ++srcX)
			{
				uint32 dstX = srcX + node.X;
				for (uint32 srcY = 0; srcY < glyph.bitmap.rows; ++srcY)
				{
					uint32 srcIndex = srcX + srcY * glyph.bitmap.width;

					uint32 dstY = (glyph.bitmap.rows - srcY) + node.Y;
					uint32 dstIndex = dstX + dstY * bin.Width;

					buffer[dstIndex] = bitmap[srcIndex];
				}
			}
		}

		m_Width = bin.Width;
		m_Height = bin.Height;

		// Record unpack now, so we can restore it once done
		int32 unpack;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glGenTextures(1, &m_Atlas);
		glBindTexture(GL_TEXTURE_2D, m_Atlas);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

		glBindTexture(GL_TEXTURE_2D, 0);

		// Restore original unpack alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);

		uint32 MAX_CHARACTERS = 200;

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_CHARACTERS * 4 * sizeof(GlyphVertex), nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_CHARACTERS * 6 * sizeof(uint32), nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Position));
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, TexCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	AltFont::~AltFont()
	{
		if (m_Atlas != 0)
			glDeleteTextures(1, &m_Atlas);

		if (m_VAO != 0)
		{
			glDeleteVertexArrays(1, &m_VAO);
			glDeleteBuffers(1, &m_VBO);
			glDeleteBuffers(1, &m_IBO);
		}
	}

	void AltFont::Draw(const std::string& Text, const glm::ivec2& Position, const glm::vec4& color)
	{
		glm::vec2 pen = Position;
		//pen.y = 1280.f - pen.y;

		for (char c : Text)
		{
			// TODO: handle new line

			auto it = m_Glyphs.find(c);
			if (it == m_Glyphs.cend())
				continue;

			const auto& glyph = it->second;

			float xpos = pen.x + glyph.Bearing.x;
			float ypos = pen.y - (glyph.Size.y - glyph.Bearing.y);

			float w = glyph.Size.x;
			float h = glyph.Size.y;

			int32 index = m_GlyphsBuffer.size();

			GlyphVertex v;

			v.Color = color;	

			v.Position = glm::vec2(xpos, ypos + h);
			v.TexCoords.x = (float)glyph.Position.x / m_Width;
			v.TexCoords.y = ((float)glyph.Position.y / m_Height + ((float)glyph.Size.y / m_Height));
			//v.TexCoords = glm::vec2(0.f, 1.f);

			m_GlyphsBuffer.push_back(v);

			v.Position = glm::vec2(xpos, ypos);
			v.TexCoords.x = (float)glyph.Position.x / m_Width;
			v.TexCoords.y = (float)glyph.Position.y / m_Height;
			//v.TexCoords = glm::vec2(0.f);

			m_GlyphsBuffer.push_back(v);

			v.Position = glm::vec2(xpos + w, ypos);
			v.TexCoords.x = (float)glyph.Position.x / m_Width + ((float)glyph.Size.x / m_Width);
			v.TexCoords.y = (float)glyph.Position.y / m_Height;
			//v.TexCoords = glm::vec2(1.f, 0.f);

			m_GlyphsBuffer.push_back(v);

			v.Position = glm::vec2(xpos + w, ypos + h);
			v.TexCoords.x = (float)glyph.Position.x / m_Width + ((float)glyph.Size.x / m_Width);
			v.TexCoords.y = ((float)glyph.Position.y / m_Height + ((float)glyph.Size.y / m_Height));
			//v.TexCoords = glm::vec2(1.f);

			m_GlyphsBuffer.push_back(v);

			m_IndexBuffer.push_back(index + 0);
			m_IndexBuffer.push_back(index + 1);
			m_IndexBuffer.push_back(index + 2);

			m_IndexBuffer.push_back(index + 0);
			m_IndexBuffer.push_back(index + 2);
			m_IndexBuffer.push_back(index + 3);	

			pen.x += (glyph.Advance >> 6);
		}
	}

	void AltFont::Flush()
	{
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GlyphVertex) * m_GlyphsBuffer.size(), m_GlyphsBuffer.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32) * m_IndexBuffer.size(), m_IndexBuffer.data());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Atlas);

		glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_GlyphsBuffer.clear();
		m_IndexBuffer.clear();
	}

	void RectangleBinPack::Init(uint32 width, uint32 height)
	{
		Width = width;
		Height = height;

		Nodes.clear();

		BinNode root;
		root.Width = width;
		root.Height = height;
		root.Left = -1;
		root.Right = -1;
		root.X = 0;
		root.Y = 0;

		Nodes.push_back(root);
	}

	float RectangleBinPack::Occupancy()
	{
		uint32 used = OccupiedSurfaceArea(Nodes.front());
		uint32 total = Width * Height;

		return (float)used / (float)total;
	}

	int32 RectangleBinPack::Insert(uint32 index, uint32 width, uint32 height)
	{
		{
			BinNode& node = Nodes[index];

			if (node.Left != -1 || node.Right != -1)
			{
				if (node.Left != -1)
				{
					int32 index = Insert(node.Left, width, height);
					if (index != -1)
						return index;
				}

				if (node.Right != -1)
				{
					int32 index = Insert(node.Right, width, height);
					if (index != -1)
						return index;
				}

				// Doesn't fit
				return -1;
			}
		}

		// If we reach this point, we are a leaf node

		// Is object to big?
		if (width > Nodes[index].Width || height > Nodes[index].Height)
			return -1;

		uint32 w = Nodes[index].Width - width;
		uint32 h = Nodes[index].Height - height;
		// Pushing back can invalidate the reference

		Nodes[index].Left = Nodes.size();
		Nodes.push_back(BinNode());
		BinNode& left = Nodes.back();

		Nodes[index].Right = Nodes.size();
		Nodes.push_back(BinNode());
		BinNode& right = Nodes.back();

		BinNode& node = Nodes[index];

		if (w <= h)
		{
			// Split horizontally

			left.X = node.X + width;
			left.Y = node.Y;
			left.Width = w;
			left.Height = height;

			right.X = node.X;
			right.Y = node.Y + height;
			right.Width = node.Width;
			right.Height = h;
		}
		else
		{
			// Split vertically

			left.X = node.X;
			left.Y = node.Y + height;
			left.Width = width;
			left.Height = h;

			right.X = node.X + width;
			right.Y = node.Y;
			right.Width = w;
			right.Height = node.Height;
		}

		// This node is no longer a leaf
		node.Width = width;
		node.Height = height;

		return index;
	}

	uint32 RectangleBinPack::OccupiedSurfaceArea(const BinNode& node)
	{
		uint32 occupied = 0;
		if (node.Left != -1 || node.Right != -1)
		{
			occupied = node.Width * node.Height;

			if (node.Left != -1)
				occupied += OccupiedSurfaceArea(Nodes[node.Left]);

			if (node.Right != -1)
				occupied += OccupiedSurfaceArea(Nodes[node.Right]);
		}

		return occupied;
	}
}