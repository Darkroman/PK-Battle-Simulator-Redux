#pragma once

#include <string_view>

class ITextSink
{
public:
	virtual ~ITextSink() = default;
	virtual void Emit(std::string_view text) = 0;
};