#pragma once

#include "helpers.hpp"
#include "decoder.hpp"

template<typename Registers, typename Memory, typename Interrupts, word PC>
struct context
{
	using registers = Registers;
	using memory = Memory;
	using interrupts = Interrupts;
	static constexpr word pc = PC;

	/**
	 * \brief Reads a value stored in the given register.
	 * \tparam Register the register index, between 0 and 16 exclusive.
	 */
	template<word Register>
	static inline constexpr word read_register = Registers::template value<Register>;

	/**
	 * \brief Reads a value stored in memory at the given address.
	 * \tparam Address the address, between 0 and MEMORY_SIZE exclusive.
	 */
	template<word Address>
	static inline constexpr word read_memory = Memory::template value<Address>;

	/**
	 * \brief Writes a value to the given register.
	 * \tparam Register the register index, between 0 and 16 exclusive.
	 * \tparam Value the value to write.
	 */
	template<word Register, word Value>
	using write_register = context<typename Registers::template replace<Register, Value>,
		Memory, Interrupts, pc>;

	/**
	 * \brief Writes a value to memory at the given address.
	 * \tparam Address the address, between 0 and MEMORY_SIZE exclusive.
	 * \tparam Value the value to write.
	 */
	template<word Address, word Value>
	using write_memory = context<Registers,
		typename Memory::template replace<Address, Value>, Interrupts, pc>;

	/**
	 * \brief Registers an interrupt handler to the given interrupt.
	 * \tparam Interrupt the interrupt number, between 0 and INTERRUPT_COUNT exclusive.
	 * \tparam Handler the handler.
	 */
	template<word Interrupt, typename Handler>
	using register_interrupt = context<Registers, Memory,
		typename Interrupts::template replace<Interrupt, Handler>, pc>;

	/**
	 * \brief Decodes the next instruction and increments the program counter.
	 */
	using decode = decode_instruction<Memory::template value<pc>>;
};
