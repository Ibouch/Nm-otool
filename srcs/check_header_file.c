/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_header_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 18:52:02 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/18 18:52:06 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static struct s_cpu_type_names	arch_names[N_CPU_TYPE] =
{
	{CPU_TYPE_I386, "i386"},
	{CPU_TYPE_X86_64, "x86_64"},
	{CPU_TYPE_ARM, "arm"},
	{CPU_TYPE_ARM64, "arm64"},
	{CPU_TYPE_POWERPC, "ppc"},
	{CPU_TYPE_POWERPC64, "ppc64"}
};

const char		*get_cpu(cpu_type_t cputype, const char *name)
{
	int8_t		i;

	i = (-1);
	while (++i < N_CPU_TYPE)
		if (name == NULL)
		{
			if (cputype == arch_names[i].cpu_type)
				return (arch_names[i].name);
		}
		else if ((ft_strcmp(name, arch_names[i].name)) == 0)
			return (arch_names[i].name);
	return ((const char *)NULL);
}

static bool		arch_is_valid(const char *arch_name)
{
	return ((arch_name == NULL || (arch_name
	&& (ft_strcmp(arch_name, "x86_64\0")) == 0)));
}

static bool		iterate_all_archs(void *ptr, t_init *init, uint32_t nfat_arch)
{
	uint32_t	i;
	const char	*name;

	i = 0;
	while (i < nfat_arch)
	{
		if ((name = get_cpu((cpu_type_t)swap_32(init->fat_arch32[i].cputype),
		NULL)))
		{
			init->arch_name = name;
			check_header_file((void *)((unsigned long)ptr
			+ swap_32(init->fat_arch32[i].offset)), init);
		}
		if (++i == nfat_arch)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

static bool		parse_fat_arch(void *ptr, t_init *init, uint32_t nfat_arch)
{
	uint32_t	i;

	i = 0;
	while (arch_is_valid(init->opt_flg.arch_name) && i < nfat_arch)
	{
		if ((cpu_type_t)swap_32(init->fat_arch32[i].cputype) == CPU_TYPE_X86_64)
		{
			return (check_header_file((void *)((unsigned long)ptr
			+ swap_32(init->fat_arch32[i].offset)), init));
		}
		++i;
	}
	return (iterate_all_archs(ptr, init, nfat_arch));
}

bool			check_header_file(void *ptr, t_init *init)
{
	if (*(unsigned int *)ptr == MH_MAGIC || *(unsigned int *)ptr == MH_MAGIC_64)
	{
		init->is_32bits = (*(unsigned int *)ptr == MH_MAGIC) ? true : false;
		if (init->opt_flg.arch_name
		&& !((ft_strcmp(init->opt_flg.arch_name, "all\0")) == 0))
			init->arch_name = init->opt_flg.arch_name;
		return (parse_load_commands(ptr, init));
	}
	else if (*(unsigned int *)ptr == FAT_CIGAM)
	{
		init->fat_arch32 = (struct fat_arch *)((size_t)ptr
		+ sizeof(struct fat_header));
		return (parse_fat_arch(ptr, init,
		swap_32(((struct fat_header *)ptr)->nfat_arch)));
	}
	return (EXIT_FAILURE);
}
