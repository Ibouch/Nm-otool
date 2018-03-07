#include <nm_otool.h>


int						main(int argc, const char **argv)
{
	int				i;
	bool			err;
	struct s_init	init;
	struct s_list	*begin;

	i = 0;
	err = false;
	ft_bzero(&init, sizeof(init));
	init.is_otool = true;
	if (NO_ARG)
		init.path = "a.out";
	if ((init_flags(argc, argv, &init)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	begin = init.args;
	while (begin != NULL || NO_ARG)
	{
		if (!(NO_ARG))
			init.path = (char *)(begin->content);
		if ((map_file(&init)) == EXIT_FAILURE)
		{
			if (NO_ARG)
				break ;
			else
			{
				begin = begin->next;
				continue ;
			}
		}
		if ((unmap_file(&init, check_header_file(init.file, &init))) == EXIT_FAILURE)
			continue ;
		if (NO_ARG)
			break ;
		begin = begin->next;
	}
	//ft_lstdel(&(init.args), &lst_args_del);
//	while (1)
//		continue ;
	return ((init.error) ? EXIT_FAILURE : EXIT_SUCCESS);
}
