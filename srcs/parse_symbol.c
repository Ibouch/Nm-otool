/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:13:42 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 13:13:46 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void	get_symbol_section(struct s_list *sect_lst, uint8_t id, char *c)
{
	while (sect_lst && id > 0 && --id)
		sect_lst = sect_lst->next;
	if (!sect_lst)
	{
		*c = 'U';
		return ;
	}
	if ((ft_strcmp(((struct s_section *)(sect_lst->content))->sectname,
	SECT_TEXT)) == 0)
		*c = 'T';
	else if ((ft_strcmp(((struct s_section *)(sect_lst->content))->sectname,
	SECT_DATA)) == 0)
		*c = 'D';
	else if ((ft_strcmp(((struct s_section *)(sect_lst->content))->sectname,
	SECT_BSS)) == 0)
		*c = 'B';
	else
		*c = 'S';
}

void		get_symbol_type(struct s_init *init, t_list *sect_lst)
{
	if ((init->symd.n_type & N_EXT))
		init->symd.is_ext = true;
	if ((init->symd.n_type & N_TYPE) == N_UNDF && init->symd.is_ext
	&& init->symd.n_value)
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
}
