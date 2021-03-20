#pragma once
#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <memory>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H  

namespace Fracture
{
	class Font;


	class FontLoader
	{

	public:
		FontLoader();
		~FontLoader() = default;

		static std::shared_ptr<Font> AddFont(const std::string& name, const std::string& path);

	};


}

#endif