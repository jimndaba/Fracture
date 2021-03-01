#include "Core/FractureCore.h"
#include "Profiler.h"

Fracture::Profiler::Profiler():m_currentSession(nullptr), m_profileCount(0)
{
}

void Fracture::Profiler::BeginSession(const std::string& name, const std::string& filepath)
{
	if(Fracture_Profiling)
	{
		m_OutputStream.open(filepath);
		WriteHeader();
		m_currentSession = new ProfilerSession(name);
	}
}

void Fracture::Profiler::EndSession()
{
	if (Fracture_Profiling)
	{
		WriteFooter();
		m_OutputStream.close();
		delete m_currentSession;
		m_currentSession = nullptr;
		m_profileCount = 0;
	}
}

void Fracture::Profiler::WriteProfile(const ProfilerResult& result)
{
	if (Fracture_Profiling)
	{
		if (m_profileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":0,";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";
	}
}

void Fracture::Profiler::WriteHeader()
{
	if (Fracture_Profiling)
	{
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}
}

void Fracture::Profiler::WriteFooter()
{
	if (Fracture_Profiling)
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}
}

Fracture::ProfilerTimer::ProfilerTimer(const char* name) :m_name(name)
{
	if (Fracture_Profiling)
	{
		m_StartPoint = std::chrono::high_resolution_clock::now();
	}
}

Fracture::ProfilerTimer::~ProfilerTimer()
{
	if (Fracture_Profiling)
	{
		if (!m_Stopped)
		{
			Stop();
		}
	}
}

void Fracture::ProfilerTimer::Stop()
{
	if (Fracture_Profiling)
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		Profiler::Get().WriteProfile({ m_name,start,end });

		m_Stopped = true;
	}
}
