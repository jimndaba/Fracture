#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Fracture
{	
    // Custom sink for spdlog that captures logs
    class ImGuiSink : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);
            message_ = fmt::to_string(formatted);

            // Add the log message to the buffer
            buffer_.push_back(message_);

            // Limit the buffer size to a certain number of messages
            constexpr int maxBufferSize = 100;
            while (buffer_.size() > maxBufferSize)
            {
                buffer_.pop_front();
            }
        }
        
        virtual void flush_(){ };

        const std::deque<std::string>& GetBuffer() const { return buffer_; }
        const std::string& GetCurrentMessage() const { return message_; }

    private:
        std::deque<std::string> buffer_;
        std::string message_;
    };


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