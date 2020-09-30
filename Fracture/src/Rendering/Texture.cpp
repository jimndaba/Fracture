#include "Texture.h"
#include "GLAD/glad.h"

Fracture::Texture::Texture(Fracture::TextureType Type):textureType(Type)
{
	glGenTextures(1, &id);
}

Fracture::Texture::Texture(std::string name, Fracture::TextureType Type):Name(name),textureType(Type)
{
	
	glGenTextures(1, &id);

}

Fracture::Texture::Texture(std::string name,int Width, int Height,Fracture::TextureType mtype):Name(name)
{

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void Fracture::Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Fracture::Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Fracture::Texture::Resize(int width, int height)
{
	Bind();	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
}
