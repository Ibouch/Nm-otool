
#ifndef							NM_OTOOL_H
# define						NM_OTOOL_H

#include						<fcntl.h>
#include						<unistd.h>
#include						<sys/stat.h>
#include						<sys/mman.h>
#include						<stdio.h>
#include						<mach-o/loader.h>
#include						<mach-o/nlist.h>
#include						<stdbool.h>
#include						<libft.h>
#include						<mach-o/fat.h>
#include						<mach-o/swap.h>
#include						<ar.h>

# define						NO_ARG (init.nb_args == 0)
# define						SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | (((x) & 0xff) << 24))
# define						SYM_NAME (char *)((size_t)init->symd.sym_name + init->symd.n_strx) // NOT USE
# define						N_CPU_TYPE 6
# define						NM_FLAGS 3
# define						OTOOL_FLAGS 1

# define						ARCH_ARG "arch="
# define						ARCH_OFFSET 5
# define						FIND_ARCH 2

# define						A_FLAGS		0x01
# define						G_FLAGS		0x02
# define						UMAJ_FLAGS	0x04
# define						DSECT_FLAGS	0x10

struct							s_cpu_type_names
{
	cpu_type_t					cpu_type;
	const char					*name;
};

struct s_cpu_type_names			g_arch_names[N_CPU_TYPE];

struct							s_section
{
	char						*sectname;	/* name of this section */
	char						*segname;	/* segment this section goes in */
};

typedef unsigned char			t_mask;

struct							s_flags_args
{
	const unsigned char			flg[2];
	t_mask						mask;
};

struct							s_flags_opt
{
	t_mask						flags;
	uint8_t						radix_opt;
	const char					*arch_name;
};

struct							s_ldcmd_data_64
{
	struct mach_header_64		*mh;
	struct segment_command_64	*sg;
	struct section_64			*sc;
};

struct							s_ldcmd_data
{
	struct mach_header			*mh;
	struct segment_command		*sg;
	struct section				*sc;
};

struct							s_parse_ldcmd
{
	uint32_t					ncmds;
	uint32_t					sizeofcmds;
	uint32_t					cmds_offset;
	uint32_t					nsects;
	uint64_t					fileoff;
	uint64_t					filesize;
	char						*segname;
	char						*sectname;
	struct s_section 			sect;
	struct s_list				*sect_lst;
	struct load_command			*lc;
	struct symtab_command		*sym_cmd;
	struct s_ldcmd_data			lcmd;
	struct s_ldcmd_data_64		lcmd64;
};

struct							s_symdata
{
	struct nlist				*array;
	struct nlist_64				*array_64;
	char						*sym_name;
	char						sym_type;
	uint32_t					n_strx;
	uint8_t						n_type;
	uint8_t						n_sect;
	uint64_t					n_value;
	bool						is_ext;
};

struct							s_otool
{
	uint64_t					size;
	uint64_t					addr;
	uint32_t					offset;
};

struct							s_init
{
	int							fd;
	bool						is_32bits;
	bool						is_otool;
	bool						is_sectdata;
	bool						error;
	off_t						f_size;
	int							nb_args;
	void						*file;
	const char					*path;
	const char					*arch_name;
	struct s_list				*args;
	struct fat_arch				*fat_arch32;
	struct s_flags_opt			opt_flg;
	struct s_parse_ldcmd		pld;
	struct s_symdata			symd;
};

const char						*get_cpu(cpu_type_t cputype, const char *name);
bool							unmap_file(struct s_init *init, bool ret);
bool							map_file(struct s_init *init);
bool							check_header_file(void *file, struct s_init *init);
bool							init_flags(int ac, const char **av, struct s_init *init);
void							show_digit_ptr(size_t addr, uint8_t base, uint8_t n_char);
bool							parse_load_commands(void *ptr, struct s_init *init);
void							ft_otool(void *ptr, struct s_init *init);

#endif
