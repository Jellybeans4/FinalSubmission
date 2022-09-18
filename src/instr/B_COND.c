#include <assert.h>
#include "B_COND.h"
#include "machine.h"
extern machine_t guest;
//C6.2.26, 1191, condition codes at 225
void decode_B_COND(instr_t * const insn) {
    int32_t instr = insn->insnbits;
    uint8_t cond = GETBF(instr, 0, 3);
    uint32_t imm19 = GETBF(instr, 5, 19);
    int64_t offset= (int64_t) (imm19 << 2); 
    insn->branch_PC = guest.proc->PC.bits->xval+offset;
    insn->cond= cond;
    return;
}

void execute_B_COND(instr_t * const insn) {
    if(insn->cond == 0)
    {insn->cond_holds = GET_ZF(guest.proc->NZCV.bits->ccval) == 1;}
    else if (insn->cond == 1)
    {insn->cond_holds = GET_CF(guest.proc->NZCV.bits->ccval) == 1;}
    else if (insn->cond == 2)
    {insn->cond_holds = GET_NF(guest.proc->NZCV.bits->ccval) == 1;}
    else if (insn->cond == 3)
    {insn->cond_holds = GET_VF(guest.proc->NZCV.bits->ccval) == 1;}
    else if (insn->cond == 4)
    {insn->cond_holds = GET_CF(guest.proc->NZCV.bits->ccval) && !GET_ZF(guest.proc->NZCV.bits->ccval);}
    else if (insn->cond== 5)
    {insn->cond_holds = GET_NF(guest.proc->NZCV.bits->ccval) == GET_VF(guest.proc->NZCV.bits->ccval);}
    else if (insn->cond== 6)
    {insn->cond_holds = (GET_NF(guest.proc->NZCV.bits->ccval) == GET_VF(guest.proc->NZCV.bits->ccval)) 
                         && (GET_ZF(guest.proc->NZCV.bits->ccval) == 0);}
    else if (insn->cond == 7)
    {insn->cond_holds = 1;}
    insn->cc.ccval = guest.proc->NZCV.bits->ccval;
    return;
}
/**
     * when '000' result = (PSTATE.Z == '1'); // EQ or NE 
 when '001' result = (PSTATE.C == '1'); // CS or CC 
 when '010' result = (PSTATE.N == '1'); // MI or PL 
 when '011' result = (PSTATE.V == '1'); // VS or VC 
 when '100' result = (PSTATE.C == '1' && PSTATE.Z == '0'); // HI or LS 
 when '101' result = (PSTATE.N == PSTATE.V); // GE or LT 
 when '110' result = (PSTATE.N == PSTATE.V && PSTATE.Z == '0'); // GT or LE 
 when '111' result = TRUE; // AL**/