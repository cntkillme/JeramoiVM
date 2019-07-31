#pragma once

#include "config.hpp"

namespace opcodes
{
	template<word reg0, word reg1>
	struct MOV
	{
		template<typename Context>
		using execute = context<
			typename Context::template write_register<reg0, Context::template read_register<reg1>>,
			typename Context::memory,
			typename Context::interrupts,
			Context::pc
		>;
	};

	struct NOP
	{
		template<typename Context>
		using execute = Context;
	};
}
