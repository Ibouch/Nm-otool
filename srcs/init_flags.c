
#include <nm_otool.h>

/*
**	-A		Write the pathname or library name of an object on each line.
**	_____________________________________________________________________
**	-g		Display only global (external) symbols.
**	_______________________________________________
**	-U		Don't display undefined symbols.
**	____________________________________________________________________________________________________________________
**	-arch	Specifies the architecture, arch_type, of the file for nm(1) to operate on when the file is a universal file
			(see arch(3) for the currently known arch_types). The arch_type can be "all" to operate on all architectures in the file.
			The default is to display the symbols from only the host architecture, if the file contains it;
			otherwise, symbols for all architectures in the file are displayed.
**	_________________________________________________________________________________________________
**	-t		The format shall be dependent on the single character used as the format option-argument:
			d	The value shall be written in decimal (default).
			o	The value shall be written in octal.
			x	The value shall be written in hexadecimal.
*/

static struct		s_flags_args nm_flags_args[NM_FLAGS] =
{
	{"-A", A_FLAGS},
	{"-g", G_FLAGS},
	{"-U", UMAJ_FLAGS}
};

static struct		s_flags_args otool_flags_args[OTOOL_FLAGS] =
{
	{"-d", DSECT_FLAGS}
};

static t_mask		get_flag_mask(const char c, bool is_otool)
{
	int8_t	i;

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

static int	check_flg(struct s_init *init, const char *arg, unsigned int *j)
{
	t_mask	mask;

	if ((ft_strncmp((const char *)((size_t)arg + *j), ARCH_ARG, ARCH_OFFSET)) == 0)
	{
		init->opt_flg.arch_name = (const char *)((size_t)arg + (*j + ARCH_OFFSET));
		if (!((ft_strcmp(init->opt_flg.arch_name, "all\0")) == 0)
		&& ((get_cpu((cpu_type_t)0, init->opt_flg.arch_name)) == NULL))
		{
			printf("./ft_nm: for the -arch option: Unknown architecture named '%s'.\n", init->opt_flg.arch_name);
			return (EXIT_FAILURE);
		}
		return (FIND_ARCH);
	}
	while (arg[*j] != '\0' && !(ft_isspace(arg[*j])))
	{
		if ((mask = get_flag_mask(arg[*j], init->is_otool)) == 0x00)
		{
			printf("./ft_nm: Unknown command line argument '%s'.\n", arg);
			return (EXIT_FAILURE);
		}
		init->opt_flg.flags |= mask;
		*j += 1;
	}
	return (EXIT_SUCCESS);
}

bool				init_flags(int ac, const char **av, struct s_init *init)
{
	int				i;
	unsigned int	j;
	int				ret;

	i = 0;
	init->opt_flg.radix_opt = 16;
	while (++i < ac)
	{
		j = 0;
		if (av[i][0] != '-')
		{
			ft_lstadd_back(&(init->args), (void *)ft_strdup(av[i]), ft_strlen(av[i]) + 1);
			init->nb_args += 1;
		}
		while (av[i][0] == '-' && av[i][j] != '\0')
		{
			if (av[i][j + 1] == '-')
				j += 2;
			else
				++j;
			if (av[i][j] == 't' && i + 1 < ac)
			{
				if ((ft_strcmp(av[i + 1], "d\0")) == 0)
					init->opt_flg.radix_opt = 10;
				else if ((ft_strcmp(av[i + 1], "o\0")) == 0)
					init->opt_flg.radix_opt = 8;
				else if ((ft_strcmp(av[i + 1], "x\0")) == 0)
					init->opt_flg.radix_opt = 16;
				else
				{
					printf("./ft_nm: for the -radix option: Cannot find option named '%s'!\n", av[i + 1]);
					return (EXIT_FAILURE);
				}
				++i;
				break ;
			}
			else if (av[i][j] == 't')
			{
				ft_putstr("./ft_nm: for the -t option: requires a value!\n");
				return (EXIT_FAILURE);
			}
			if ((ret = check_flg(init, (const char *)av[i], &j)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			else if (ret == FIND_ARCH)
				break ;
			if (av[i][j])
				++j;
		}
	}
	return (EXIT_SUCCESS);
}
