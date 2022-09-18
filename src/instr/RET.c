#include <assert.h>
#include "RET.h"
#include "machine.h"
extern machine_t guest;
//C6.2.254, 1730, ret implies x30
void decode_RET(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t n = GETBF(instr, 5, 5);
    insn->dst = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->branch_PC = (int64_t)(insn->dst->bits->xval);
    return;
}

void execute_RET(instr_t * const insn) {
    return;
}