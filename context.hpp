#pragma once

#include "type_list.hpp"
#include "value_list.hpp"

constexpr index_t MEMORY_SIZE = 64; ///> memory allocated to the context in words (default: 64 words)
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

template<typename Registers = make_value_list<word, 16>,
	typename Memory = make_value_list<word, MEMORY_SIZE>,
	typename Interrupts = make_type_list<trivial_interrupt, INTERRUPT_COUNT>,
	word pc = 0>
struct context
{
	/**
	 * \brief Reads a value stored in the given register.
	 * \tparam Register the register index, between 0 and 16 exclusive.
	 */
	template<index_t Register>
	static inline constexpr word read_register = Registers::template value<Register>;

	/**
	 * \brief Reads a value stored in memory at the given address.
	 * \tparam Address the address, between 0 and MEMORY_SIZE exclusive.
	 */
	template<index_t Address>
	static inline constexpr word read_memory = Memory::template value<Address>;

	/**
	 * \brief Writes a value to the given register.
	 * \tparam Register the register index, between 0 and 16 exclusive.
	 * \tparam Value the value to write.
	 */
	template<index_t Register, word Value>
	using write_register = context<typename Registers::template replace<Register, Value>,
		Memory, Interrupts, pc>;

	/**
	 * \brief Writes a value to memory at the given address.
	 * \tparam Address the address, between 0 and MEMORY_SIZE exclusive.
	 * \tparam Value the value to write.
	 */
	template<index_t Address, word Value>
	using write_memory = context<Registers,
		typename Memory::template replace<Address, Value>, Interrupts, pc>;

	/**
	 * \brief Registers an interrupt handler to the given interrupt.
	 * \tparam Interrupt the interrupt number, between 0 and INTERRUPT_COUNT exclusive.
	 * \tparam Handler the handler.
	 */
	template<index_t Interrupt, typename Handler>
	using register_interrupt = context<Registers, Memory,
		typename Interrupts::template replace<Interrupt, Handler>, pc>;
};
