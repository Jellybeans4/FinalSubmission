#include <assert.h>
#include "ASR.h"
#include "machine.h"

extern machine_t guest;
//C6.2.17, 1175
void decode_ASR(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    
    uint8_t d = GETBF(instr, 0, 5);
    uint8_t n = GETBF(instr, 5, 5);
    //uint8_t imms = GETBF(instr, 10, 6); //codes type of shift 
    uint8_t immr = GETBF(instr, 16, 6); //amount we are shifting
    uint8_t randomN = GETBF(instr, 22, 1);
    // 

    // 
    insn->dst = (d == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + d);
    insn->src1 = (n == 31) ? &(guest.proc->SP) : (guest.proc->GPR.names64 + n);
    insn->opnd1.xval = (insn->src1->bits->xval)>>immr;
    return;
}
void execute_ASR(instr_t * const insn) {
    insn->val_ex.xval = insn->opnd1.xval;
    return;
}