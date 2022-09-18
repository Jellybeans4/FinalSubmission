#include <assert.h>
#include <stdlib.h>
#include "err_handler.h"
#include "STUR.h"
#include "machine.h"

extern machine_t guest;
//C6.2.346, 1922 STUR Xt, Xn, stores val n in t
void decode_STUR(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t t = GETBF(instr, 0, 5); // WHAT DOES T DO 
    uint8_t n = GETBF(instr, 5, 5);
    int imm9 = GETBF(instr, 12, 9);
    int64_t offset= imm9; //check sign extend imm9 to 64
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->src2 = (t == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + t);
    insn->opnd1.xval = (insn->src1->bits->xval) + offset;
    insn->opnd2.xval = (insn->src2->bits->xval);
    // if (31 == n) {
    //     if (0 != insn->opnd1.xval) {
    //         logging(LOG_FATAL, "Stack pointer misaligned");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    return;
}
void execute_STUR(instr_t * const insn) {
    //val_ex is address, mem_xval is data
    insn->val_ex.xval = insn->opnd1.xval;
    insn->val_mem.xval = insn->opnd2.xval;
    return;
}