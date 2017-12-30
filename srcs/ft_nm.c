#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdio.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>

void	print_output(struct symtab_command *sym, char *ptr, uint32_t tt)
{
	uint32_t		i;
	char			*string;
	struct nlist_64	*array;

	array = (void *)ptr + sym->symoff; // on se decale de symoff octet pour acceder au tableau des symbols
	string = (void *)ptr + sym->stroff;
	
	for (i = 0; i < sym->nsyms; ++i)
	{
		printf("%d %s\n", array[i].n_un.n_strx, string + array[i].n_un.n_strx);
	}
}

void	handle_64(char *ptr)
{
	uint32_t				ncmds;
	uint32_t				i;
	struct load_command		*lc;

	ncmds = ((struct mach_header_64 *)ptr)->ncmds;
	i = 0;
	lc = (struct load_command *)((size_t)ptr + sizeof(struct mach_header_64));
	while (i < ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
		{
			print_output((struct symtab_command *)lc, ptr, ((struct mach_header_64 *)ptr)->sizeofcmds);
			break ;
		}
		lc = (struct load_command *)((unsigned long)lc + lc->cmdsize);
		++i;
	}
}

void	ft_nm(char *ptr)
{
	struct mach_header_64	*t;

	if (*(int *)ptr == MH_MAGIC_64)
		handle_64(ptr);
}

int	main(int ac, char **av)
{
	int			i;
	int			fd;
	char		*ptr;
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
		if ((ptr = (char *)mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		{
			fprintf(stderr, "./ft_nm: mmap system call has failed.\n");
			return (-1);
		}
		ft_nm(ptr);
		if (munmap(ptr, buf.st_size) == (-1))
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