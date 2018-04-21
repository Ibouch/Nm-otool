/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 18:52:29 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/18 18:52:34 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static bool		close_file(int fd, bool *error)
{
	if (close(fd) == (-1))
	{
		ft_putstr_fd("Nm-Otool: close system call has failed.\n", 2);
		*error = true;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static bool		open_file(struct s_init *init)
{
	struct stat	buf;

	if ((init->fd = open(init->path, O_RDONLY)) == (-1))
	{
		ft_putstr_fd("Nm-Otool: ", 2);
		ft_putstr_fd(init->path, 2);
		ft_putstr_fd(": No such file or directory.\n", 2);
		return (EXIT_FAILURE);
	}
	if (fstat(init->fd, &buf) == (-1))
	{
		ft_putstr_fd("Nm-Otool: fstat system call has failed.\n", 2);
		close_file(init->fd, &(init->error));
		return (EXIT_FAILURE);
	}
	if (S_IFDIR & buf.st_mode)
	{
		ft_putstr_fd("Nm-Otool: ", 2);
		ft_putstr_fd(init->path, 2);
		ft_putstr_fd(": Is a directory.\n", 2);
		close_file(init->fd, &(init->error));
		return (EXIT_FAILURE);
	}
	init->f_size = buf.st_size;
	return (EXIT_SUCCESS);
}

static bool		map_file(struct s_init *init)
{
	if ((open_file(init)) == EXIT_FAILURE)
	{
		init->error = true;
		return (EXIT_FAILURE);
	}
	if ((init->file = (void *)mmap(0, (size_t)(init->f_size), PROT_READ,
	MAP_PRIVATE, init->fd, 0)) == MAP_FAILED)
	{
		ft_putstr_fd("Nm-Otool: mmap system call has failed.\n", 2);
		close_file(init->fd, &(init->error));
		init->error = true;
		return (EXIT_FAILURE);
	}
	return ((close_file(init->fd, &(init->error))) == EXIT_SUCCESS
	? EXIT_SUCCESS : EXIT_FAILURE);
}

static bool		unmap_file(struct s_init *init, bool ret)
{
	if ((munmap(init->file, (size_t)(init->f_size))) == -1)
	{
		ft_putstr_fd("Nm-Otool: munmap system call has failed.\n", 2);
		init->error = true;
		return (EXIT_FAILURE);
	}
	if (ret == EXIT_FAILURE)
	{
		ft_putstr_fd("Nm-Otool: ", 2);
		ft_putstr_fd(init->path, 2);
		ft_putstr_fd(" The file was not recognized as a \
valid object file\n", 2);
		init->error = true;
	}
	return (EXIT_SUCCESS);
}

void			parse_file(struct s_init *init, struct s_list *begin)
{
	while (begin != NULL || init->nb_args == 0)
	{
		if (!(init->nb_args == 0))
			init->path = (char *)(begin->content);
		if ((map_file(init)) == EXIT_FAILURE)
		{
			if (init->nb_args == 0)
				break ;
			else
			{
				begin = begin->next;
				continue ;
			}
		}
		if ((unmap_file(init, check_header_file(init->file, init)))
		== EXIT_FAILURE)
			continue ;
		if (init->nb_args == 0)
			break ;
		begin = begin->next;
	}
}
