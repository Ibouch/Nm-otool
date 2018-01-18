#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <stdbool.h>
#include <libft.h>
#include <nm_otool.h>

void	get_symbol_section(struct s_list *lst, uint8_t id, char *c)
{
	while (lst && --id)
		lst = lst->next;
	if ((ft_strcmp(((struct s_section *)(lst->content))->sectname, SECT_TEXT)) == 0)
		*c = 'T';
	else if ((ft_strcmp(((struct s_section *)(lst->content))->sectname, SECT_DATA)) == 0)
		*c = 'D';
	else if ((ft_strcmp(((struct s_section *)(lst->content))->sectname, SECT_BSS)) == 0)
		*c = 'B';
	else if ((ft_strcmp(((struct s_section *)(lst->content))->sectname, SECT_COMMON)) == 0)
		*c = 'C';
	else
		*c = 'S';
}

void	print_output(t_list *lst, struct symtab_command *sym, char *ptr)
{
	uint32_t		i;
	char			*string;
	struct nlist_64	*array;
	bool			is_ext;
	char			c;

	array = (struct nlist_64 *)((size_t)ptr + sym->symoff); // on se decale de symoff octet pour acceder au tableau des symbols
	string = (char *)((size_t)ptr + sym->stroff);
	for (i = 0; i < sym->nsyms; ++i)
	{
		is_ext = false;
		if ((array[i].n_type & N_EXT))
			is_ext = true;
		if ((array[i].n_type & N_STAB)) // skip debug symbol pour les print -a
			c = '-';
		else if ((array[i].n_type & N_TYPE) == N_UNDF)
			c = 'U';
		else if ((array[i].n_type & N_TYPE) == N_ABS)
			c = 'A';
		else if ((array[i].n_type & N_TYPE) == N_SECT && array[i].n_sect)
			get_symbol_section(lst, array[i].n_sect, &c);
		else if ((array[i].n_type & N_TYPE) == N_PBUD)
			c = 'u';
		else if ((array[i].n_type & N_TYPE) == N_INDR)
			c = 'I';
		if (array[i].n_value)
			printf("%016llx ", array[i].n_value);
		else
			write(1, "                 ", 17);
		c = ((!is_ext) ? (char)ft_tolower((int)c) : c);
		printf("%c %s\n", c, string + array[i].n_un.n_strx);
	}
}

void							handle_64(void *ptr)
{
	uint32_t					ncmds;
	uint32_t					i;
	struct load_command			*lc;
	struct segment_command_64	*sg;
	struct section_64			*sc;
	struct s_section			sect;
	struct s_list				*lst;

	ncmds = ((struct mach_header_64 *)ptr)->ncmds;
	i = 0;
	lc = (struct load_command *)((size_t)ptr + sizeof(struct mach_header_64));
	lst = NULL;
	while (i < ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			sg = (struct segment_command_64 *)lc;
			sc = (struct section_64 *)((size_t)sg + sizeof(struct segment_command_64));
			for (uint32_t j = 0; j < sg->nsects; j++)
			{
				sect.segname = ft_strdup(sc->segname);
				sect.sectname = ft_strdup(sc->sectname);
				ft_lstadd_back(&lst, (void *)&sect, sizeof(sect));
				sc = (struct section_64 *)((size_t)sc + sizeof(struct section_64));
			}
		}
		if (lc->cmd == LC_SYMTAB)
		{
			print_output(lst, (struct symtab_command *)lc, ptr);
			break ;
		}
		lc = (struct load_command *)((unsigned long)lc + lc->cmdsize);
		++i;
	}
}

void	ft_nm(char *ptr)
{
	if (*(unsigned int *)ptr == MH_MAGIC_64)
		handle_64(ptr);
	/*else if (*(int *)ptr == MH_MAGIC)
		handle_32(ptr);
	else
		unknown_arch*/
}

int	main(int ac, char **av)
{
	int			i;
	int			fd;
	void		*ptr;
	struct stat	buf;

	i = 0;
	while (++i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) == (-1))
		{
			fprintf(stderr, "./ft_nm: %s: No such file or directory.\n", av[i]);
			return (-1);
		}
		if (fstat(fd, &buf) == (-1))
		{
			fprintf(stderr, "./ft_nm: fstat system call has failed.\n");
			return (-1);
		}
		if (S_IFDIR & buf.st_mode)
		{
			fprintf(stderr, "./ft_nm: %s: Is a directory.\n", av[i]);
			return (-1);
		}
		if ((ptr = (void *)mmap(0, (size_t)buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		{
			fprintf(stderr, "./ft_nm: mmap system call has failed.\n");
			return (-1);
		}
		ft_nm(ptr);
		if (munmap(ptr, (size_t)buf.st_size) == (-1))
		{
			fprintf(stderr, "./ft_nm: munmap system call has failed.\n");
			return (-1);
		}
		if (close(fd) == (-1))
		{
			fprintf(stderr, "./ft_nm: close system call has failed.\n");
			return (-1);
		}
	}
}
