#include "FracturePCH.h"
#include "Logger.h"



std::shared_ptr<spdlog::logger> Fracture::Logger::m_logger;

Fracture::Logger::Logger()
{
	std::vector<spdlog::sink_ptr> logSinks;
	//spdlogSink_ = std::make_shared<ImGuiSink>();
	//spdlog::set_default_logger(std::make_shared<spdlog::logger>("logger", spdlogSink_));
	logSinks.emplace_back(std::make_shared<ImGuiSink>());
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Fracture.log", true));
	logSinks[0]->set_pattern("%^[%T] %n: %v%$");
	logSinks[1]->set_pattern("%^[%T] %n: %v%$");
	logSinks[2]->set_pattern("[%T] [%l] %n: %v");


	m_logger = std::make_shared<spdlog::logger>("FRACTURE", begin(logSinks), end(logSinks));

	spdlog::register_logger(m_logger);
	m_logger->set_level(spdlog::level::trace);
	m_logger->flush_on(spdlog::level::trace);
}
