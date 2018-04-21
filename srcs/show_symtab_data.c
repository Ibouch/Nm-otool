/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_symtab_data.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:01:19 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 13:01:25 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void		check_arch_definition(struct s_init *init)
{
	if (init->arch_name)
	{
		ft_putchar('\n');
		ft_putstr(init->path);
		ft_putstr(" (for architecture ");
		ft_putstr(init->arch_name);
		ft_putstr("):\n");
	}
}

static void		symd_struct_init(struct s_symdata *symd, uint32_t i,
				bool is_32bits)
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

static void		print_symbol_data(struct s_init *init)
{
	if (init->opt_flg.flags & A_FLAGS)
	{
		ft_putstr(init->path);
		ft_putstr(": ");
	}
	if (!((init->symd.n_type & N_TYPE) == N_UNDF))
		(init->is_32bits) ? show_digit_addr(init->symd.n_value,
		init->opt_flg.radix_opt, 8) : show_digit_addr(init->symd.n_value,
		init->opt_flg.radix_opt, 16);
	else
		(init->is_32bits) ? ft_putstr("        ")
		: ft_putstr("                ");
	if (!(init->symd.is_ext))
		init->symd.sym_type = (char)ft_tolower((int)init->symd.sym_type);
	ft_putchar(' ');
	ft_putchar(init->symd.sym_type);
	ft_putchar(' ');
	ft_putendl((char *)((size_t)init->symd.sym_name + init->symd.n_strx));
}

bool			show_symtab_data(struct s_init *init, t_list *sect_lst,
				void *ptr)
{
	uint32_t	i;

	check_arch_definition(init);
	i = 0;
	while (i < init->pld.sym_cmd->nsyms)
	{
		symd_struct_init(&(init->symd), i, init->is_32bits);
		if ((size_t)init->symd.sym_name + init->symd.n_strx
		> (size_t)ptr + init->f_size)
			return (EXIT_FAILURE);
		if ((!(init->symd.n_type & N_EXT) && init->opt_flg.flags & G_FLAGS)
		|| ((init->symd.n_type & N_TYPE) == N_UNDF
		&& init->opt_flg.flags & UMAJ_FLAGS)
		|| (init->symd.n_type & N_STAB))
		{
			++i;
			continue;
		}
		get_symbol_type(init, sect_lst);
		print_symbol_data(init);
		++i;
	}
	return (EXIT_SUCCESS);
}
