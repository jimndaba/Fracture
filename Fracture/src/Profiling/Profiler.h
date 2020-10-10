#pragma once
#ifndef PROFILER_H
#define PROFILER_H

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>

namespace Fracture
{
	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfilerResult
	{
		std::string Name;
		long long Start, End;
	};

	struct ProfilerSession
	{
		std::string Name;

		ProfilerSession(std::string name) :Name(name)
		{

		}
	};

	class Profiler
	{

	private:

		ProfilerSession* m_currentSession;
		std::ofstream m_OutputStream;
		int m_profileCount;

	public:
		Profiler();

		void BeginSession(const std::string& name, const std::string& filepath = "result.json");
		void EndSession();

		void WriteProfile(const ProfilerResult& result);
		void WriteHeader();
		void WriteFooter();

		static Profiler& Get()
		{
			static Profiler* instance = new Profiler();
			return *instance;
		}
	};

	class ProfilerTimer
	{

	private:

		const char* m_name;
		bool m_Stopped;
		std::chrono::steady_clock::time_point m_StartPoint;

	public:
		ProfilerTimer(const char* name);
		~ProfilerTimer();
		void Stop();
	};




}

#endif