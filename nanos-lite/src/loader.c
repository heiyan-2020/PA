#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
size_t ramdisk_read(void*, size_t, size_t);
static uintptr_t loader(PCB *pcb, const char *filename) {
	Elf_Ehdr elf_header[1];
	//read the elf_header.
	ramdisk_read(elf_header, 0, sizeof(Elf_Ehdr));
	//verify file type.
	assert(*(uint32_t *)elf_header->e_ident == 0x464C457F);
	//read the program_header.
	Elf_Phdr prog_header[1];
	size_t offset = elf_header->e_phoff;
	size_t len = elf_header->e_phentsize;
	uint8_t* entry = NULL;
	for(uint32_t i = 0; i < elf_header->e_phnum; i++) {
		ramdisk_read(prog_header, offset, len);
		offset += len;
		//seg needs to be loaded.
		if (prog_header->p_type == 0x1) {
			uint8_t* vaddr = (uint8_t*)prog_header->p_vaddr;
			if (entry == NULL) {
				entry = vaddr;
			}
			ramdisk_read(vaddr, prog_header->p_offset, prog_header->p_filesz);
			memset(vaddr + prog_header->p_filesz, 0, prog_header->p_memsz - prog_header->p_filesz);			
			printf("0x%x\n", *vaddr);
		}
	}
  return (uintptr_t)entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

