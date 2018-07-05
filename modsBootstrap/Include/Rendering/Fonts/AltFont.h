#pragma once

#include "Types.h"

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>

#include <map>
#include <string>
#include <vector>

namespace mods
{
	struct BinNode
	{
		BinNode()
		{
			Left = Right = -1;
			X = Y = 0;
			Width = Height = 0;
		}

		int32 Left, Right;
		uint32 X, Y;
		uint32 Width, Height;
	};

	struct RectangleBinPack
	{
		std::vector<BinNode> Nodes;
		uint32 Width, Height;

		void Init(uint32 width, uint32 height);
		float Occupancy();
		int32 Insert(uint32 index, uint32 width, uint32 height);

	private:

		// Calculates occupied area of given node and its leaves
		uint32 OccupiedSurfaceArea(const BinNode& node);
	};

	struct AltGlyph
	{		
		glm::ivec2 Position;
		glm::ivec2 TexCoords;

		glm::ivec2 Size;
		glm::ivec2 Bearing;
		uint32 Advance;
	};

	class AltFont
	{
	public:

		AltFont(const std::string& path, int32 size);
		~AltFont();

		void Draw(const std::string& Text, const glm::ivec2& Position, const glm::vec4& color = glm::vec4(1.f));
		void Flush();

	public:

		inline bool IsValid() const { return m_Atlas != 0; }
		inline uint32 GetHandle() const { return m_Atlas; }

		inline int32 GetSize() const { return m_Size; }

		inline int32 GetWidth() const { return m_Width; }
		inline int32 GetHeight() const { return m_Height; }

	public:

		// Handle to atlas
		uint32 m_Atlas;

		//Size of the font
		int32 m_Size;

		// Width and height of the atlas
		int32 m_Width, m_Height;

		// All glyphs contained in the atlas
		std::map<unsigned char, AltGlyph> m_Glyphs;

		uint32 m_VAO, m_VBO, m_IBO;

		struct GlyphVertex
		{
			glm::vec2 Position;
			glm::vec4 Color;
			glm::vec2 TexCoords;
		};

		std::vector<GlyphVertex> m_GlyphsBuffer;
		std::vector<uint32> m_IndexBuffer;
	};
}