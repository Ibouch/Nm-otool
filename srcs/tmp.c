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

static void	print_digit_addr(size_t p, uint8_t base)
{
	const char	*s;

	s = "0123456789abcdef";

	if (p > 0)
	{
		print_digit_addr(p / base, base);
		ft_putchar(s[(p % base)]);
	}
}

void		show_digit_ptr(size_t addr, uint8_t base, uint8_t n_char)
{
	size_t	len;
	size_t	addr_len;

	addr_len = ft_udigitlen(addr, base);
	len = (addr_len > n_char) ? 0 : n_char - addr_len;
	len = (addr == 0) ? len + 1 : len;
	while (len--)
		ft_putchar('0');
	print_digit_addr(addr, base);
}

void		ttt(struct s_symdata *symd, uint32_t i, bool is_32bits)
{
	if (is_32bits)
	{
		symd->n_type = symd->array[i].n_type;
		symd->n_sect = symd->array[i].n_sect;
		symd->n_value = symd->array[i].n_value;
		symd->n_strx = symd->array[i].n_un.n_strx;
	}
	else
	{
		symd->n_type = symd->array_64[i].n_type;
		symd->n_sect = symd->array_64[i].n_sect;
		symd->n_value = symd->array_64[i].n_value;
		symd->n_strx = symd->array_64[i].n_un.n_strx;
	}
	symd->is_ext = false;
}

void		show_symtab_data(struct s_init *init, t_list *sect_lst)
{
	uint32_t		i;

	if (init->arch_name)
	{
		ft_putchar('\n');
		ft_putstr(init->path);
		ft_putstr(" (for architecture ");
		ft_putstr(init->arch_name);
		ft_putstr("):\n");
	}
	i = 0;
	while (i < init->pld.sym_cmd->nsyms)
	{
		ttt(&(init->symd), i, init->is_32bits);
		if ((!(init->symd.n_type & N_EXT) && init->opt_flg.flags & G_FLAGS)
		|| ((init->symd.n_type & N_TYPE) == N_UNDF && init->opt_flg.flags & UMAJ_FLAGS)
		|| (init->symd.n_type & N_STAB))
		{
			++i;
			continue;
		}
		if ((init->symd.n_type & N_EXT))
			init->symd.is_ext = true;
		if ((init->symd.n_type & N_TYPE) == N_UNDF && init->symd.is_ext && init->symd.n_value)
			init->symd.sym_type = 'C';
		else if ((init->symd.n_type & N_TYPE) == N_UNDF)
			init->symd.sym_type = 'U';
		else if ((init->symd.n_type & N_TYPE) == N_ABS)
			init->symd.sym_type = 'A';
		else if ((init->symd.n_type & N_TYPE) == N_SECT && init->symd.n_sect)
			get_symbol_section(sect_lst, init->symd.n_sect, &(init->symd.sym_type));
		else if ((init->symd.n_type & N_TYPE) == N_PBUD)
			init->symd.sym_type = 'u';
		else if ((init->symd.n_type & N_TYPE) == N_INDR)
			init->symd.sym_type = 'I';
		if (init->opt_flg.flags & A_FLAGS)
		{
			ft_putstr(init->path);
			ft_putstr(": ");
		}
		if (!((init->symd.n_type & N_TYPE) == N_UNDF))
			(init->is_32bits) ? show_digit_ptr(init->symd.n_value, init->opt_flg.radix_opt, 8) : show_digit_ptr(init->symd.n_value, init->opt_flg.radix_opt, 16);
		else
			(init->is_32bits) ? ft_putstr("        ") : ft_putstr("                ");
		if (!(init->symd.is_ext))
			init->symd.sym_type = (char)ft_tolower((int)init->symd.sym_type);
		ft_putchar(' ');
		ft_putchar(init->symd.sym_type);
		ft_putchar(' ');
		ft_putendl(SYM_NAME);
		++i;
	}
}

