def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}


def_EHelper(addi) {
	sword_t tmp = ((id_src2->imm << 30) >> 31) & 0xFFFFF000;
	const	sword_t sign_extension = id_src2->imm | tmp;
	rtl_addi(s, ddest, dsrc1, sign_extension);
}
