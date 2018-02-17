
#include <nm_otool.h>

static struct		s_flags_args flags_args[N_FLAGS] =
{
	{"-a", A_FLAGS},
	{"-b", B_FLAGS},
	{"-c", C_FLAGS},
	{"-d", D_FLAGS},
	{"-e", E_FLAGS},
	{"-f", F_FLAGS},
	{"-g", G_FLAGS},
	{"-h", H_FLAGS}
};

static t_mask		get_flag_mask(const char c)
{
	int8_t	i;

	i = (-1);
	while (++i < N_FLAGS)
		if (c == flags_args[i].flg[1])
			return (flags_args[i].mask);
	return ((t_mask)0x00);
}

static int	check_flg(const char *arg, unsigned int *j, struct s_flags_opt *opt_flg)
{
	t_mask	mask;

	if ((ft_strncmp((const char *)((size_t)arg + *j), ARCH_ARG, ARCH_OFFSET)) == 0)
	{
		opt_flg->arch_name = (const char *)((size_t)arg + (*j + ARCH_OFFSET));
		if (!((ft_strcmp(opt_flg->arch_name, "all\0")) == 0)
		&& ((get_cpu((cpu_type_t)0, opt_flg->arch_name)) == NULL))
		{
			printf("./ft_nm: for the -arch option: Unknown architecture named '%s'.\n", opt_flg->arch_name);
			return (EXIT_FAILURE);
		}
		return (FIND_ARCH);
	}
	while (arg[*j] != '\0' && !(ft_isspace(arg[*j])))
	{
		if ((mask = get_flag_mask(arg[*j])) == 0x00)
		{
			printf("./ft_nm: Unknown command line argument '%s'.\n", arg);
			return (EXIT_FAILURE);
		}
		opt_flg->flags |= mask;
		*j += 1;
	}
	return (EXIT_SUCCESS);
}

bool				init_flags(int ac, const char **av, struct s_flags_opt *opt_flg)
{
	int				i;
	unsigned int	j;
	int				ret;

	i = 0;
	while (++i < ac)
	{
		j = 0;
		while (av[i][0] == '-' && av[i][j] != '\0')
		{
			if (av[i][j + 1] == '-')
				j += 2;
			else
				++j;
			if ((ret = check_flg((const char *)av[i], &j, opt_flg)) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			else if (ret == FIND_ARCH)
				break ;
			if (av[i][j])
				++j;
		}
	}
	return (EXIT_SUCCESS);
}
