#pragma once

#include "config.hpp"

namespace opcodes
{
	template<word Reg0, word Reg1>
	struct MOV
	{
		template<typename Context>
		using execute = typename Context
			::template write_register<Reg0, Context::template read_register<Reg1>>;
	};

	template<word Base, sword Disp>
	struct LEA
	{
		template<typename Context>
		using execute = typename Context
			::template write_register<0, Context::template read_register<Base> + Disp>;
	};

	template<word Base, sword Disp>
	struct LD
	{
		template<typename Context>
		using execute = typename Context::template write_register<0, Context::template read_memory<
			Context::template read_register<Base> + Disp>>;
	};

	template<sword Value>
	struct LDi
	{
		template<typename Context>
		using execute = typename Context::template write_register<0, Value>;
	};

	template<word Base, sword Disp>
	struct ST
	{
		template<typename Context>
		using execute = typename Context::template write_memory<Context::template read_register<Base> + Disp,
			Context::template read_register<0>>;
	};

	template<sword Interrupt>
	struct INT
	{
		template<typename Context>
		using execute = typename Context::interrupts::template type<Interrupt>::execute;
	};

	struct HLT
	{
		template<typename Context>
		using execute = halted_context<Context>;
	};

	struct NOP
	{
		template<typename Context>
		using execute = Context;
	};
}
