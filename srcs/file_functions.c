
#include <nm_otool.h>

static bool		close_file(int fd, bool *error)
{
	if (close(fd) == (-1))
	{
		fprintf(stderr, "./ft_nm: close system call has failed.\n");
		*error = true;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static bool 	open_file(struct s_init *init)
{
	struct stat	buf;

	if ((init->fd = open(init->path, O_RDONLY)) == (-1))
	{
		fprintf(stderr, "./ft_nm: %s: No such file or directory.\n", init->path);
		return (EXIT_FAILURE);
	}
	if (fstat(init->fd, &buf) == (-1))
	{
		fprintf(stderr, "./ft_nm: fstat system call has failed.\n");
		close_file(init->fd, &(init->error));
		return (EXIT_FAILURE);
	}
	if (S_IFDIR & buf.st_mode)
	{
		fprintf(stderr, "./ft_nm: %s: Is a directory.\n", init->path);
		close_file(init->fd, &(init->error));
		return (EXIT_FAILURE);
	}
	init->f_size = buf.st_size;
	return (EXIT_SUCCESS);
}

bool			map_file(struct s_init *init)
{
	if ((open_file(init)) == EXIT_FAILURE)
	{
		init->error = true;
		return (EXIT_FAILURE);
	}
	if ((init->file = (void *)mmap(0, (size_t)(init->f_size), PROT_READ, MAP_PRIVATE, init->fd, 0)) == MAP_FAILED)
	{
		fprintf(stderr, "./ft_nm: mmap system call has failed.\n");
		close_file(init->fd, &(init->error));
		init->error = true;
		return (EXIT_FAILURE);
	}
	return ((close_file(init->fd, &(init->error))) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
}

bool			unmap_file(struct s_init *init, bool ret)
{
	if ((munmap(init->file, (size_t)(init->f_size))) == -1)
	{
		fprintf(stderr, "./ft_nm: munmap system call has failed.\n");
		init->error = true;
		return (EXIT_FAILURE);
	}
	if (ret == EXIT_FAILURE)
	{
		printf("./ft_nm: %s The file was not recognized as a valid object file\n", init->path);
		init->error = true;
	}
	return (EXIT_SUCCESS);
}

static void	otool_init(struct s_init *init, struct s_otool *ot)
{
	ot->size = (init->is_32bits) ? init->pld.lcmd.sc->size : init->pld.lcmd64.sc->size;
	ot->addr = (init->is_32bits) ? init->pld.lcmd.sc->addr : init->pld.lcmd64.sc->addr;
	ot->offset = (init->is_32bits) ? init->pld.lcmd.sc->offset : init->pld.lcmd64.sc->offset;
	if (init->is_sectdata)
		ft_putstr("Contents of (__DATA,__data) section");
	else
	{
		ft_putstr(init->path);
		if (init->arch_name)
		{
			ft_putstr(" (for architecture ");
			ft_putstr(init->arch_name);
			ft_putchar(')');
		}
		ft_putstr(":\nContents of (__TEXT,__text) section");
	}
}

void		ft_otool(void *ptr, struct s_init *init)
{
	uint64_t		i;
	struct s_otool	ot;

	otool_init(init, &ot);
	i = 0;
	while (i < ot.size)
	{
		if ((i % init->opt_flg.radix_opt) == 0)
		{
			ft_putchar('\n');
			show_digit_ptr((size_t)ot.addr + i, init->opt_flg.radix_opt, (init->is_32bits) ? 8 : 16);
			ft_putchar('\t');
		}
		show_digit_ptr((*(unsigned char *)((size_t)ptr + (ot.offset + i))),
		init->opt_flg.radix_opt, (init->opt_flg.radix_opt == 16) ? 2 : 3);
		++i;
		(i == ot.size) ? ft_putchar('\n') : ft_putchar(' ');
	}
}
