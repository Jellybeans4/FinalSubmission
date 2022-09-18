#include <assert.h>
#include "ADDS_RR.h"
#include "machine.h"
extern machine_t guest;
//C6.2.9, 1161
void decode_ADDS_RR(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    assert(EXTRACT(instr, 0xFF000000, 24) == 0xABU);
  
    uint8_t d = EXTRACT(instr, 0x1FU, 0);
    uint8_t n = EXTRACT(instr, 0x3E0U, 5);
    uint8_t m = EXTRACT(instr, 0x3F0000, 16);
    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + d);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->src2 = (m == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + m);
    insn->opnd1.xval = insn->src1->bits->xval;
    insn->opnd2.xval = insn->src2->bits->xval;
    return;
}

void execute_ADDS_RR(instr_t * const insn) {
    insn->val_ex.xval = insn->opnd1.xval + insn->opnd2.xval;
    guest.proc->NZCV.bits->ccval=PACK_CC(
        insn->val_ex.xval<0, //N egative flag
        insn->val_ex.xval==0, //Z ero flag
        insn->val_ex.xval<insn->opnd1.xval || insn->val_ex.xval<insn->opnd2.xval, //C arry flag
        (insn->opnd1.xval > 0 && insn->opnd2.xval > 0 && insn->val_ex.xval<0) || //oV erflow flag
        (insn->opnd1.xval < 0 && insn->opnd2.xval < 0 && insn->val_ex.xval>0) 
        );
    return;
}