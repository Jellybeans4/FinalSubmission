#include <assert.h>
#include "B.h"
#include "machine.h"
extern machine_t guest;
//C6.2.25, 1190
void decode_B(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint32_t imm26 = GETBF(instr, 0, 26);
    int64_t offset= (int64_t) (imm26 << 2); 
    insn->branch_PC = guest.proc->PC.bits->xval+offset;
    return;
}

void execute_B(instr_t * const insn) {
    return;
}