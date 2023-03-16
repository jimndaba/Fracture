#pragma once
#ifndef IMAGELOADER_H
#define IMAGELOADER_H

namespace Fracture
{
	struct Texture;

	class ImageLoader
	{
	public:
		ImageLoader();
		
		static std::shared_ptr<Texture> LoadTexture(const std::string& path);

		

	};

}


#endif