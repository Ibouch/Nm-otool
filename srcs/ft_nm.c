
#include <nm_otool.h>

struct s_cpu_type_names	g_arch_names[N_CPU_TYPE] = 
{
	{CPU_TYPE_I386, "i386"},
	{CPU_TYPE_X86_64, "x86_64"},
	{CPU_TYPE_ARM, "arm"},
	{CPU_TYPE_ARM64, "arm64"},
	{CPU_TYPE_POWERPC, "ppc"},
	{CPU_TYPE_POWERPC64, "ppc64"}
};

void		get_symbol_section(struct s_list *sect_lst, uint8_t id, char *c)
{
	while (sect_lst && id > 0 && --id)
		sect_lst = sect_lst->next;
	if (!sect_lst)
	{
		*c = 'U';
		return ;
	}
	if ((ft_strcmp(((struct s_section *)(sect_lst->content))->sectname, SECT_TEXT)) == 0)
		*c = 'T';
	else if ((ft_strcmp(((struct s_section *)(sect_lst->content))->sectname, SECT_DATA)) == 0)
		*c = 'D';
	else if ((ft_strcmp(((struct s_section *)(sect_lst->content))->sectname, SECT_BSS)) == 0)
		*c = 'B';
	else
		*c = 'S';
}

void		print_output_64(t_list *sect_lst, struct symtab_command *sym, struct s_init *init, void *ptr)
{
	uint32_t		i;
	char			*string;
	struct nlist_64	*array;
	bool			is_ext;
	char			c;

	array = (struct nlist_64 *)((size_t)ptr + sym->symoff);
	string = (char *)((size_t)ptr + sym->stroff);
	if (init->opt_flg.arch_name)
		printf("\n%s (for architecture %s):\n", init->path, init->opt_flg.arch_name);
	for (i = 0; i < sym->nsyms; ++i)
	{
		is_ext = false;
		if ((array[i].n_type & N_EXT))
			is_ext = true;
		if ((array[i].n_type & N_STAB))
		{
			continue;
			//c = '-';
		}
		else if ((array[i].n_type & N_TYPE) == N_UNDF && is_ext && array[i].n_value)
			c = 'C';
		else if ((array[i].n_type & N_TYPE) == N_UNDF)
			c = 'U';
		else if ((array[i].n_type & N_TYPE) == N_ABS)
			c = 'A';
		else if ((array[i].n_type & N_TYPE) == N_SECT && array[i].n_sect)
			get_symbol_section(sect_lst, array[i].n_sect, &c);
		else if ((array[i].n_type & N_TYPE) == N_PBUD)
			c = 'u';
		else if ((array[i].n_type & N_TYPE) == N_INDR)
			c = 'I';
		if (array[i].n_value)
			printf("%016llx ", array[i].n_value);
		else
			ft_putstr("                 ");
		c = ((!is_ext) ? (char)ft_tolower((int)c) : c);
		printf("%c %s\n", c, string + array[i].n_un.n_strx);
	}
}

static void	print_digit_addr(size_t p)
{
	const char	*s;

	s = "0123456789abcdef";
	if (p > 0)
	{
		print_digit_addr(p / 16);
		ft_putchar(s[(p % 16)]);
	}
}

void	show_byte_base(unsigned char c, uint8_t base)
{
	const char	*s;

	s = "0123456789abcdef";
	if (c > 0)
	{
		show_byte_base(c / base, base);
		ft_putchar(s[(c % base)]);
	}
}

void		otool_64(struct section_64 *sc, void *ptr)
{
	uint64_t	i;
	uint64_t	size;
	uint64_t	addr;

	i = 0;
	size = sc->size;
	addr = sc->addr;
	ft_putstr("./ft_nm:\nContents of (__TEXT,__text) section");
	//printf("%llx\t%llu\n", sc->addr, sc->size);
	while (i < size)
	{
		if ((i % 16) == 0)
		{
			ft_putchar('\n');
			print_digit_addr((size_t)addr + i);
			ft_putchar('\t');
		}
		if (__TEXT_BYTE == 0)
			ft_putstr("00");
		else if (__TEXT_BYTE < 16)
			ft_putchar('0');
		if (__TEXT_BYTE)
			show_byte_base(__TEXT_BYTE, 16);
		ft_putchar(' ');
		++i;
	}
}

bool					parse_lc_segment(struct s_init *init, struct s_section *sect, struct s_list **sect_lst)
{
	uint32_t i;

	i = 0;
	init->pld.sg = (struct segment_command_64 *)init->pld.lc;
	init->pld.sc = (struct section_64 *)((size_t)init->pld.sg + sizeof(struct segment_command_64));
	if ((size_t)init->pld.sg->fileoff + (size_t)init->pld.sg->filesize > (size_t)init->f_size)
		return (EXIT_FAILURE);
	while (i < init->pld.sg->nsects)
	{
		sect->segname = ft_strdup(init->pld.sc->segname);
		sect->sectname = ft_strdup(init->pld.sc->sectname);
		ft_lstadd_back(sect_lst, (void *)sect, sizeof(*sect));
		//if ((ft_strcmp(sc->sectname, SECT_TEXT)) == 0 || (ft_strcmp(sc->sectname, SECT_DATA)) == 0)
			//otool_64(sc, ptr);
		init->pld.sc = (struct section_64 *)((size_t)init->pld.sc + sizeof(struct section_64));
		++i;
	}
	return (EXIT_SUCCESS);
}

bool					parse_load_commands(void *ptr, struct s_init *init)
{
	uint32_t					i;
	struct s_section			sect;
	struct s_list				*sect_lst;

	ft_bzero(&(init->pld), sizeof(struct s_parse_ldcmd));
	init->pld.mh = ((struct mach_header_64 *)ptr);
	init->pld.lc = (struct load_command *)((size_t)ptr + sizeof(struct mach_header_64));
	i = 0;
	sect_lst = NULL;
	while (i < init->pld.mh->ncmds)
	{
		if (init->pld.mh->ncmds > init->pld.mh->sizeofcmds || init->pld.sizeofcmds > init->pld.mh->sizeofcmds || (init->pld.lc->cmdsize % 8) != 0)
			return (EXIT_FAILURE);
		if (init->pld.lc->cmd == LC_SEGMENT_64)
			if ((parse_lc_segment(init, &sect, &sect_lst)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		if (init->pld.lc->cmd == LC_SYMTAB)
			print_output_64(sect_lst, (struct symtab_command *)init->pld.lc, init, ptr);
		init->pld.sizeofcmds += init->pld.lc->cmdsize;
		init->pld.lc = (struct load_command *)((unsigned long)init->pld.lc + init->pld.lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}

int				main(int argc, const char **argv)
{
	int				i;
	bool			err;
	struct s_init	init;

	i = 0;
	err = false;
	ft_bzero(&init, sizeof(init));
	if (NO_ARG)
		init.path = "a.out";
	if ((init_flags(argc, argv, &(init.opt_flg))) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	while (++i < argc || NO_ARG)
	{
		if (!(NO_ARG))
			init.path = argv[i];
		if (init.path[0] == '-')
			continue ;
		if ((map_file(&(init))) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if ((unmap_file(&init, &err, check_header_file(init.file, &init))) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (NO_ARG)
			break ;
	}
	return ((err) ? EXIT_FAILURE : EXIT_SUCCESS);
}
