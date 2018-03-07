
#include <nm_otool.h>


static long long arobj_size(struct ar_hdr* header) {
  char buf[sizeof(header->ar_size) + 1];
  memcpy(buf, header->ar_size, sizeof(header->ar_size));
  buf[sizeof(header->ar_size)] = '\0';
  return strtoll(buf, NULL, 10) + sizeof(*header);
}

void	ARCHIVE_FILE(void *ptr, struct s_init *init)
{
	uint32_t nranlibs;
	size_t	ar_offset;
	struct ranlib * ranlib;

	(void)init;
	if ((ft_strncmp((const char *)ptr, ARMAG, SARMAG)) == 0)
	{
		(size_t)ptr += SARMAG;
		while ((size_t)ptr < ((size_t)ptr + init->f_size))
		{
			struct ar_hdr* header = (struct ar_hdr*)(ptr);
			(size_t)ptr += arobj_size(header);
		}
		/*
		write(1, ((struct ar_hdr *)((size_t)ptr + SARMAG))->ar_name, 10);
		ar_offset = 0;
		printf("%s\n", (const char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr))));
		if ((ft_strncmp((const char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr))), SYMDEF, ft_strlen(SYMDEF))) == 0)
			ar_offset = (SARMAG + sizeof(struct ar_hdr)) + ft_strlen(SYMDEF);
		if ((ft_strncmp((const char *)((size_t)ptr + (SARMAG + sizeof(struct ar_hdr))), SYMDEF_SORTED, ft_strlen(SYMDEF_SORTED))) == 0)
			ar_offset = (SARMAG + sizeof(struct ar_hdr)) + ft_strlen(SYMDEF_SORTED);
		if (ar_offset)
		{
			uint32_t i = 0;
			printf("ar_offset = %zu\n", ar_offset);
			printf("%s\n", (char *)((size_t)ptr + ar_offset));
			printf("%u\n", nranlibs = *(uint32_t *)((size_t)ptr + 88) / sizeof(struct ranlib));
			ranlib = (struct ranlib *)((size_t)ptr + (ar_offset + sizeof(uint32_t)));
			printf("%u NRANLIBS\n", nranlibs);
			while (i < nranlibs)
			{
				printf("%x\n", * ((unsigned int *)((size_t)ptr + ranlib->ran_off)));
				++ranlib;
				++i;
			}
		}
		//printf("%s\n", (char  * )((size_t)ptr + SARMAG));
		//printf()
		*/
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
	return (EXIT_SUCCESS);
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
