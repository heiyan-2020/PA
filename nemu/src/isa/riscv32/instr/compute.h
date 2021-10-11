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
