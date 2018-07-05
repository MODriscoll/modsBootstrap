#pragma once

#include "AltFont.h"
#include "Rendering\Shaders\Shader.h"

#include <array>

namespace mods
{
	class TextBatchRenderer
	{
	public:

		// The max amount of fonts to use per batch
		static constexpr uint32 MaxFontsPerBatch = 16;

	private:

		struct GlyphVertex
		{
		public:

			GlyphVertex()
				: Position(0.f)
				, Index(0)
				, Color(1.f)
				, TexCoords(0.f)
			{

			}

		public:

			// Position of glyph on screen
			glm::vec3 Position;

			// Index to font to use
			int32 Index;

			// Color of the glyph
			glm::vec4 Color;

			// Tex Coords to glyph
			glm::vec2 TexCoords;
		};

	public:

		TextBatchRenderer();
		~TextBatchRenderer();

	public:

		// Creates buffers for batches of given size
		bool Create(uint32 batchSize);

		// Destroys existing buffers if any
		bool Destroy();

		// Flushes the buffers contents
		void Flush();

	public:

		// Add given text to be rendered to screen
		void Print(const std::string& text, const AltFont& font, const glm::ivec2& position, const glm::vec4& color, float depth = 0.f);

	public:

		inline bool IsValid() const { return m_VAO != 0; }

		// Get if current batch should be flushed
		bool ShouldFlush() const;

	private:

		// Adds a new font into the batch, flushing if already full
		// Returns index to newly added font or existing one
		uint32 AddFont(const AltFont& font);

	private:

		// The max amount of characters to draw before flushing
		uint32 m_MaxBatchSize; // Maybe fixed size?

		// Handles to objects
		uint32 m_VAO, m_VBO, m_IBO;

		// All vertices to pass to vertex buffer
		std::vector<GlyphVertex> m_Vertices;

		// Program for rendering text
		// TODO: move outside? (due to projection matrix needing to be set)
		// right now the shader uses the ortho from camera include
		ShaderProgram m_Program;

		// Count for how many characters are in current batch
		uint32 m_BatchSize;

		// Handle to all fonts atlases
		std::array<uint32, MaxFontsPerBatch> m_AtlasHandles;

		// Count for how many fonts are ready for use in current batch
		uint32 m_FontsInBatch;
	};
}