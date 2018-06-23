#pragma once

#include "Types.h"

#include <glm\vec2.hpp>

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

	public:

		int32 m_Width, m_Height;

		// Handle to atlas
		uint32 m_Handle;

		// Height (size) of the texture
		//int32 m_Height;

		// Only ascii for now
		std::map<char, AltGlyph> m_Glyphs;
	};
}