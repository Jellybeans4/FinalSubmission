/**************************************************************************
 * C S 429 architecture emulator
 * 
 * instr.c - The top-level entry point for instruction processing.
 * 
 * Most of the details will be written in files in the instr/ subdirectory.
 * 
 * Copyright (c) 2022. S. Chatterjee. All rights reserved.
 * May not be used, modified, or copied without permission.
 **************************************************************************/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "err_handler.h"
#include "instr.h"
#include "machine.h"
#include "instructions.h"

extern machine_t guest;

inline unsigned safe_GETBF(int32_t src, unsigned frompos, unsigned width) {
    return ((((unsigned) src) & (((1 << width) - 1) << frompos)) >> frompos);
}

static inline void init_itable_entry(opcode_t op, unsigned idx) {itable[idx] = op;}

static inline void init_itable_range(opcode_t op, unsigned idx1, unsigned idx2) {
    for (unsigned i = idx1; i <= idx2; i++) itable[i] = op;
}

/*
 * Initialize the itable. Called from interface.c.
 * Do not re-write.
 */

void init_itable(void) {
    for (int i = 0; i < (2<<11); i++) itable[i] = OP_ERROR;
    init_itable_entry(OP_LDURB, 0x1c2U);
    init_itable_entry(OP_LDUR, 0x7c2U);
    init_itable_entry(OP_STURB, 0x1c0U);
    init_itable_entry(OP_STUR, 0x7c0U);
    init_itable_range(OP_MOVK, 0x794U, 0x797U);
    init_itable_range(OP_MOVZ, 0x694U, 0x697U);
    init_itable_range(OP_ADD_RI, 0x488U, 0x48bU);
    init_itable_entry(OP_ADDS_RR, 0x558U);
    init_itable_entry(OP_SUBS_RR, 0x758U);
    init_itable_entry(OP_MVN, 0x551U);
    init_itable_entry(OP_ORR_RR, 0x550U);
    init_itable_entry(OP_EOR_RR, 0x650U);
    init_itable_entry(OP_ANDS_RR, 0x750U);
    init_itable_range(OP_UBFM, 0x69aU, 0x69bU); // LSL, LSR share the same opcode
    init_itable_range(OP_ASR, 0x49aU, 0x49bU);
    init_itable_range(OP_B, 0x0a0U, 0x0bfU);
    init_itable_range(OP_B_COND, 0x2a0U, 0x2a7U);
    init_itable_range(OP_BL, 0x4a0U, 0x4bfU);
    init_itable_entry(OP_RET, 0x6b2U);
    init_itable_entry(OP_NOP, 0x6a8U);
    init_itable_entry(OP_HLT, 0x6a2U);
}

/*
 * Fetch.
 *
 * This is the same for all instructions. Do not re-write.
 */

void fetch_instr(instr_t *const insn) {
    insn->insnbits = mem_read_I(guest.proc->PC.bits->xval);
    return;
}

/*
 * Decode and read operands: top level dispatcher (partially implemented).
 *
 * STUDENT TODO:
 * Finish dispatcher.
 * Finish lower levels in instruction-specific files in the instr/ subdirectory.
 */

void decode_instr(instr_t *const insn) {
    // Don't change the next three lines.
    int32_t instr = insn->insnbits;
    unsigned op = GETBF(instr, 21, 11);
    insn->op = itable[op];

    switch(insn->op) {
        case OP_NONE: assert(false); break;
        case OP_LDURB: decode_LDURB(insn); break;
        case OP_LDUR: decode_LDUR(insn); break;
        case OP_STURB: decode_STURB(insn); break;
        case OP_STUR: decode_STUR(insn); break;
        case OP_MOVK: decode_MOVK(insn); break;
        case OP_MOVZ: decode_MOVZ(insn); break;
        case OP_ADD_RI: decode_ADD_RI(insn); break;
        case OP_ADDS_RR: decode_ADDS_RR(insn); break;
        case OP_SUBS_RR: decode_SUBS_RR(insn); break;
        case OP_MVN: decode_MVN(insn); break;
        case OP_ORR_RR: decode_ORR_RR(insn); break;
        case OP_EOR_RR: decode_EOR_RR(insn); break;
        case OP_ANDS_RR: decode_ANDS_RR(insn); break;
        case OP_LSL: decode_LSL(insn); break;
        case OP_LSR: decode_LSR(insn); break;
        case OP_UBFM: decode_UBFM(insn); break;
        case OP_ASR: decode_ASR(insn); break;

        case OP_B:  decode_B(insn); break;
        case OP_B_COND: decode_B_COND(insn); break;
        case OP_BL:   decode_BL(insn); break;
        case OP_RET:  decode_RET(insn);break;

        case OP_NOP: decode_NOP(insn); break;
        case OP_HLT: decode_HLT(insn); break;
        case OP_ERROR:  break;
    }
    return;
}

