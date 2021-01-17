#pragma once
#ifndef RENDERSINK_H
#define RENDERSINK_H

#include <string>


namespace Fracture
{
	class RenderSource;

	class RenderSink
	{

	public:
		RenderSink(std::string Name);
		~RenderSink();

		void SetTarget(std::string OutputSource, std::string TargetPass);

		const std::string& GetRegisteredName() const;
		const  std::string& GetSourceName() const ;
		const  std::string& GetTargetPassName() const;
		virtual void PostLinkValidate();
		virtual void Bind(RenderSource& source) = 0;

	private:
		std::string registeredName;
		std::string sourceName;
		std::string targetPass;
		

	};

}

#endif