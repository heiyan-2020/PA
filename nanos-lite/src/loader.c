#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
static Elf_Ehdr elf_header[1];
static Elf_Phdr prog_header[1];
size_t ramdisk_read(void*, size_t, size_t);
static uintptr_t loader(PCB *pcb, const char *filename) {
	//read the elf_header.
	ramdisk_read(elf_header, 0, sizeof(Elf_Ehdr));
	//verify file type.
	assert(*(uint32_t *)elf_header->e_ident == 0x464C457F);
	//read the program_header.
	size_t offset = elf_header->e_phoff;
	size_t len = elf_header->e_phentsize;
	for(uint32_t i = 0; i < elf_header->e_phnum; i++) {
		ramdisk_read(prog_header, offset, len);
		offset += len;
		printf("--------------------\np_type is 0x%x, p_offset is 0x%x, p_vaddr is 0x%x, p_filesz is 0x%x, p_memsz is 0x%x\n", prog_header->p_type, prog_header->p_offset, prog_header->p_vaddr, prog_header->p_filesz, prog_header->p_memsz);

		//seg needs to be loaded.
		if (prog_header->p_type == 0x1) {
			uint8_t* vaddr = (uint8_t*)prog_header->p_vaddr;
			ramdisk_read(vaddr, prog_header->p_offset, prog_header->p_filesz);
			memset(vaddr + prog_header->p_filesz, 0, prog_header->p_memsz - prog_header->p_filesz);			
		}
	}
  return (uintptr_t)0x80000094;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

