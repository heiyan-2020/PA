#include <proc.h>
#include <elf.h>
#include <fs.h>

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
uintptr_t loader(PCB *pcb, const char *filename) {
	char** addr = (char**)0x830135f0;
	printf("%s\n", *addr);
	//read the elf_header.
	int fd = fs_open(filename, 0, 0);	
	fs_read(fd, elf_header, sizeof(Elf_Ehdr));
	//verify file type.
	assert(*(uint32_t *)elf_header->e_ident == 0x464C457F);
	//read the program_header.
	size_t offset = elf_header->e_phoff;
	size_t len = elf_header->e_phentsize;
	for(uint32_t i = 0; i < elf_header->e_phnum; i++) {
		fs_lseek(fd, offset, SEEK_SET);
		fs_read(fd, prog_header, len);
		offset += len;
		//seg needs to be loaded.
		if (prog_header->p_type == 0x1) {
			uint8_t* vaddr = (uint8_t*)prog_header->p_vaddr;
			fs_lseek(fd, prog_header->p_offset, SEEK_SET);
			fs_read(fd, vaddr, prog_header->p_filesz);
			memset(vaddr + prog_header->p_filesz, 0, prog_header->p_memsz - prog_header->p_filesz);			
		}
	}
  return (uintptr_t)elf_header->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

