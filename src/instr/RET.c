#include <assert.h>
#include "RET.h"
#include "machine.h"
extern machine_t guest;
//C6.2.254, 1730, ret implies x30
void decode_RET(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t n = GETBF(instr, 5, 5);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->opnd1.xval = insn->src1->bits->xval;
    insn->branch_PC = (int64_t) n;
    return;
}

void execute_RET(instr_t * const insn) {
    return;
}