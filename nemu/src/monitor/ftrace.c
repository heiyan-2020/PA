#include <isa.h>
#include <stdio.h>
#include <string.h>
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
	char* str_pool = (char*) malloc(string_table.sh_size);
	fseek(ftrace_fp, string_table.sh_offset, SEEK_SET);
	item_count = fread(str_pool, 1, string_table.sh_size, ftrace_fp);
	assert(item_count == string_table.sh_size);
	//find Symbol table
	fseek(ftrace_fp, elf_header.e_shoff, SEEK_SET);
	do {
			item_count = fread(&symbol_table, 40, 1, ftrace_fp);
			assert(item_count == 1);
			char* name = str_pool + symbol_table.sh_name;
			if (strcmp(name, ".stmtab") == 0) {
				break;
			}	
	} while (1);
	printf("DEBUG INFO:Find Successfully!\n");
	fclose(ftrace_fp);
}
