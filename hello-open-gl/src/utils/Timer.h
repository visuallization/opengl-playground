#pragma once

#include <chrono>
#include <iostream>

class Timer {
public:
	Timer()
		: m_Start(std::chrono::high_resolution_clock::now())
		, m_Duration(0.0f)
	{}

	~Timer() {
		Stop();
	}

	void Start() {
		m_Start = std::chrono::high_resolution_clock::now();
	}

	void Stop() {
		m_End = std::chrono::high_resolution_clock::now();
		m_Duration = m_End - m_Start;

		float ms = m_Duration.count() * 1000;
		std::cout << "Timer took " << ms << " ms" << std::endl;
	}

private:
	std::chrono::steady_clock::time_point m_Start, m_End;
	std::chrono::duration<float> m_Duration;
};