#include <print>

#include "ConsoleTextSink.h"

void ConsoleTextSink::Emit(std::string_view text)
{
	std::println("{}", text);
}