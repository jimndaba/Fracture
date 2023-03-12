#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Fracture
{	
	class Logger
	{

		static std::shared_ptr<spdlog::logger> m_logger;

	public:
		Logger();


		static spdlog::logger* GetLogger()
		{
			return m_logger.get();
		}	

	};

	// Logger Macros
#define FRACTURE_TRACE(...) ::Fracture::Logger::GetLogger()->trace(__VA_ARGS__)
#define FRACTURE_INFO(...) ::Fracture::Logger::GetLogger()->info(__VA_ARGS__)
#define FRACTURE_WARN(...) ::Fracture::Logger::GetLogger()->warn(__VA_ARGS__)
#define FRACTURE_ERROR(...) ::Fracture::Logger::GetLogger()->error(__VA_ARGS__)
#define FRACTURE_CRITICAL(...) ::Fracture::Logger::GetLogger()->critical(__VA_ARGS__)
}

#endif