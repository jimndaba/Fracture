#pragma once
#ifndef HELPERS_H
#define HELPERS_H


namespace Fracture
{
	namespace Helper
	{

		void SplitPath(const std::string& fullPath, std::string& dir, std::string& fileName);

		std::string GetFileNameFromPath(const std::string& fullPath);

		std::string GetDirectoryFromPath(const std::string& fullPath);

		std::string GetExtensionFromFileName(const std::string& filename);

		std::string RemoveExtension(const std::string& filename);

		std::string RemoveIllegalCharFromString(const std::string& filename);

		std::string ReplaceIllegalCharFromString(const std::string& filename, const std::string& with);
	}
}

#endif