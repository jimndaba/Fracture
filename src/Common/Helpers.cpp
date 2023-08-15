#include "FracturePCH.h"
#include "Helpers.h"

namespace Fracture
{
	namespace Helper
	{
		void SplitPath(const std::string& fullPath, std::string& dir, std::string& fileName)
		{
			size_t found;
			found = fullPath.find_last_of("/\\");
			dir = fullPath.substr(0, found + 1);
			fileName = fullPath.substr(found + 1);
		}

		std::string GetFileNameFromPath(const std::string& fullPath)
		{
			if (fullPath.empty())
			{
				return fullPath;
			}

			std::string ret, empty;
			SplitPath(fullPath, empty, ret);
			return ret;
		}

		std::string GetDirectoryFromPath(const std::string& fullPath)
		{
			if (fullPath.empty())
			{
				return fullPath;
			}

			std::string ret, empty;
			SplitPath(fullPath, ret, empty);
			return ret;
		}

		std::string GetExtensionFromFileName(const std::string& filename)
		{
			size_t idx = filename.rfind('.');

			if (idx != std::string::npos)
			{
				std::string extension = filename.substr(idx + 1);
				return extension;
			}

			// No extension found
			return "";
		}

		std::string RemoveExtension(const std::string& filename)
		{
			size_t idx = filename.rfind('.');

			if (idx == std::string::npos)
			{
				// extension not found:
				return filename;
			}
			return filename.substr(0, idx);
		}

		std::string RemoveIllegalCharFromString(const std::string& filename)
		{
			std::string s = filename;
			std::string illegalChars = "\\/:?\"<>|";
			std::string::iterator it;
			for (std::string::iterator it = s.begin(); it < s.end(); ++it) {
				bool found = illegalChars.find(*it) != std::string::npos;
				if (found) {
					*it = ' ';
				}
			}
			return s;
		}
		std::string ReplaceIllegalCharFromString(const std::string& filename, const std::string& with)
		{
			std::string s = filename;
			std::string illegalChars = "\\/:?\"<>|";
			std::string::iterator it;
			for (std::string::iterator it = s.begin(); it < s.end(); ++it) {
				bool found = illegalChars.find(*it) != std::string::npos;
				if (found) {
					*it = *with.c_str();
				}
			}
			return s;
		}
	}
}