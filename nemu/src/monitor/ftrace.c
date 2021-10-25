#include <isa.h>
#include <stdio.h>
extern char* elf_file; 
Elf32_Ehdr elf_header;
Elf32_Shdr string_table;
Elf32_Shdr symbol_table;
void init_ftrace() {
	FILE* ftrace_fp = fopen(elf_file, "r");
	int item_count = 0;
	//read in elf header.
	item_count = fread(&elf_header, 52, 1, ftrace_fp);
	assert(item_count == 1);
	//read string table.
	fseek(ftrace_fp, elf_header.e_shoff, SEEK_SET);
	fseek(ftrace_fp, elf_header.e_shstrndx * elf_header.e_shentsize, SEEK_CUR);
	item_count = fread(&string_table, 40, 1, ftrace_fp);
	assert(item_count == 1);
	printf("%u\n", elf_header.e_shentsize);
	printf("%u\n", string_table.sh_name);
	fclose(ftrace_fp);
}
