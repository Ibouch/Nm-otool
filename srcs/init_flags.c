/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 18:52:58 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/18 18:53:06 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

/*
**	-A		Write the pathname or library name of an object on each line.
**	----------------------------------------------------------------------------
**	-g		Display only global (external) symbols.
**	----------------------------------------------------------------------------
**	-U		Don't display undefined symbols.
**	----------------------------------------------------------------------------
**	-arch	Specifies the architecture, arch_type, of the file for nm(1)
**			to operate on when the file is a universal file
**			(see arch(3) for the currently known arch_types).
**			The arch_type can be "all" to operate on all architectures in
**			the file. The default is to display the symbols from only the host
**			architecture, if the file contains it; otherwise, symbols for
**			all architectures in the file are displayed.
**	----------------------------------------------------------------------------
**	-t		The format shall be dependent on the single character used
**			as the format option-argument:
**				d	The value shall be written in decimal (default).
**				o	The value shall be written in octal.
**				x	The value shall be written in hexadecimal.
*/

static int8_t		check_radix_flag(t_parse_flags *f, int ac, const char **av,
					t_init *init)
{
	if (av[f->i][f->j] == 't' && f->i + 1 < ac)
	{
		if ((ft_strcmp(av[f->i + 1], "d\0")) == 0)
			init->opt_flg.radix_opt = 10;
		else if ((ft_strcmp(av[f->i + 1], "o\0")) == 0)
			init->opt_flg.radix_opt = 8;
		else if ((ft_strcmp(av[f->i + 1], "x\0")) == 0)
			init->opt_flg.radix_opt = 16;
		else
		{
			ft_putstr("Nm-Otool: for the -radix option: \
Cannot find option named '");
			ft_putstr(av[f->i + 1]);
			ft_putstr("'!\n");
			return (EXIT_FAILURE);
		}
		++f->i;
		return (FIND_RADIX);
	}
	else if (av[f->i][f->j] == 't')
	{
		ft_putstr("Nm-Otool: for the -t option: requires a value!\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int8_t		parse_flag(t_parse_flags *f, int ac, const char **av,
					t_init *init)
{
	if (av[f->i][f->j + 1] == '-')
		f->j += 2;
	else
		++f->j;
	if ((f->ret = check_radix_flag(f, ac, av, init)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	else if (f->ret == FIND_RADIX)
		return (BREAK_VALUE);
	if ((f->ret = check_flag(init,
	(const char *)av[f->i], &f->j)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	else if (f->ret == FIND_ARCH)
		return (BREAK_VALUE);
	if (av[f->i][f->j])
		++f->j;
	return (EXIT_SUCCESS);
}

bool				init_flags(int ac, const char **av, t_init *init)
{
	t_parse_flags	f;

	ft_bzero(&f, sizeof(t_parse_flags));
	init->opt_flg.radix_opt = 16;
	while (++f.i < ac)
	{
		f.j = 0;
		if (av[f.i][0] != '-')
		{
			ft_lstadd_back(&(init->args), (void *)ft_strdup(av[f.i]),
			ft_strlen(av[f.i]) + 1);
			init->nb_args += 1;
		}
		while (av[f.i][0] == '-' && av[f.i][f.j] != '\0')
		{
			if ((f.ret = parse_flag(&f, ac, av, init)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			else if (f.ret == BREAK_VALUE)
				break ;
		}
	}
	return (EXIT_SUCCESS);
}
