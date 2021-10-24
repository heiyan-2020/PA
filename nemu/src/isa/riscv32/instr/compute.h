def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}


def_EHelper(addi) {
	rtl_addi(s, ddest, dsrc1, id_src2->simm);
}


def_EHelper(auipc) {
	rtl_li(s, ddest, id_src1->imm + s->pc);
}

def_EHelper(jal) {
	rtl_li(s, ddest, s->pc + 4);
	rtl_li(s, &(s->dnpc), id_src1->simm + s->pc);
}

def_EHelper(jalr) {
	rtl_addi(s, &(s->dnpc), dsrc1, id_src2->simm);
	s->dnpc &= 0xFFFFFFFE;
	rtl_li(s, ddest, s->pc + 4);
}

def_EHelper(add) {
	rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
	rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sltiu) {
	rtl_setrelopi(s, RELOP_LTU, ddest, dsrc1, id_src2->simm);
}

def_EHelper(beq) {
	rtl_setrelop(s, RELOP_EQ, s0, dsrc1, dsrc2);
	if (*s0) {
		rtl_li(s, &(s->dnpc), id_dest->simm + s->pc);
	}	
}

def_EHelper(bne) {
	rtl_setrelop(s, RELOP_NE, s0, dsrc1, dsrc2);
	if (*s0) {
		rtl_li(s, &(s->dnpc), id_dest->simm + s->pc);
	}	
}

def_EHelper(bge) {
	rtl_setrelop(s, RELOP_GE, s0, dsrc1, dsrc2);
	if (*s0) {
		rtl_li(s, &(s->dnpc), id_dest->simm + s->pc);
	}
}

def_EHelper(bltu) {
	rtl_setrelop(s, RELOP_LTU, s0, dsrc1, dsrc2);
	if (*s0) {
		rtl_li(s, &(s->dnpc), id_dest->simm + s->pc);
	}
}

def_EHelper(sltu) {
	rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}

def_EHelper(myxor) {
	rtl_xor(s, ddest, dsrc1, dsrc2);
}

def_EHelper(myor) {
	rtl_or(s,ddest, dsrc1, dsrc2);
}

def_EHelper(srai) {
	rtl_srai(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(andi) {
	rtl_andi(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(sll) {
	rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(myand) {
	rtl_and(s, ddest, dsrc1, dsrc2);
}

def_EHelper(xori) {
	rtl_xori(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(div) {
	rtl_divs_q(s, ddest, dsrc1, dsrc2);
}

def_EHelper(rem) {
	rtl_divs_r(s, ddest, dsrc1, dsrc2);
}

def_EHelper(mul) {
	rtl_mulu_lo(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slli) {
	rtl_slli(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(srli) {
	rtl_srli(s, ddest, dsrc1, id_src2->simm);
}
