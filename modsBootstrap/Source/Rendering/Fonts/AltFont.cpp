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
		: m_Handle(0)
		, m_Height(0)
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

		size = !size ? 16 : size;
		if (FT_Set_Pixel_Sizes(face, 0, size))
		{
			std::cout << "Error: Failed to set size " << size << " for font" << std::endl;
		}

		int32 numChars = 128;
		std::vector<FT_GlyphSlotRec_> glyphslots;
		std::vector<std::vector<unsigned char>> bitmaps;

		bool bFail = false;
		for (unsigned char c = 32; c < 128; ++c)
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

			if (bitmap.empty())
				continue;

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

		glGenTextures(1, &m_Handle);
		glBindTexture(GL_TEXTURE_2D, m_Handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, m_Width, m_Height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer.data());

		glBindTexture(GL_TEXTURE_2D, 0);

		// Restore original unpack alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	AltFont::~AltFont()
	{
		if (m_Handle != 0)
			glDeleteTextures(1, &m_Handle);
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