/*
 * Execute: top level dispatcher (partially implemented).
 *
 * STUDENT TODO:
 * Finish dispatcher.
 * Finish lower levels in instruction-specific files in the instr/ subdirectory.
 */

void execute_instr(instr_t *const insn) {
    switch(insn->op) {
        case OP_NONE: assert(false); break;
        case OP_LDURB: execute_LDURB(insn); break;
        case OP_LDUR: execute_LDUR(insn); break;
        case OP_STURB: execute_STURB(insn); break;
        case OP_STUR: execute_STUR(insn); break;
        case OP_MOVK: execute_MOVK(insn); break;
        case OP_MOVZ: execute_MOVZ(insn); break;
        case OP_ADD_RI: execute_ADD_RI(insn); break;
        case OP_ADDS_RR: execute_ADDS_RR(insn); break;
        case OP_SUBS_RR: execute_SUBS_RR(insn); break;
        case OP_MVN: execute_MVN(insn); break;
        case OP_ORR_RR: execute_ORR_RR(insn); break;
        case OP_EOR_RR: execute_EOR_RR(insn); break;
        case OP_ANDS_RR: execute_ANDS_RR(insn); break;
        case OP_LSL: execute_LSL(insn); break;
        case OP_LSR: execute_LSR(insn); break;
        case OP_UBFM: execute_UBFM(insn); break;
        case OP_ASR: execute_ASR(insn); break;

        case OP_B:execute_B(insn); break;
        case OP_B_COND: execute_B_COND(insn); break;
        case OP_BL: execute_BL(insn); break;
        case OP_RET: execute_RET(insn); break; // fine for now ????/s

        case OP_NOP: execute_NOP(insn); break;
        case OP_HLT: execute_HLT(insn); break;
        case OP_ERROR: assert(false); break;
    }
    return;
}

/*
 * Access memory: top level dispatcher (partially implemented).
 *
 * STUDENT TODO:
 * Finish dispatcher using lower level routines in the file instr/commmon_memory.c.
 */

void memory_instr(instr_t *const insn) {
    switch(insn->op) {
        case OP_NONE: assert(false); break;
        case OP_LDURB: common_memory_load_LX(insn); break; //bw
        case OP_LDUR: common_memory_load_LX(insn); break;
        case OP_STURB: common_memory_store_XL(insn); break; // wx
        case OP_STUR: common_memory_store_XL(insn); break;
        
        case OP_MOVK: common_memory_none(insn); break;
        case OP_MOVZ: common_memory_none(insn); break;
        case OP_ADD_RI: common_memory_none(insn); break;
        case OP_ADDS_RR: common_memory_none(insn); break;
        case OP_SUBS_RR: common_memory_none(insn); break;
        case OP_MVN: common_memory_none(insn); break;
        case OP_ORR_RR: common_memory_none(insn); break;
        case OP_EOR_RR: common_memory_none(insn); break;
        case OP_ANDS_RR: common_memory_none(insn); break;
        case OP_LSL: common_memory_none(insn); break;
        case OP_LSR: common_memory_none(insn); break;
        case OP_UBFM: common_memory_none(insn); break;
        case OP_ASR: common_memory_none(insn); break;

        case OP_B: common_memory_none(insn); break;
        case OP_B_COND: common_memory_none(insn); break;
        case OP_BL: common_memory_none(insn); break;
        case OP_RET: common_memory_none(insn); break; // OKOKOKOKOK

        case OP_NOP: common_memory_none(insn); break;
        case OP_HLT: common_memory_none(insn); break;
        case OP_ERROR: assert(false); break;
    }
    return;
}

/*
 * Write back to register file: top level dispatcher (partially implemented).
 *
 * STUDENT TODO:
 * Finish dispatcher using lower level routines in the file instr/common_writeback.c.
 */

