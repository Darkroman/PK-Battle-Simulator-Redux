#pragma once

#include "interfaces/ITextSink.h"

class ConsoleTextSink final : public ITextSink
{
public:
	void Emit(std::string_view text) override;
};