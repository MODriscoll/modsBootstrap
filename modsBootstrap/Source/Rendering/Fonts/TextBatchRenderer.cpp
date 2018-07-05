#include "Rendering\Fonts\TextBatchRenderer.h"

#include "IncludeGLFW.h"

#include <glm\common.hpp>

namespace mods
{
	TextBatchRenderer::TextBatchRenderer()
		: m_MaxBatchSize(0)
		, m_VAO(0)
		, m_VBO(0)
		, m_IBO(0)
		, m_BatchSize(0)
		, m_FontsInBatch(0)
	{
		m_Program.Load("Resources/Shaders/HUD/TextBatch.vert", "Resources/Shaders/HUD/TextBatch.frag");
	}

	TextBatchRenderer::~TextBatchRenderer()
	{
		Destroy();
	}

	bool TextBatchRenderer::Create(uint32 batchSize)
	{
		// Destroy potential existing batch
		Destroy();

		if (batchSize == 0)
			return false;

		m_MaxBatchSize = batchSize;

		// Four vertices per letter
		uint32 VertexSize = m_MaxBatchSize * (sizeof(GlyphVertex) * 4);
		
		// Six indices per letter
		uint32 IndexSize = m_MaxBatchSize * (sizeof(uint32) * 6);

		// Pre-calculate all the indices as they are not going to change
		std::vector<uint32> indices(m_MaxBatchSize * 6);
		for (uint32 i = 0, v = 0; i < indices.size(); v += 4)
		{
			indices[i++] = v + 0;
			indices[i++] = v + 1;
			indices[i++] = v + 2;
			
			indices[i++] = v + 0;
			indices[i++] = v + 2;
			indices[i++] = v + 3;
		}

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Position));
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Index));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Color));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, TexCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		m_Vertices.resize(m_MaxBatchSize * 4);

		return true;
	}

	bool TextBatchRenderer::Destroy()
	{
		if (!IsValid())
			return true;

		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);

		m_VAO = 0;
		m_VBO = 0;
		m_IBO = 0;

		m_Vertices.clear();
		
		m_AtlasHandles.fill(0);
		m_FontsInBatch = 0;

		return true;
	}

	void TextBatchRenderer::Flush()
	{
		if (!IsValid())
			return;

		// Is there anything to flush?
		if (m_Vertices.empty() || m_BatchSize == 0)
			return;

		m_Program.Bind();

		// Set up font uniform values
		{
			std::string uniform("atlas[");

			for (int32 i = 0; i < (int32)m_FontsInBatch; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_AtlasHandles[i]);
				m_Program.SetUniformValue(uniform + std::to_string(i) + ']', i);
			}
		}

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_BatchSize * (sizeof(GlyphVertex) * 4), m_Vertices.data());

		glDrawElements(GL_TRIANGLES, m_BatchSize * 6, GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_Program.Unbind();

		m_BatchSize = 0;

		m_AtlasHandles.fill(0);
		m_FontsInBatch = 0;
	}

	void TextBatchRenderer::Print(const std::string& text, const AltFont& font, const glm::ivec2& position, const glm::vec4& color, float depth)
	{
		if (!IsValid())
			return;

		glm::vec2 pen = position;
		depth = glm::clamp(depth, 0.f, 1.f);
		
		// Add atlas now, track its index to give to vertices
		uint32 atlas = AddFont(font);

		float width = (float)font.GetWidth();
		float height = (float)font.GetHeight();

		for (char c : text)
		{
			if (ShouldFlush())
				Flush();

			// Manage special characters differently
			if (c == '\n')
			{
				pen.x = 0.f;
				pen.y -= (float)font.GetSize();
				continue;
			}

			// Make sure font has loaded this character
			auto it = font.m_Glyphs.find(c);
			if (it == font.m_Glyphs.cend())
				continue;

			const AltGlyph& glyph = it->second;

			// Position for character on screen
			float x = pen.x + glyph.Bearing.x;
			float y = pen.y - (glyph.Size.y - glyph.Bearing.y);

			// Size for character on screen
			float w = (float)glyph.Size.x;
			float h = (float)glyph.Size.y;

			// Position of character on atlas
			float tx = (float)glyph.Position.x / width;
			float ty = (float)glyph.Position.y / height;

			// Size of character on atlas
			float tw = (float)glyph.Size.x / width;
			float th = (float)glyph.Size.y / height;

			uint32 index = (m_BatchSize++ * 4);

			// Top-Left
			{
				GlyphVertex& v = m_Vertices[index++];
				v.Position = glm::vec3(x, y + h, depth);
				v.Index = atlas;
				v.Color = color;
				v.TexCoords = glm::vec2(tx, ty + th);
			}

			// Bottom-Left
			{
				GlyphVertex& v = m_Vertices[index++];
				v.Position = glm::vec3(x, y, depth);
				v.Index = atlas;
				v.Color = color;
				v.TexCoords = glm::vec2(tx, ty);
			}

			// Bottom-Right
			{
				GlyphVertex& v = m_Vertices[index++];
				v.Position = glm::vec3(x + w, y, depth);
				v.Index = atlas;
				v.Color = color;
				v.TexCoords = glm::vec2(tx + tw, ty);
			}

			// Top-Right
			{
				GlyphVertex& v = m_Vertices[index];
				v.Position = glm::vec3(x + w, y + h, depth);
				v.Index = atlas;
				v.Color = color;
				v.TexCoords = glm::vec2(tx + tw, ty + th);
			}

			pen.x += (glyph.Advance >> 6);
		}
	}

	bool TextBatchRenderer::ShouldFlush() const
	{
		return m_BatchSize >= m_MaxBatchSize || m_FontsInBatch >= MaxFontsPerBatch;
	}

	uint32 TextBatchRenderer::AddFont(const AltFont& font)
	{
		assert(font.IsValid());

		uint32 handle = font.GetHandle();

		// Check if this font is already set for use
		for (uint32 i = 0; i < m_FontsInBatch; ++i)
		{
			if (m_AtlasHandles[i] == handle)
				return i;
		}

		// Flush if out of space
		if (m_FontsInBatch >= MaxFontsPerBatch)
			Flush();

		m_AtlasHandles[m_FontsInBatch] = handle;
		return m_FontsInBatch++;
	}
}