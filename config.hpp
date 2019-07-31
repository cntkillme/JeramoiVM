#pragma once

using word = unsigned short; ///> unsigned word
using sword = signed short; ///> signed word

constexpr word MEMORY_SIZE = 64; ///> memory allocated to the context in words (default: 64 words)
constexpr word INTERRUPT_COUNT = 32; ///> number of interrupts supported (default: 32)

template<typename Registers, typename Memory, typename Interrupts, word PC>
struct context;
