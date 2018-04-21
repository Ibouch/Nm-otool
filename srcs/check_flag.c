/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_flag.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 12:06:31 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 12:06:33 by ibouchla         ###   ########.fr       */
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

static struct s_flags_args	nm_flags_args[NM_FLAGS] =
{
	{"-A", A_FLAGS},
	{"-g", G_FLAGS},
	{"-U", UMAJ_FLAGS}
};

static struct s_flags_args	otool_flags_args[OTOOL_FLAGS] =
{
	{"-d", DSECT_FLAGS}
};

static t_mask	get_flag_mask(const char c, bool is_otool)
{
	int8_t		i;

	i = (-1);
	if (is_otool)
	{
		while (++i < OTOOL_FLAGS)
			if (c == otool_flags_args[i].flg[1])
				return (otool_flags_args[i].mask);
	}
	else
	{
		while (++i < NM_FLAGS)
			if (c == nm_flags_args[i].flg[1])
				return (nm_flags_args[i].mask);
	}
	return ((t_mask)0x00);
}

static int8_t	find_arch_arg(t_init *init, const char *arg,
				unsigned int *j)
{
	if ((ft_strncmp((const char *)((size_t)arg + *j),
	ARCH_ARG, ARCH_OFFSET)) == 0)
	{
		init->opt_flg.arch_name = (const char *)((size_t)arg
		+ (*j + ARCH_OFFSET));
		if (!((ft_strcmp(init->opt_flg.arch_name, "all\0")) == 0)
		&& ((get_cpu(0, init->opt_flg.arch_name)) == NULL))
		{
			ft_putstr("Nm-Otool: for the -arch option: Unknown \
architecture named '");
			ft_putstr(init->opt_flg.arch_name);
			ft_putstr("'.\n");
			return (EXIT_FAILURE);
		}
		return (FIND_ARCH);
	}
	return (EXIT_SUCCESS);
}

int8_t			check_flag(t_init *init, const char *arg,
				unsigned int *j)
{
	t_mask		mask;
	int8_t		ret;

	if ((ret = find_arch_arg(init, arg, j)) != EXIT_SUCCESS)
		return (ret);
	while (arg[*j] != '\0' && !(ft_isspace(arg[*j])))
	{
		if ((mask = get_flag_mask(arg[*j], init->is_otool)) == 0x00)
		{
			ft_putstr("Nm-Otool: Unknown command line argument '");
			ft_putstr(arg);
			ft_putstr("'.\n");
			return (EXIT_FAILURE);
		}
		init->opt_flg.flags |= mask;
		*j += 1;
	}
	return (EXIT_SUCCESS);
}
