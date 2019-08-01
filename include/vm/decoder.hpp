#pragma once

#include "config.hpp"
#include "opcodes.hpp"

/**
 * Instruction formats:
 * iA: null(10)opcode(6)
 * iB: null(2)reg1(4)reg0(4)opcode(6)
 * iC: disp(6)base(4)opcode(6)
 * iD: imm(10) opcode(6)
 */

struct BAD_OPCODE { };

template<word opcode, word rest>
struct decode_instruction_impl { using type = BAD_OPCODE; };

#define iA(opcode, ophandler) template<word rest> \
struct decode_instruction_impl<opcode, rest> { \
	using type = opcodes::ophandler; \
}

#define iB(opcode, ophandler) template<word rest> \
struct decode_instruction_impl<opcode, rest> { \
	using type = opcodes::template ophandler<(rest&0xF), ((rest>>4)&0xF)>; \
}

#define iC(opcode, ophandler) template<word rest> \
struct decode_instruction_impl<opcode, rest> { \
	using type = opcodes::template ophandler<(rest&0xF), (static_cast<sword>(rest)>>4)>; \
}

#define iD(opcode, ophandler) template<word rest> \
struct decode_instruction_impl<opcode, rest> { \
	using type = opcodes::template ophandler<static_cast<sword>(rest)>; \
}

iB(0x00, MOV);
iC(0x01, LEA); iC(0x02, LD); iD(0x03, LDi); iC(0x04, ST);
/*iB(0x05, PUSH); iB(0x06, POP);
iB(0x07, ADD); iB(0x08, SUB); iB(0x09, MUL); iB(0x0A, DIV); iB(0x0B, MOD);
iB(0x0C, NOT); iB(0x0D, OR); iB(0x0E, AND); iB(0x0F, XOR);
iD(0x10, JMP); iD(0x11, CALL); iA(0x12, RET); iC(0x13, GOTO);
iB(0x14, EQ); iD(0x15, EQi);
iB(0x16, NEQ); iD(0x17, NEQi);
iB(0x18, LT); iD(0x19, LTi);
iB(0x1A, LE); iD(0x1B, LEi);
iB(0x1C, GT); iD(0x1D, GTi);
iB(0x1E, GE); iD(0x1F, GEi);*/
iD(0x20, INT);
iA(0x21, HLT);
iA(0x22, NOP);

#undef iA
#undef iB
#undef iC
#undef iD

template<word Instruction>
using decode_instruction = typename decode_instruction_impl<(Instruction&0x3F), (Instruction>>6)>::type;
