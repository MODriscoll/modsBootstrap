#include "Rendering\Fonts\Font.h"

#include "IncludeGLFW.h"

// TODO: move to utility header and cpp
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

namespace mods
{
	Font::Font(const std::string& path, uint32 size)
		: m_Size(0)
	{
		Load(path, size);
	}

	Font::Font(Font&& rhs)
		: m_Glyphs(std::move(rhs.m_Glyphs))
		, m_Size(rhs.m_Size)
	{
		rhs.m_Size = 0;
	}

	Font::~Font()
	{
		Unload();
	}

	Font& Font::operator=(Font&& rhs)
	{
		Unload();

		m_Glyphs = std::move(rhs.m_Glyphs);
		m_Size = rhs.m_Size;

		rhs.m_Size = 0;
	}

	bool Font::Load(const std::string& path, uint32 size)
	{
		// TODO: Make utility function

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
		{
			std::cout << "Error: Failed to initialize free type library" << std::endl;
			return false;
		}

		FT_Face face;
		if (FT_New_Face(ft, path.c_str(), 0, &face))
		{
			std::cout << "Error: Failed to open font source at path: " << path << std::endl;
			return false;
		}

		// Set the size for the glyphs when extracting
		FT_Set_Pixel_Sizes(face, 0, size);

		// Record unpack now, so we can restore it once done
		int32 unpack; 
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack);

		// Disable openGL byte-alignement restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// TODO: update to support utf,
		// this will only support first 128 characters in ASCII
		for (byte c = 0; c < 128; ++c)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "Error: Failed to load glyph for character: " << std::to_string(c) << std::endl;
				continue;
			}

			const FT_GlyphSlot& slot = face->glyph;

			Glyph glyph;

			glyph.Size = glm::ivec2(slot->bitmap.width, slot->bitmap.rows);
			glyph.Bearing = glm::ivec2(slot->bitmap_left, slot->bitmap_top);
			glyph.Offset = slot->advance.x;

			glGenTextures(1, &glyph.Handle);
			glBindTexture(GL_TEXTURE_2D, glyph.Handle);

			// Only need one color channel
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph.Size.x, glyph.Size.y,
				0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			m_Glyphs.emplace(std::make_pair(c, std::move(glyph)));
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		// Restore original unpack alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	bool Font::Unload()
	{
		// There should be at least 
		// one glyph if we are valid
		if (m_Glyphs.empty())
			return true;

		for (auto& glyph : m_Glyphs)
			glDeleteTextures(1, &glyph.second.Handle);
	}
}