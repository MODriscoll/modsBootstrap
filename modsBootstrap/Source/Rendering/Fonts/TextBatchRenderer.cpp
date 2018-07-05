#include "Rendering\Fonts\TextBatchRenderer.h"

#include "IncludeGLFW.h"

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

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, VertexSize, nullptr, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexSize, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Position));
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Depth));
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Index));
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, Color));
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphVertex), (void*)offsetof(GlyphVertex, TexCoords));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		m_Vertices.resize(m_MaxBatchSize * 4);
		m_Indices.resize(m_MaxBatchSize * 6);
		
		// Pre-calculate indices as they are unlikely to change
		// TODO: maybe use static draw and set indices initially
		for (uint32 i = 0, v = 0; i < m_Indices.size(); v += 4)
		{
			m_Indices[i++] = v + 0;
			m_Indices[i++] = v + 1;
			m_Indices[i++] = v + 2;

			m_Indices[i++] = v + 0;
			m_Indices[i++] = v + 2;
			m_Indices[i++] = v + 3;
		}
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
		m_Indices.clear();
		
		m_AtlasHandles.fill(0);
		m_FontsInBatch = 0;

		return true;
	}

	void TextBatchRenderer::Flush()
	{
		if (!IsValid())
			return;

		// Is there anything to flush?
		if (m_Vertices.empty() || m_Indices.empty())
			return;

		m_Program.Bind();

		// Set up font uniform values
		{
			std::string uniform("atlas[");

			for (uint32 i = 0; i < m_FontsInBatch; ++i)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_AtlasHandles[i]);
				m_Program.SetUniformValue(uniform + std::to_string(i) + ']', i);
			}
		}

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GlyphVertex) * m_Vertices.size(), m_Vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32) * m_Indices.size(), m_Indices.data());

		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_Program.Unbind();

		m_Vertices.clear();
		m_Indices.clear();

		m_AtlasHandles.fill(0);
		m_FontsInBatch = 0;
	}

	void TextBatchRenderer::Print(const std::string& text, const AltFont& font, const glm::vec2& position, const glm::vec4& color, float depth)
	{
		if (!IsValid())
			return;

		glm::vec2 pen = position;
		
		// Add atlas now, track its index to give to vertices
		uint32 atlas = AddFont(font);

		for (char c : text)
		{
			if (ShouldFlush())
				Flush();

			// Make sure font has loaded this character
			auto it = font.m_Glyphs.find(c);
			if (it == font.m_Glyphs.cend())
				continue;
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