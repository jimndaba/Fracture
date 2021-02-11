#include "Texture.h"


Fracture::Texture::Texture(Fracture::TextureType Type):textureType(Type), mTexturetarget(TextureTarget::Texture2D)
{
	glGenTextures(1, &id);
}

Fracture::Texture::Texture(std::string name, Fracture::TextureType Type):Name(name),textureType(Type), mTexturetarget(TextureTarget::Texture2D)
{
	
	glGenTextures(1, &id);

}

Fracture::Texture::Texture(std::string name,int Width, int Height,Fracture::TextureType mtype):Name(name), mTexturetarget(TextureTarget::Texture2D)
{
	mTexturetarget =TextureTarget::Texture2D;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

Fracture::Texture::Texture(std::string name, int Width, int Height, GLenum internalFormat, GLenum format, GLenum ftype, Fracture::TextureType mtype):
	Name(name),textureType(mtype),
	mTexturetarget(TextureTarget::Texture2D),
	width(Width),
	height(Height)
{

	InternalFormat = internalFormat;
	Type = ftype;
	Format = format;
	mTexturetarget = TextureTarget::Texture2D;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	if (mtype == TextureType::ColorAttachment)
	{
		InternalFormat = GL_RGBA;
		if (Type == GL_HALF_FLOAT)
			InternalFormat = GL_RGBA16F;
		else if (Type == GL_FLOAT)
			InternalFormat = GL_RGBA32F;

		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, Width, Height, 0, format, Type, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//glGenerateMipmap(GL_TEXTURE_2D);

	}
	if (mtype == TextureType::DepthStencilAttachment)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,	Width, Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//glGenerateMipmap(GL_TEXTURE_2D);
	
		
	}

	glBindTexture(GL_TEXTURE_2D, 0);


}

Fracture::Texture::Texture(std::string name, int Width, int Height, Fracture::TextureType mtype,void* data) :Name(name), mTexturetarget(TextureTarget::Texture2D)
{
	mTexturetarget = TextureTarget::Texture2D;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, Width, Height, 0, GL_RGB, GL_FLOAT,data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

}

Fracture::Texture::Texture(std::string name, int Width, int Height, TextureTarget target, GLenum internalFormat, GLenum format, GLenum ftype, Fracture::TextureType mtype):
	Name(name), mTexturetarget(TextureTarget::Texture2D),width(Width),height(Height)
{
	InternalFormat = internalFormat;
	Type = ftype;
	Format = format;
	GLenum texturetarget;

	if (target == TextureTarget::Texture2D)
	{
		texturetarget = GL_TEXTURE_2D;
	}
	if (target == TextureTarget::MultiSample)
	{
		texturetarget = GL_TEXTURE_2D_MULTISAMPLE;
	}
	if (target == TextureTarget::CubeMap)
	{
		texturetarget = GL_TEXTURE_CUBE_MAP;
	}


	glGenTextures(1, &id);
	glBindTexture(texturetarget, id);
	if (mtype == TextureType::ColorAttachment)
	{
		InternalFormat = GL_RGBA;
		if (Type == GL_HALF_FLOAT)
			InternalFormat = GL_RGBA16F;
		else if (Type == GL_FLOAT)
			InternalFormat = GL_RGBA32F;

		if (target == TextureTarget::Texture2D)
		{
			glTexImage2D(texturetarget, 0, InternalFormat, Width, Height, 0, format, Type, NULL);
			glTexParameteri(texturetarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(texturetarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(texturetarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(texturetarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			glTexParameterfv(texturetarget, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		if (target == TextureTarget::MultiSample)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,4, InternalFormat, Width, Height, GL_TRUE);
		}
			
		
	}
	if (mtype == TextureType::DepthStencilAttachment && target == TextureTarget::Texture2D)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Width, Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);		
		
	}
	if (mtype == TextureType::DepthStencilAttachment && target == TextureTarget::CubeMap)
	{
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	glBindTexture(texturetarget, 0);

}

void Fracture::Texture::Bind()
{
	if (mTexturetarget == TextureTarget::Texture2D)
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}
	if (mTexturetarget == TextureTarget::MultiSample)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, id);
	}
	if (mTexturetarget == TextureTarget::CubeMap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP,id);;
	}

}

void Fracture::Texture::Unbind()
{	
	if (mTexturetarget == TextureTarget::Texture2D)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	if (mTexturetarget == TextureTarget::MultiSample)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	}
}

void Fracture::Texture::Resize(int mwidth, int mheight)
{
	if (mTexturetarget == TextureTarget::Texture2D)
	{
		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, mwidth, mheight, 0, Format, Type, 0);
	}
	if (mTexturetarget == TextureTarget::MultiSample)
	{
		Bind();
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, InternalFormat, mwidth, mheight, GL_TRUE);
	}
	
	width = mwidth;
	height = mheight;
	Unbind();
}
