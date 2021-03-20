#include "FontLoader.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Font.h"
#include "Logging/Logger.h"

Fracture::FontLoader::FontLoader()
{

}

std::shared_ptr<Fracture::Font> Fracture::FontLoader::AddFont(const std::string&  name,const std::string& path)
{
    std::shared_ptr<Font> font = std::make_shared<Font>(name,path);
    FT_Library ft;
    FT_Error error;
    if (FT_Init_FreeType(&ft))
    {
        FRACTURE_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face;
   
    error = FT_New_Face(ft, path.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format)
    {
        FRACTURE_ERROR("ERROR::FREETYPE: Unkown file format");
    }
    else if (error)
    {
        FRACTURE_ERROR("ERROR::FREETYPE: Failed to load font");
    }

    FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            FRACTURE_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };

        font->AddCharacter(c, character);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

	return font;
}
