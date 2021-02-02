#pragma once
#ifndef LOGGER_H
#define LOGGER_H


#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include <memory>

namespace Fracture
{
	class Logger
	{

	public:
		Logger();
		~Logger();

		static std::shared_ptr<spdlog::logger> GetLogger()
		{
			return m_logger;
		}

	private:

		static std::shared_ptr<spdlog::logger> m_logger;

	};

	// Logger Macros

#define FRACTURE_TRACE(...) ::Fracture::Logger::GetLogger()->trace(__VA_ARGS__)
#define FRACTURE_INFO(...) ::Fracture::Logger::GetLogger()->info(__VA_ARGS__)
#define FRACTURE_WARN(...) ::Fracture::Logger::GetLogger()->warn(__VA_ARGS__)
#define FRACTURE_ERROR(...) ::Fracture::Logger::GetLogger()->error(__VA_ARGS__)
#define FRACTURE_CRITICAL(...) ::Fracture::Logger::GetLogger()->critical(__VA_ARGS__)
}

#endif