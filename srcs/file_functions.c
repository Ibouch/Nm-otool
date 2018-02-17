
#include <nm_otool.h>

static bool		close_file(int fd)
{
	if (close(fd) == (-1))
	{
		fprintf(stderr, "./ft_nm: close system call has failed.\n");
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
		close_file(init->fd);
		return (EXIT_FAILURE);
	}
	if (S_IFDIR & buf.st_mode)
	{
		fprintf(stderr, "./ft_nm: %s: Is a directory.\n", init->path);
		close_file(init->fd);
		return (EXIT_FAILURE);
	}
	init->f_size = buf.st_size;
	return (EXIT_SUCCESS);
}

bool			map_file(struct s_init *init)
{
	if ((open_file(init)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if ((init->file = (void *)mmap(0, (size_t)(init->f_size), PROT_READ, MAP_PRIVATE, init->fd, 0)) == MAP_FAILED)
	{
		fprintf(stderr, "./ft_nm: mmap system call has failed.\n");
		close_file(init->fd);
		return (EXIT_FAILURE);
	}
	return ((close_file(init->fd)) == EXIT_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
}

bool			unmap_file(struct s_init *init, bool *err, bool ret)
{
	if ((munmap(init->file, (size_t)(init->f_size))) == -1)
	{
		fprintf(stderr, "./ft_nm: munmap system call has failed.\n");
		return (EXIT_FAILURE);
	}
	if (ret == EXIT_FAILURE)
	{
		printf("./ft_nm: %s The file was not recognized as a valid object file\n", init->path);
		*err = true;
	}
	return (EXIT_SUCCESS);
}
