#pragma once
#ifndef FONT_H
#define FONT_H

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <memory>

namespace Fracture
{
	class VertexArray;
	class VertexBuffer;

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	class Font
	{
	public:
		Font(const std::string& name, const std::string& path);

		std::string GetName();
		std::string GetPath();
		std::map<char, Character> GetCharacters();

		void AddCharacter(const char c, const Character& cha);

		std::shared_ptr<VertexArray> GetVAO();
		std::shared_ptr<VertexBuffer> GetBuffer();
		void bind();
		void unbind();
	


	private:
		std::string m_Name;
		std::string m_Path;
		std::map<char, Character> m_Characters;
		std::shared_ptr<VertexArray> m_vao;
		std::shared_ptr<VertexBuffer> m_vbo;
	};

}

#endif