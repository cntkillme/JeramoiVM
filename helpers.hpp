#pragma once

#include "config.hpp"

/// An interrupt type that does nothing.
struct trivial_interrupt
{
	template<typename Context>
	struct handler
	{
		using execute = Context;
	};
};
