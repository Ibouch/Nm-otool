/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lc_segment.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:14:20 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 15:14:22 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void		pld_sg_struct_init(struct s_init *init)
{
	if (init->is_32bits)
	{
		init->pld.lcmd.sg = (struct segment_command *)init->pld.lc;
		init->pld.lcmd.sc = (struct section *)((size_t)init->pld.lcmd.sg
		+ sizeof(struct segment_command));
		init->pld.nsects = init->pld.lcmd.sg->nsects;
		init->pld.fileoff = init->pld.lcmd.sg->fileoff;
		init->pld.filesize = init->pld.lcmd.sg->filesize;
	}
	else
	{
		init->pld.lcmd64.sg = (struct segment_command_64 *)init->pld.lc;
		init->pld.lcmd64.sc = (struct section_64 *)((size_t)init->pld.lcmd64.sg
		+ sizeof(struct segment_command_64));
		init->pld.nsects = init->pld.lcmd64.sg->nsects;
		init->pld.fileoff = init->pld.lcmd64.sg->fileoff;
		init->pld.filesize = init->pld.lcmd64.sg->filesize;
	}
}

static bool		check_offset_out_of_range(void *ptr, struct s_init *init)
{
	return (((size_t)init->pld.fileoff
	+ (size_t)init->pld.filesize > (size_t)init->f_size)
	|| (init->is_32bits && (size_t)init->pld.lcmd.sc
	> (size_t)ptr + init->f_size)
	|| (!init->is_32bits && (size_t)init->pld.lcmd64.sc
	> (size_t)ptr + init->f_size));
}

static bool		check_otool_section(void *ptr, struct s_init *init)
{
	if ((ft_strcmp(init->pld.sect.sectname, SECT_TEXT)) == 0)
	{
		init->is_sectdata = false;
		if ((ft_otool(ptr, init)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if ((ft_strcmp(init->pld.sect.sectname, SECT_DATA)) == 0
	&& init->opt_flg.flags & DSECT_FLAGS)
	{
		init->is_sectdata = true;
		if ((ft_otool(ptr, init)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static void		add_section_data(struct s_init *init)
{
	init->pld.sect.segname = ((init->is_32bits)
	? init->pld.lcmd.sc->segname : init->pld.lcmd64.sc->segname);
	init->pld.sect.sectname = ((init->is_32bits)
	? init->pld.lcmd.sc->sectname : init->pld.lcmd64.sc->sectname);
}

bool			parse_lc_segment(void *ptr, struct s_init *init)
{
	uint32_t	i;

	i = 0;
	pld_sg_struct_init(init);
	while (i < init->pld.nsects)
	{
		if (check_offset_out_of_range(ptr, init))
			return (EXIT_FAILURE);
		add_section_data(init);
		if (init->is_otool && (check_otool_section(ptr, init)) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		else
			ft_lstadd_back(&(init->pld.sect_lst), &(init->pld.sect),
			sizeof(init->pld.sect));
		if (init->is_32bits)
			init->pld.lcmd.sc = (struct section *)((size_t)init->pld.lcmd.sc
			+ sizeof(struct section));
		else
			init->pld.lcmd64.sc = (struct section_64 *)
			((size_t)init->pld.lcmd64.sc + sizeof(struct section_64));
		++i;
	}
	return (EXIT_SUCCESS);
}