void wback_instr(instr_t *const insn) {
    switch(insn->op) {
        case OP_NONE: assert(false); break;
        case OP_LDURB: common_writeback_mem_X(insn); break;
        case OP_LDUR:  common_writeback_mem_X(insn); break;
        case OP_STURB: common_writeback_none(insn); break;
        case OP_STUR: common_writeback_none(insn); break;
        case OP_MOVK: common_writeback_alu_X(insn); break;
        case OP_MOVZ: common_writeback_alu_X(insn); break;
        case OP_ADD_RI:common_writeback_alu_X(insn); break;
        case OP_ADDS_RR: common_writeback_alu_X(insn); break;
        case OP_SUBS_RR: common_writeback_alu_X(insn); break;
        case OP_MVN: common_writeback_alu_X(insn); break;
        case OP_ORR_RR: common_writeback_alu_X(insn); break;
        case OP_EOR_RR: common_writeback_alu_X(insn); break;
        case OP_ANDS_RR: common_writeback_alu_X(insn); break;
        case OP_LSL: common_writeback_alu_X(insn); break;
        case OP_LSR: common_writeback_alu_X(insn); break;
        case OP_UBFM: common_writeback_alu_X(insn); break;
        case OP_ASR: common_writeback_alu_X(insn); break;

        case OP_B: common_writeback_none(insn); break;
        case OP_B_COND: common_writeback_none(insn); break;
        case OP_BL: common_writeback_special(insn); break;
        case OP_RET: common_writeback_none(insn); break; 

        case OP_NOP: common_writeback_none(insn); break;
        case OP_HLT: common_writeback_none(insn); break;
        case OP_ERROR: assert(false); break;
    }
    return;
}

/*
 * Update PC: top level dispatcher (partially implemented).
 *
 * STUDENT TODO:
 * Finish dispatcher using lower level routines in the file instr/common_update_pc.c.
 */

void update_pc_instr(instr_t *const insn) {
    switch(insn->op) {
        case OP_NONE: assert(false); break;
        case OP_LDURB: update_pc_next(insn); break;
        case OP_LDUR: update_pc_next(insn); break;
        case OP_STURB: update_pc_next(insn); break;
        case OP_STUR: update_pc_next(insn); break;
        case OP_MOVK: update_pc_next(insn); break;
        case OP_MOVZ: update_pc_next(insn); break;
        case OP_ADD_RI: update_pc_next(insn); break;
        case OP_ADDS_RR: update_pc_next(insn); break;
        case OP_SUBS_RR: update_pc_next(insn); break;
        case OP_MVN: update_pc_next(insn); break;
        case OP_ORR_RR: update_pc_next(insn); break;
        case OP_EOR_RR: update_pc_next(insn); break;
        case OP_ANDS_RR: update_pc_next(insn); break;
        case OP_LSL: update_pc_next(insn); break;
        case OP_LSR: update_pc_next(insn); break;
        case OP_UBFM: update_pc_next(insn); break;
        case OP_ASR: update_pc_next(insn); break;
        case OP_B: update_pc_branch(insn);
            break;
        case OP_B_COND: update_pc_branch(insn);
            break;
        case OP_BL:  update_pc_branch(insn);
            break;
        case OP_RET: update_pc_branch(insn); 
            break;
        case OP_NOP: update_pc_next(insn); break;
        case OP_HLT: update_pc_halt(insn); break;
        case OP_ERROR: assert(false); break;
    }
    return;
}

#ifdef DEBUG
static char *opcode_names[] = {
    "ERR ", 
    "LDURB ",
    "LDUR ",
    "STURB ",
    "STUR ",
    "MOVK ",
    "MOVZ ",
    "ADD ",
    "ADDS ",
    "SUBS ",
    "MVN ",
    "ORR ",
    "EOR ",
    "ANDS ",
    "LSL ",
    "LSR ",
    "UBFM ",
    "ASR ",
    "B ",
    "B.cond ",
    "BL ",
    "RET ",
    "NOP ",
    "HLT ",
};

static char *cond_names[] = {
    "EQ", "NE", "CS", "CC", "MI", "PL", "VS", "VC", 
    "HI", "LS", "GE", "LT", "GT", "LE", "AL", "NV"
};
#endif

/*
 * A debugging aid to print out the fields of an instruction.
 * 
 * Only S_FETCH and S_DECODE stages are fully implemented.
 * The remaining stages are an optional STUDENT TODO.
 * The DEBUG flag will be turned off during auto-grading.
 */

void show_instr(const instr_t * insn, const proc_stage_t stage) {
#ifdef DEBUG
    switch (stage) {
        case S_FETCH:
            printf("F:[%08lX  %08X]\n", guest.proc->PC.bits->xval, insn->insnbits);
            break;
        case S_DECODE:
            printf(" D:\t\t\t[%s\t%s\t%s\t%s\t%s\t%016lX\t%d]\n", 
                opcode_names[insn->op], 
                insn->cond ? cond_names[insn->cond] : "--",
                insn->dst ? insn->dst->name : "---",
                insn->src1 ? insn->src1->name : "---",
                insn->src2 ? insn->src2->name : "---",
                insn->imm, 
                insn->shift);
            break;

        case S_EXECUTE: 
            printf("  X:\n");
            break;
        case S_MEMORY: 
            printf("   M:\n");
            break;
        case S_WBACK: 
            printf("    W:\n");
            break;
        case S_UPDATE_PC: 
            printf("     U:\n");
            break;
        default: assert(false); break;
    }
#endif
    return;
}
