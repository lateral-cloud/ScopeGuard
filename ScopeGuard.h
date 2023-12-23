#pragma once

#include <functional>
#include <chrono>
#include "print.h"

struct ScopeGuard
{
	using clock = std::chrono::steady_clock;
	using time_point = std::chrono::time_point<clock, std::chrono::milliseconds>;

protected:
	const std::wstring name;
	const std::function<void()> func;
	time_point constru;

	static time_point NowMs() noexcept
	{
		return std::chrono::time_point_cast<
			std::chrono::milliseconds,
			std::chrono::steady_clock,
			clock::duration>(clock::now());
	}
	std::wstring MakeTimeStr() const
	{
		static const std::wstring beg = L"[time: ", ed = L"ms]";
		time_point tp2 = NowMs();
		return beg + std::to_wstring((tp2 - constru).count()) + ed;
	}
	void PrintBegin()
	{
		static const std::wstring word = L" [" + name + L": begin] ";
		constru = NowMs();
		std::wcout << L"[timept:" << constru.time_since_epoch() << L"]" << word << '\n';
	}
	void PrintEnd() const
	{
		static const std::wstring word = L" [" + name + L": end] ";
		std::wcout << MakeTimeStr() << word << '\n';
	}
	void PrintPrint(const auto&... args) const
	{
		static const std::wstring word = L" [" + name + L": print] ";
		std::wcout << MakeTimeStr() << word;
		print(args...);
	}
	void PrintInput(auto&... args) const
	{
		static const std::wstring word = L" [" + name + L": input] ";
		std::wcout << MakeTimeStr() << word;
		int _[] = { (std::wcin >> args, 0) ... };
	}
	void PrintError(const auto&... args) const
	{
		static const std::wstring word = L" [" + name + L": error] ";
		std::wcout << MakeTimeStr() << word;
		print(args...);
	}

public:
	ScopeGuard(std::wstring_view str) : name(str), func([this]() { PrintEnd(); })
	{
		PrintBegin();
	}
	~ScopeGuard() { func(); }

	void Print(const auto&... args) const
	{
		PrintPrint(args...);
	}
	void Input(auto&... args) const
	{
		PrintInput(args...);
	}
	void Exit(const auto&... args, int code = 0) const
	{
		PrintError(args...);
		std::exit(code);
	}
	void QuickExit(const auto&... args, int code = 0) const
	{
		PrintError(args...);
		std::quick_exit(code);
	}
	void Abort(const auto&... args) const
	{
		PrintError(args...);
		std::abort();
	}
	void Terminate(const auto&... args) const
	{
		PrintError(args...);
		std::terminate();
	}
	void WaitTime(int ms) const
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
	void WaitChar() const
	{
		(void)getchar();
		(void)getchar();
	}
};