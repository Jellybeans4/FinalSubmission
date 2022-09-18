//writeback special, looks like branch

#include <assert.h>
#include "BL.h"
#include "machine.h"
extern machine_t guest;
//C6.2.34, 1205
void decode_BL(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint32_t imm26 = GETBF(instr, 0, 26);
    int64_t offset= (int64_t) (imm26 << 2); 
    insn->dst = guest.proc->GPR.names64 + 30;
    insn->branch_PC = guest.proc->PC.bits->xval+ offset;
    insn->next_PC =  guest.proc->PC.bits->xval +4; 
    return;
}

void execute_BL(instr_t * const insn) {
    return;
}