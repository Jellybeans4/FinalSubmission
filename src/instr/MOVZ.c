#include <assert.h>
#include "MOVZ.h"
#include "machine.h"
extern machine_t guest;
//C6.2.227, 1681
void decode_MOVZ(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t d = GETBF(instr, 0, 5);
    uint64_t imm16 = GETBF(instr, 5, 16);
    uint8_t hw = GETBF(instr, 21,2);
    hw=hw<<4;
    
    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 +d);
    insn->imm = imm16;
    insn->shift = hw;
    return;
}
void execute_MOVZ(instr_t * const insn) {
    insn->val_ex.xval = insn->imm << insn->shift;
    return;
}