bool					parse_lc_segment(void *ptr, struct s_init *init)
{
	uint32_t	i;

	if (init->is_32bits)
	{
		init->pld.lcmd.sg = (struct segment_command *)init->pld.lc;
		init->pld.lcmd.sc = (struct section *)((size_t)init->pld.lcmd.sg + sizeof(struct segment_command));
		init->pld.nsects = init->pld.lcmd.sg->nsects;
		init->pld.fileoff = init->pld.lcmd.sg->fileoff;
		init->pld.filesize = init->pld.lcmd.sg->filesize;
	}
	else
	{
		init->pld.lcmd64.sg = (struct segment_command_64 *)init->pld.lc;
		init->pld.lcmd64.sc = (struct section_64 *)((size_t)init->pld.lcmd64.sg + sizeof(struct segment_command_64));
		init->pld.nsects = init->pld.lcmd64.sg->nsects;
		init->pld.fileoff = init->pld.lcmd64.sg->fileoff;
		init->pld.filesize = init->pld.lcmd64.sg->filesize;
	}
	if ((size_t)init->pld.fileoff + (size_t)init->pld.filesize > (size_t)init->f_size)
		return (EXIT_FAILURE);
	i = 0;
	while (i < init->pld.nsects)
	{
		init->pld.segname = ((init->is_32bits) ? init->pld.lcmd.sc->segname : init->pld.lcmd64.sc->segname);
		init->pld.sectname = ((init->is_32bits) ? init->pld.lcmd.sc->sectname : init->pld.lcmd64.sc->sectname);
		if (init->is_otool)
		{
			if ((ft_strcmp(init->pld.sectname, SECT_TEXT)) == 0)
			{
				init->is_sectdata = false;
				ft_otool(ptr, init);
			}
			else if ((ft_strcmp(init->pld.sectname, SECT_DATA)) == 0 && init->opt_flg.flags & DSECT_FLAGS)
			{
				init->is_sectdata = true;
				ft_otool(ptr, init);
			}
		}
		else
		{
			init->pld.sect.segname = ft_strdup(init->pld.segname);
			init->pld.sect.sectname = ft_strdup(init->pld.sectname);
			ft_lstadd_back(&(init->pld.sect_lst), &(init->pld.sect), sizeof(init->pld.sect));;
		}
		if (init->is_32bits)
			init->pld.lcmd.sc = (struct section *)((size_t)init->pld.lcmd.sc + sizeof(struct section));
		else
			init->pld.lcmd64.sc = (struct section_64 *)((size_t)init->pld.lcmd64.sc + sizeof(struct section_64));
		++i;
	}
	return (EXIT_SUCCESS);
}

bool					parse_load_commands(void *ptr, struct s_init *init)
{
	uint32_t					i;
	const char					*arch_name;

	ft_bzero(&(init->pld), sizeof(struct s_parse_ldcmd));
	if (init->is_32bits)
	{
		init->pld.lcmd.mh = ((struct mach_header *)ptr);
		init->pld.ncmds = init->pld.lcmd.mh->ncmds;
		init->pld.sizeofcmds = init->pld.lcmd.mh->sizeofcmds;
		init->pld.lc = (struct load_command *)((size_t)ptr + sizeof(struct mach_header));
		arch_name = get_cpu(init->pld.lcmd.mh->cputype, (const char *)NULL);
	}
	else
	{
		init->pld.lcmd64.mh = ((struct mach_header_64 *)ptr);
		init->pld.ncmds = init->pld.lcmd64.mh->ncmds;
		init->pld.sizeofcmds = init->pld.lcmd64.mh->sizeofcmds;
		init->pld.lc = (struct load_command *)((size_t)ptr + sizeof(struct mach_header_64));
		arch_name = get_cpu(init->pld.lcmd64.mh->cputype, (const char *)NULL);
	}
	if (arch_name && init->arch_name && !((ft_strcmp(arch_name, init->arch_name)) == 0))
	{
		printf("./ft_nm: %s: No architecture specified.\n", init->path);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < init->pld.ncmds)
	{
		if (init->pld.ncmds > init->pld.sizeofcmds || init->pld.cmds_offset > init->pld.sizeofcmds
		|| (((init->pld.lc->cmdsize % 8) != 0 && !(init->is_32bits))
		|| ((init->pld.lc->cmdsize % 4) != 0 && init->is_32bits)))
			return (EXIT_FAILURE);
		if (init->pld.lc->cmd == LC_SEGMENT_64 || (init->pld.lc->cmd == LC_SEGMENT && init->is_32bits))
			if ((parse_lc_segment(ptr, init)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		if (init->pld.lc->cmd == LC_SYMTAB && init->is_otool == false)
		{
			init->pld.sym_cmd = (struct symtab_command *)(init->pld.lc);
			init->symd.sym_name = (char *)((size_t)ptr + init->pld.sym_cmd->stroff);
			if (init->is_32bits)
				init->symd.array = (struct nlist *)((size_t)ptr + init->pld.sym_cmd->symoff);
			else
				init->symd.array_64 = (struct nlist_64 *)((size_t)ptr + init->pld.sym_cmd->symoff);
			show_symtab_data(init, init->pld.sect_lst);
		}
		init->pld.cmds_offset += init->pld.lc->cmdsize;
		init->pld.lc = (struct load_command *)((unsigned long)init->pld.lc + init->pld.lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}

void	lst_args_del(void *content, size_t content_size)
{
	(void)content_size;
	ft_strdel((char **)(&content));
}

