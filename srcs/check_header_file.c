
#include <nm_otool.h>

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s2;

	s2 = (char *)malloc(sizeof(char) * (n + 1));
	if (!s2)
		return (NULL);
	ft_strncpy(s2, s1, n);
	return (s2);
}

void	ARCHIVE_FILE(void *ptr, struct s_init *init)
{
	struct ar_hdr	*ar;
	char			*p;
	off_t			n;

	if ((ft_strncmp((const char *)ptr, ARMAG, SARMAG)) == 0)
	{
		ar = (struct ar_hdr *)((size_t)ptr + SARMAG);
		p = (char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr) + ft_atoi(ar->ar_size)));
		n = 0;
		while ((off_t)p + n < (off_t)ptr + init->f_size)
		{
			ft_putendl((char *)((size_t)p + (n + sizeof(struct ar_hdr))));
			ar = (struct ar_hdr *)p;
			n += sizeof(struct ar_hdr) + ft_strlen((char *)((size_t)p + (n + sizeof(struct ar_hdr)))) + ft_atoi(ar->ar_size);
		}
	}
}

const char	*get_cpu(cpu_type_t cputype, const char *name)
{
	int8_t	i;

	i = (-1);
	while (++i < N_CPU_TYPE)
		if (name == NULL)
		{
			if (cputype == g_arch_names[i].cpu_type)
				return (g_arch_names[i].name);
		}
		else
			if ((ft_strcmp(name, g_arch_names[i].name)) == 0)
				return (g_arch_names[i].name);
	return ((const char *)NULL);
}

bool				arch_is_valid(const char *arch_name)
{
	return ((arch_name == NULL || (arch_name
	&& (ft_strcmp(arch_name, "x86_64\0")) == 0)));
}

bool				parse_fat_arch(void *ptr, struct s_init *init, uint32_t	nfat_arch)
{
	uint32_t	i;
	const char	*name;

	i = 0;
	while (arch_is_valid(init->opt_flg.arch_name) && i < nfat_arch) // FAT HEADER FOR FIND X86_64
	{
		if ((cpu_type_t)SWAP32(init->fat_arch32[i].cputype) == CPU_TYPE_X86_64)
			return (check_header_file((void *)((unsigned long)ptr + SWAP32(init->fat_arch32[i].offset)), init));
		++i;
	}
	i = 0;
	while (i < nfat_arch) // ITERATE ON ALL ARCH
	{
		if ((name = get_cpu((cpu_type_t)SWAP32(init->fat_arch32[i].cputype), NULL)))
		{
			init->arch_name = name;
			check_header_file((void *)((unsigned long)ptr + SWAP32(init->fat_arch32[i].offset)), init);
		}
		if (++i == nfat_arch)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

bool			check_header_file(void *ptr, struct s_init *init)
{
	ARCHIVE_FILE(ptr, init);
	if (*(unsigned int *)ptr == MH_MAGIC || *(unsigned int *)ptr == MH_MAGIC_64)
	{
		init->is_32bits = (*(unsigned int *)ptr == MH_MAGIC) ? true : false;
		if (init->opt_flg.arch_name && !((ft_strcmp(init->opt_flg.arch_name, "all\0")) == 0))
			init->arch_name = init->opt_flg.arch_name;
		return (parse_load_commands(ptr, init));
	}
	else if (*(unsigned int *)ptr == FAT_CIGAM)
	{
		init->fat_arch32 = (struct fat_arch *)((size_t)ptr + sizeof(struct fat_header));
		return (parse_fat_arch(ptr, init, SWAP32(((struct fat_header *)ptr)->nfat_arch)));
	}
	return (EXIT_FAILURE);
}
