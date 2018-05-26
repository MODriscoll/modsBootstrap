#include "Rendering\Fonts\Font.h"

#include "IncludeGLFW.h"
#include "Rendering\Shaders\Shader.h"

// TODO: move to utility header and cpp
#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

namespace mods
{
	Font::Font(const std::string& path, uint32 size)
		: m_VBO(0)
		, m_VAO(0)
		, m_Size(0)
	{
		Load(path, size);
	}

	Font::Font(Font&& rhs)
		: m_VBO(rhs.m_VBO)
		, m_VAO(rhs.m_VAO)
		, m_Glyphs(std::move(rhs.m_Glyphs))
		, m_Size(rhs.m_Size)
	{
		rhs.m_VBO = 0;
		rhs.m_VAO = 0;
		rhs.m_Size = 0;
	}

	Font::~Font()
	{
		Unload();
	}

	Font& Font::operator=(Font&& rhs)
	{
		Unload();

		m_VBO = rhs.m_VBO;
		m_VAO = rhs.m_VAO;
		m_Glyphs = std::move(rhs.m_Glyphs);
		m_Size = rhs.m_Size;

		rhs.m_VBO = 0;
		rhs.m_VAO = 0;
		rhs.m_Size = 0;

		return *this;
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

		// Create the quad to render to
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);

		// Since we update the contents of the buffer for each glyph when drawing
		// We allocate the memory use GL_DYNAMIC_DRAW
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}

	bool Font::Unload()
	{
		// There should be at least 
		// one glyph if we are valid
		if (m_Glyphs.empty())
			return true;

		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);

		for (auto& glyph : m_Glyphs)
			glDeleteTextures(1, &glyph.second.Handle);

		m_VAO = 0;
		m_VBO = 0;
		m_Glyphs.clear();
		m_Size = 0;

		return true;
	}

	void Font::Draw(ShaderProgram& shader, const std::string& text,
		glm::vec2 position, const glm::vec4& color, float scale) const
	{
		if (!IsValid())
			return;
		
		shader.SetUniformValue("textcolor", color);

		// Instead of calling once per loop
		glActiveTexture(GL_TEXTURE0);

		glBindVertexArray(m_VAO);
		
		for (char c : text)
		{
			const Glyph& glyph = m_Glyphs.at(c);

			float x = position.x + glyph.Bearing.x * scale;
			float y = position.y - (glyph.Size.y - glyph.Bearing.y) * scale;

			float width = glyph.Size.x * scale;
			float height = glyph.Size.y * scale;

			// Vertices, takes in a vector 4
			// vec4 vData = (pos.x, pos.y, texcoords.x, texcoords.y)
			float vertices[6][4] = {
				{ x,			y + height,	0.f, 0.f },
				{ x,			y,			0.f, 1.f },
				{ x + width,	y,			1.f, 1.f },

				{ x,			y + height, 0.f, 0.f },
				{ x + width,	y,			1.f, 1.f },
				{ x + width,	y + height,	1.f, 0.f }
			};

			// Set quad to render this glyphs texture
			glBindTexture(GL_TEXTURE_2D, glyph.Handle);

			// Update current vertex buffer
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 24, vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			// Bitshift by 6 to get value in pixels (2^6 = 64)
			position.x += (glyph.Offset >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}