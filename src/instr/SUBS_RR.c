#include <assert.h>
#include "SUBS_RR.h"
#include "machine.h"

extern machine_t guest;
//C6.2.364
void decode_SUBS_RR(instr_t * const insn) {
    int32_t instr = insn->insnbits;

    uint8_t d = EXTRACT(instr, 0x1FU, 0);
    uint8_t n = EXTRACT(instr, 0x3E0U, 5);
    uint8_t imm6 = GETBF(instr, 10,6);
    uint8_t sh = GETBF(instr, 22,2);
    uint8_t m = GETBF(instr, 16,5);
    // bool is_aliased = (sh == 0 && imm12 == 0 && (d == 31 || n == 31));

    // insn->op = is_aliased ? OP_MOV : OP_ADD;
    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + d);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->src2 = (m == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + m);
    insn->opnd1.xval = insn->src1->bits->xval;
    insn->opnd2.xval = insn->src2->bits->xval;
    insn->opnd2.xval = ~(insn->opnd2.xval)+1;
    return;
}

void execute_SUBS_RR(instr_t * const insn) {
    insn->val_ex.xval = insn->opnd1.xval + insn->opnd2.xval;
    return;
}
