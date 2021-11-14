
def_EHelper(ecall) {
	rtl_li(s, &(s->dnpc), isa_raise_intr(1, 0x80000540));
}

def_EHelper(csrrw) {

}
