#include <assert.h>
#include "LSR.h"
#include "machine.h"

extern machine_t guest;
//C6.2.213
void decode_LSR(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t d = GETBF(instr, 0, 5);
    uint8_t n = GETBF(instr, 5, 5);
    //immr= shift
    uint8_t immr = GETBF(instr, 16, 6);
    uint8_t imms = GETBF(instr, 10, 6);
    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 +d);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->opnd1.xval = (insn->src1->bits->xval);
    insn->shift = immr;
    return;
}
void execute_LSR(instr_t * const insn) {
    insn->val_ex.xval = (int64_t) ((uint64_t) insn->opnd1.xval >> insn->shift);
    return;
}