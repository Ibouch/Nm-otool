/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_load_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 12:35:07 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 12:35:10 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static bool			size_range_not_valid(void *ptr, struct s_init *init)
{
	return (((size_t)init->pld.lc > (size_t)ptr + init->f_size)
		|| (((init->pld.lc->cmdsize % 8) != 0 && !(init->is_32bits))
		|| ((init->pld.lc->cmdsize % 4) != 0 && init->is_32bits)));
}

static bool			arch_mh_isvalid(const char *arch_name, struct s_init *init)
{
	if (arch_name && init->arch_name
	&& !((ft_strcmp(arch_name, init->arch_name)) == 0))
	{
		ft_putstr("Nm-Otool: ");
		ft_putstr(init->path);
		ft_putstr(": No architecture specified.\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static bool			pld_struct_init(void *ptr, struct s_init *init)
{
	const char		*arch_name;

	ft_bzero(&(init->pld), sizeof(struct s_parse_ldcmd));
	if (init->is_32bits)
	{
		init->pld.lcmd.mh = ((struct mach_header *)ptr);
		init->pld.ncmds = init->pld.lcmd.mh->ncmds;
		init->pld.lc = (struct load_command *)((size_t)ptr
		+ sizeof(struct mach_header));
		arch_name = get_cpu(init->pld.lcmd.mh->cputype, (const char *)NULL);
	}
	else
	{
		init->pld.lcmd64.mh = ((struct mach_header_64 *)ptr);
		init->pld.ncmds = init->pld.lcmd64.mh->ncmds;
		init->pld.lc = (struct load_command *)((size_t)ptr
		+ sizeof(struct mach_header_64));
		arch_name = get_cpu(init->pld.lcmd64.mh->cputype, (const char *)NULL);
	}
	return (arch_mh_isvalid(arch_name, init));
}

static bool			parse_symtab(void *ptr, struct s_init *init)
{
	init->pld.sym_cmd = (struct symtab_command *)(init->pld.lc);
	init->symd.sym_name = (char *)((size_t)ptr + init->pld.sym_cmd->stroff);
	if (init->is_32bits)
	{
		init->symd.array = (struct nlist *)((size_t)ptr
		+ init->pld.sym_cmd->symoff);
		if ((size_t)init->symd.array + init->pld.sym_cmd->nsyms
		> (size_t)ptr + init->f_size)
			return (EXIT_FAILURE);
	}
	else
	{
		init->symd.array_64 = (struct nlist_64 *)((size_t)ptr
		+ init->pld.sym_cmd->symoff);
		if ((size_t)init->symd.array_64 + init->pld.sym_cmd->nsyms
		> (size_t)ptr + init->f_size)
			return (EXIT_FAILURE);
	}
	if ((show_symtab_data(init, init->pld.sect_lst, ptr)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

bool				parse_load_commands(void *ptr, struct s_init *init)
{
	uint32_t		i;

	if ((pld_struct_init(ptr, init)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = 0;
	while (i < init->pld.ncmds)
	{
		if (size_range_not_valid(ptr, init))
			return (EXIT_FAILURE);
		if (init->pld.lc->cmd == LC_SEGMENT_64
		|| (init->pld.lc->cmd == LC_SEGMENT && init->is_32bits))
			if ((parse_lc_segment(ptr, init)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		if (init->pld.lc->cmd == LC_SYMTAB && init->is_otool == false)
			if ((parse_symtab(ptr, init)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		init->pld.lc = (struct load_command *)((unsigned long)init->pld.lc
		+ init->pld.lc->cmdsize);
		++i;
	}
	return (EXIT_SUCCESS);
}
