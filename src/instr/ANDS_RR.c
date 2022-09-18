#include <assert.h>
#include "ANDS_RR.h"
#include "machine.h"

extern machine_t guest;
//C6.2.15
void decode_ANDS_RR(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    assert(EXTRACT(instr, 0xFF000000, 24) == 0xEAU);
    
    uint8_t d = GETBF(instr, 0, 5);
    uint8_t n = GETBF(instr, 5, 5);
    uint8_t m = GETBF(instr, 16, 5);
    
    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + d);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->src2 = (m == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + m);
    insn->opnd1.xval = insn->src1->bits->xval;
    insn->opnd2.xval = insn->src2->bits->xval;
    return;
}

void execute_ANDS_RR(instr_t * const insn) {
    insn->val_ex.xval = insn->opnd1.xval & insn->opnd2.xval;
    //set negative and carry bits
    guest.proc->NZCV.bits->ccval=PACK_CC(insn->val_ex.xval<0,insn->val_ex.xval==0,0,0);
    return;
}