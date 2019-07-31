#pragma once

#include "type_list.hpp"
#include "value_list.hpp"

constexpr index_t MEMORY_SIZE = 128; ///> memory allocated to the context in bytes (default: 128 bytes)
constexpr index_t INTERRUPT_COUNT = 32; ///> number of interrupts supported (default: 32)

using word = unsigned short; ///> unsigned word
using sword = signed short; ///> signed word

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

struct trivial_interrupt
{
	template<typename Context>
	struct handler
	{
		using execute = Context; ///> Do nothing for default ISR
	};
};

template<typename Memory = make_value_list<word, (MEMORY_SIZE/sizeof(word))>,
	typename Registers = make_value_list<word, 16>,
	typename Interrupts = make_type_list<trivial_interrupt, INTERRUPT_COUNT>,
	word pc = 0>
struct context
{
	template<index_t Register>
	static inline constexpr word read_register = Registers::template value<Register>;

	template<index_t Address>
	static inline constexpr word read_memory = Memory::template value<Address>;

	template<index_t Register, word Value>
	using write_register = context<Memory, typename Registers::template replace<Register, Value>, Interrupts, pc>;

	template<index_t Address, word Value>
	using write_memory = context<typename Memory::template replace<Address, Value>, Registers, Interrupts, pc>;

	template<index_t Interrupt, typename Handler>
	using register_interrupt = context<Memory, Registers, typename Interrupts::template replace<Interrupt, Handler>, pc>;
};
