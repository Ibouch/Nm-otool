
#include <nm_otool.h>

/*
void	ARCHIVE_FILE
{
	if ((ft_strncmp((const char *)ptr, ARMAG, SARMAG)) == 0)
	{
		//write(1, ((struct ar_hdr *)((size_t)ptr + SARMAG))->ar_name, 10);
		ar_offset = 0;
		printf("%s\n", (const char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr))));
		if ((ft_strncmp((const char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr))), SYMDEF, ft_strlen(SYMDEF))) == 0)
			ar_offset = (SARMAG + sizeof(struct ar_hdr)) + ft_strlen(SYMDEF);
		if ((ft_strncmp((const char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr))), SYMDEF_SORTED, ft_strlen(SYMDEF_SORTED))) == 0)
			ar_offset = (SARMAG + sizeof(struct ar_hdr)) + ft_strlen(SYMDEF_SORTED);
		if (ar_offset)
		{
			uint32_t i = 0;
			printf("%s\n", (char *)((size_t)ptr + ar_offset));
			printf("%u\n", tt = *(uint32_t *)((size_t)ptr + 88) / sizeof(struct ranlib));
			ranlib = (struct ranlib *)((size_t)ptr + (ar_offset + sizeof(uint32_t)));
			while (i < tt)
			{
				//printf("%u\n", ranlib->ran_un.ran_strx);
				++ranlib;
				++i;
			}
		}
		//printf("%s\n", (char  * )((size_t)ptr + SARMAG));
		//printf()
		return (EXIT_SUCCESS);
	}
}*/

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

bool				parse_fat_arch(void *ptr, struct s_init *init, uint32_t	nfat_arch)
{
	const char	*name;
	uint32_t	i;

	i = 0;
	while (i < nfat_arch)
	{
		if ((cpu_type_t)SWAP32(init->fat_arch32[i].cputype) == CPU_TYPE_X86_64)
			return (check_header_file((void *)((unsigned long)ptr + SWAP32(init->fat_arch32[i].offset)), init));
		++i;
	}
	i = 0;
	while (i < nfat_arch)
	{
		name = get_cpu((cpu_type_t)SWAP32(init->fat_arch32[i].cputype), NULL);
		if (name)
			check_header_file((void *)((unsigned long)ptr + SWAP32(init->fat_arch32[i].offset)), init);
		if (++i == nfat_arch)
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

bool			check_header_file(void *ptr, struct s_init *init)
{
	if (*(unsigned int *)ptr == MH_MAGIC)
		init->is_32bits = true;
	if (*(unsigned int *)ptr == MH_MAGIC || *(unsigned int *)ptr == MH_MAGIC_64)
		return (parse_load_commands(ptr, init));
	else if (*(unsigned int *)ptr == FAT_CIGAM)
	{
		init->fat_arch32 = (struct fat_arch *)((size_t)ptr + sizeof(struct fat_header));
		return (parse_fat_arch(ptr, init, SWAP32(((struct fat_header *)ptr)->nfat_arch)));
	}
	return (EXIT_FAILURE);
}
