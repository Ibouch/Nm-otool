/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 11:51:13 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 11:51:18 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void			otool_init(struct s_init *init, struct s_otool *ot)
{
	ot->size = (init->is_32bits)
	? init->pld.lcmd.sc->size : init->pld.lcmd64.sc->size;
	ot->addr = (init->is_32bits)
	? init->pld.lcmd.sc->addr : init->pld.lcmd64.sc->addr;
	ot->offset = (init->is_32bits)
	? init->pld.lcmd.sc->offset : init->pld.lcmd64.sc->offset;
	if (init->is_sectdata)
		ft_putstr("Contents of (__DATA,__data) section");
	else
	{
		ft_putstr(init->path);
		if (init->arch_name)
		{
			ft_putstr(" (for architecture ");
			ft_putstr(init->arch_name);
			ft_putchar(')');
		}
		ft_putstr(":\nContents of (__TEXT,__text) section");
	}
}

bool				ft_otool(void *ptr, struct s_init *init)
{
	uint64_t		i;
	struct s_otool	ot;

	otool_init(init, &ot);
	i = 0;
	while (i < ot.size)
	{
		if ((off_t)(ot.offset + i) > init->f_size)
			return (EXIT_FAILURE);
		if ((i % init->opt_flg.radix_opt) == 0)
		{
			ft_putchar('\n');
			show_digit_addr((size_t)ot.addr + i, init->opt_flg.radix_opt,
			(init->is_32bits) ? 8 : 16);
			ft_putchar('\t');
		}
		show_digit_addr((*(unsigned char *)((size_t)ptr + (ot.offset + i))),
		init->opt_flg.radix_opt, (init->opt_flg.radix_opt == 16) ? 2 : 3);
		++i;
		(i == ot.size) ? ft_putchar('\n') : ft_putchar(' ');
	}
	return (EXIT_SUCCESS);
}
