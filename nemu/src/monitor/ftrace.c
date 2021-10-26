#include <isa.h>
#include <stdio.h>
#include <string.h>
#define ELF32_ST_BIND(info) ((info) >> 4)
#define ELF32_ST_TYPE(info) ((info) & 0xf)
void print_str_pool();
extern char* elf_file; 
extern char* ftrace_file;
Elf32_Ehdr elf_header;
Elf32_Shdr string_table;
Elf32_Shdr symbol_table;
char* str_pool; 
Elf32_Sym* symbol_pool;
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
	str_pool = (char*) malloc(string_table.sh_size);
	fseek(ftrace_fp, string_table.sh_offset, SEEK_SET);
	item_count = fread(str_pool, 1, string_table.sh_size, ftrace_fp);
	assert(item_count == string_table.sh_size);
	//find Symbol table
	fseek(ftrace_fp, elf_header.e_shoff, SEEK_SET);
	do {
			item_count = fread(&symbol_table, 40, 1, ftrace_fp);
			assert(item_count == 1);
			char* name = str_pool + symbol_table.sh_name;
			if (strcmp(name, ".symtab") == 0) {
				break;
			}	
	} while (1);
	//load each symbol
	symbol_pool = (Elf32_Sym*) malloc(symbol_table.sh_size);
	fseek(ftrace_fp, symbol_table.sh_offset, SEEK_SET);
	item_count = fread(symbol_pool, 1, symbol_table.sh_size, ftrace_fp);
	assert(item_count == symbol_table.sh_size);
	for(int i = 0; i < symbol_table.sh_size / symbol_table.sh_entsize; i++) {
		printf("%u\t\t%s\t\t0x%08x\t\t%d\t\t0x%02x\t\t0x%02x\n",(symbol_pool + i)->st_name ,str_pool + (symbol_pool + i)->st_name, (symbol_pool + i)->st_value, (symbol_pool + i)->st_size, ELF32_ST_BIND((symbol_pool + i)->st_info), ELF32_ST_TYPE((symbol_pool + i)->st_info));
	}
	fclose(ftrace_fp);
}

void print_str_pool() {
	printf("the size of str_pool is %d\n", string_table.sh_size);
	char* ptr = str_pool;
	while (*ptr == '\0') {
		ptr++;
	}
	int len = strlen(ptr);
	while (ptr - str_pool < string_table.sh_size) {
		printf("len = %d, string = %s, offset = %ld\n",len, ptr, ptr - str_pool);
		ptr += len;
		while (*ptr == '\0') {
		ptr++;
	}
		len = strlen(ptr);
	}
}

void print_ftrace(char* log) {
	FILE* file = fopen(ftrace_file, "a");
	fputs(log, file);
	fclose(file);
}

unsigned char get_type(Elf32_Sym* symbol) {
	return ELF32_ST_TYPE(symbol->st_info);
}

bool func_call(uint32_t addr, uint32_t site) {
	Elf32_Sym* itr = symbol_pool;
	int count = 0;
	char log[128] = {'\0'};
	char tmpBuffer[128];
	while (count < symbol_table.sh_size / symbol_table.sh_entsize) {
		if (itr->st_value == addr) {
			//function call.
			sprintf(tmpBuffer, "[0x%x]\tcall", site);
			strcat(log, tmpBuffer);
			sprintf(tmpBuffer, "[@0x%x]\n", addr);
			strcat(log, tmpBuffer);
			print_ftrace(log);
			return true;
		}
		itr++;
		count++;
	}
	return false;
}

bool func_return(uint32_t site) {
	Elf32_Sym* itr = symbol_pool;
	int count = 0;
	char log[128];
	char tmpBuffer[128];
printf("DEBUG INFO: the address of this instr is 0x%x\n", site);
	while (count < symbol_table.sh_size / symbol_table.sh_entsize) {
			if (site > itr->st_value && site <= itr->st_size) {
			//function return
			sprintf(tmpBuffer, "[0x%0x]\treturn", site);
			strcat(log ,tmpBuffer);
			sprintf(tmpBuffer, "[%s]\n", str_pool + (itr->st_name));
			strcat(log, tmpBuffer);
			print_ftrace(log);
			return true;
			}
			itr++;
			count++;
		}
	return false;
	}
