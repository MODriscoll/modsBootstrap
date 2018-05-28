#pragma once

#include "Types.h"

namespace mods
{
	enum class eMaterialParameterType : byte
	{
		Bool,
		Int,
		Float,

		Vec2,
		Vec3,
		Vec4,

		Mat2,
		Mat3,
		Mat4,

		Texture2D,

		Cubemap,

		Count
	};

	struct MaterialParameter
	{
	public:

		MaterialParameter()
			: m_Location(-1)
		{

		}

		virtual ~MaterialParameter() = default;

	public:

		// Binds this parameter at its location
		virtual void Bind(uint32 unit = 0) const = 0;

	public:

		// Sets this parameters location
		inline void SetLocation(int32 location) { m_Location = location; }

	public:

		// Get if the parameter and location is valid
		virtual bool IsValid() const { return m_Location >= 0; }

		inline virtual eMaterialParameterType GetParameterType() const = 0;

	protected:

		// Location of the parameters uniform
		int32 m_Location;
	};
}