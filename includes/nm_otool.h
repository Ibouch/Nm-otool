
#ifndef					NM_OTOOL_H
# define				NM_OTOOL_H

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <stdbool.h>
#include <libft.h>
#include <mach-o/fat.h>
#include <mach-o/swap.h>
#include <ar.h>

# define				NO_ARG (argc == 1)
# define				SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
# define				__TEXT_BYTE (*(unsigned char *)((size_t)ptr + (sc->offset + i)))
# define				N_CPU_TYPE 6
# define				N_FLAGS 8

# define				ARCH_ARG "arch="
# define				ARCH_OFFSET 5
# define				FIND_ARCH 2
# define				A_FLAGS	0x01
# define				B_FLAGS	0x02
# define				C_FLAGS	0x04
# define				D_FLAGS	0x08
# define				E_FLAGS	0x10
# define				F_FLAGS	0x20
# define				G_FLAGS	0x40
# define				H_FLAGS	0x80

struct					s_cpu_type_names
{
	cpu_type_t			cpu_type;
	const char			*name;
};

struct s_cpu_type_names	g_arch_names[N_CPU_TYPE];

struct					s_section
{
	char				*sectname;	/* name of this section */
	char				*segname;	/* segment this section goes in */
};
/*
struct					s_fat_arch32 // A VOIR SI JUTILISE
{
	const char			*arch_name;
	uint32_t			arch_offset;
};

struct					s_fat_arch64 // A VOIR SI JUTILISE
{
	const char			*arch_name;
	uint64_t			arch_offset;
};
*/
typedef unsigned char	t_mask;

struct					s_flags_args
{
	const unsigned char	flg[2];
	t_mask				mask;
};

struct					s_flags_opt
{
	t_mask				flags;
	const char			*arch_name;
	const char			radix_opt;
};

struct							s_parse_ldcmd
{
	struct mach_header_64		*mh;
	struct load_command			*lc;
	struct segment_command_64	*sg;
	struct section_64			*sc;
	uint32_t					sizeofcmds;
};

struct						s_init
{
	int						fd;
	bool					is_32bits;
	off_t					f_size;
	void					*file;
	const char				*path;
	struct fat_arch			*fat_arch32;
	struct s_flags_opt		opt_flg;
	struct s_parse_ldcmd	pld;
};

const char					*get_cpu(cpu_type_t cputype, const char *name);
bool						unmap_file(struct s_init *init, bool *err, bool ret);
bool						map_file(struct s_init *init);
bool						check_header_file(void *file, struct s_init *init);
bool						init_flags(int ac, const char **av, struct s_flags_opt *opt_flg);
void						show_byte_base(unsigned char c, uint8_t base);
bool						parse_load_commands(void *ptr, struct s_init *init);

#endif
