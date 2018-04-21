/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/18 18:52:44 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/18 18:52:46 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

int					main(int argc, const char **argv)
{
	struct s_init	init;
	struct s_list	*begin;

	ft_bzero(&init, sizeof(init));
	if (init.nb_args == 0)
		init.path = "a.out";
	init.is_otool = true;
	if ((init_flags(argc, argv, &init)) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	begin = init.args;
	parse_file(&init, begin);
	return ((init.error) ? EXIT_FAILURE : EXIT_SUCCESS);
}
