#include <assert.h>
#include <stdlib.h>
#include "err_handler.h"
#include "LDUR.h"
#include "machine.h"

extern machine_t guest;
//C6.2.202, 1637
void decode_LDUR(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t t = GETBF(instr, 0, 5); // WHAT DOES T DO 
    uint8_t n = GETBF(instr, 5, 5);
    int imm9 = GETBF(instr, 12, 9);
    int64_t offset= imm9; //check sign extend imm9 to 64
    insn->op = OP_LDUR;
    
    insn->dst = (t == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + t);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->opnd1.xval = (insn->src1->bits->xval);
    insn->opnd2.xval = offset;
    // if (31 == n) {
    //     if (0 != insn->opnd1.xval) {
    //         logging(LOG_FATAL, "Stack pointer misaligned");
    //         exit(EXIT_FAILURE);
    //     }
    // }
    return;
}
void execute_LDUR(instr_t * const insn) {
    insn->val_ex.xval = insn->opnd1.xval + insn->opnd2.xval;
    return;
}