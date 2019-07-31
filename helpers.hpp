#pragma once

#include "config.hpp"
#include "type_list.hpp"
#include "value_list.hpp"

/// An interrupt type that does nothing.
struct trivial_interrupt
{
	template<typename Context>
	struct handler
	{
		using execute = Context;
	};
};

template<typename Type, index_t Count, typename TypeList>
struct make_type_list_impl :
	make_type_list_impl<Type, Count - 1, typename TypeList::template push_back<Type>> { };

template<typename Type, typename TypeList>
struct make_type_list_impl<Type, 0, TypeList>
{
	using type = TypeList;
};

template<typename Type, index_t Count, typename ValueList>
struct make_value_list_impl :
	make_value_list_impl<Type, Count - 1, typename ValueList::template push_back<static_cast<Type>(0)>> { };

template<typename Type, typename ValueList>
struct make_value_list_impl<Type, 0, ValueList>
{
	using type = ValueList;
};

template<typename Type, index_t Count>
using make_type_list = typename make_type_list_impl<Type, Count, type_list<>>::type;

template<typename Type, index_t Count>
using make_value_list = typename make_value_list_impl<Type, Count, value_list<>>::type;

using make_context = context<make_value_list<word, 16>,
	make_value_list<word, MEMORY_SIZE>,
	make_type_list<trivial_interrupt, INTERRUPT_COUNT>,
	0>;
