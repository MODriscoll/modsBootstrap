#pragma once

#include "Types.h"

#include <glm\fwd.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace mods
{
	class ShaderProgram;

	enum class eShaderType : uint32
	{
		Vertex			= 0x8B31, 
		TessControl		= 0x8E88, 
		TessEvaluation	= 0x8E87,
		Geometry		= 0x8DD9,
		Fragment		= 0x8B30
	};

	// The source path for loading a shader
	struct ShaderSource
	{
	protected:

		ShaderSource(eShaderType type, const std::string& path)
			: Type(type), Path(path)
		{

		}

	public:

		// The type of this shader
		const eShaderType Type;

		// The path to this shaders source
		const std::string Path;
	};

	struct VertexShaderSource : public ShaderSource
	{
	public:

		VertexShaderSource(const std::string& path)
			: ShaderSource(eShaderType::Vertex, path)
		{

		}
	};
	struct TessControlShaderSource : public ShaderSource
	{
	public:

		TessControlShaderSource(const std::string& path)
			: ShaderSource(eShaderType::TessControl, path)
		{

		}
	};
	struct TessEvaluationShaderSource : public ShaderSource
	{
	public:

		TessEvaluationShaderSource(const std::string& path)
			: ShaderSource(eShaderType::TessEvaluation, path)
		{

		}
	};
	struct GeometryShaderSource : public ShaderSource
	{
	public:

		GeometryShaderSource(const std::string& path)
			: ShaderSource(eShaderType::Geometry, path)
		{

		}
	};
	struct FragmentShaderSource : public ShaderSource
	{
	public:

		FragmentShaderSource(const std::string& path)
			: ShaderSource(eShaderType::Fragment, path)
		{

		}
	};

	// Struct for constructing a new shader program
	struct ShaderProgramConstructor final
	{
	private:

		enum eShaderMask : byte
		{
			Vertex			= 1 << 0,
			TessControl		= 1 << 1,
			TessEveluation	= 1 << 2,
			Geometry		= 1 << 3,
			Fragment		= 1 << 4,

			// These shaders are required for a
			// program to be successfully constructed
			Required		= Vertex | Fragment
		};

	public:

		ShaderProgramConstructor()
			: m_Mask(0)
		{
			// Max possible amount of shaders
			m_Shaders.reserve(5u);
		}

	public:

		// Loads and compiles the given type of shader from a file.
		void LoadShader(eShaderType type, const std::string& path);

		// Loads and compiles the given shader source
		void LoadShader(const ShaderSource& source);

		// Links all of the shaders into a single program.
		// An invalid program is returned on failure
		std::shared_ptr<ShaderProgram> Construct();

	private:

		// Loads the source of a shader from the given path
		bool LoadSource(const std::string& path, std::string& source) const;

		// Compiles the given source into a shader
		bool CompileSource(eShaderType type, const std::string& source, uint32& shader) const;

	private:

		// Returns the mask value for the given type
		constexpr eShaderMask GetBitForType(eShaderType type) const;

		// Returns a readable string for the given type
		constexpr const char* GetStringForType(eShaderType type) const;

	private:

		// Bitmask of added shader types
		byte m_Mask;

		// All compiled shaders
		std::vector<uint32> m_Shaders;
	};

	// Wrapper for a shader program
	class ShaderProgram
	{
		friend struct ShaderProgramConstructor;

	private:

		ShaderProgram(uint32 program);

	public:

		ShaderProgram(const ShaderProgram& rhs) = delete;
		ShaderProgram(ShaderProgram&& rhs);

		~ShaderProgram();

		ShaderProgram& operator = (const ShaderProgram& rhs) = delete;
		ShaderProgram& operator = (ShaderProgram&& rhs);

	public:

		// Binds this program for use
		void Bind();

		// Unbinds this program for use
		void Unbind();

	public:

		inline bool IsValid() const { return m_Program != 0; }

	public:

		// Sets the uniform with the given name to the given value
		void SetUniformValue(const std::string& name, bool value);
		void SetUniformValue(const std::string& name, int32 value);
		void SetUniformValue(const std::string& name, float value);
		void SetUniformValue(const std::string& name, const glm::vec2& value);
		void SetUniformValue(const std::string& name, const glm::vec3& value);
		void SetUniformValue(const std::string& name, const glm::vec4& value);
		void SetUniformValue(const std::string& name, const glm::mat4& value, bool transpose = false);

	private:

		// Retrieves the location of the given uniform.
		// Saves location for later indexing if uniform exists
		bool GetUniformLocation(const std::string& name, int32& location);

	private:

		// Handle to the program
		uint32 m_Program;

		// Locations for the uniform values that have been set
		std::unordered_map<std::string, int32> m_Uniforms;
	};
}