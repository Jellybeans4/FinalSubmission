#include <assert.h>
#include "LSL.h"
#include "machine.h"

extern machine_t guest;
//C6.2.213
void decode_LSL(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t d = GETBF(instr, 0, 5);
    uint8_t n = GETBF(instr, 5, 5);
    //immr= (-<shift> MOD 64), imms= #(63-<shift>), imms+1= immr
    uint16_t immr = GETBF(instr, 16, 6);
    uint16_t imms = GETBF(instr, 10, 6);

    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 +d);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->opnd1.xval = (insn->src1->bits->xval);
    insn->shift=63-imms;
    return;
}
void execute_LSL(instr_t * const insn) {
    insn->val_ex.xval = insn->opnd1.xval << insn->shift;
    return;
}