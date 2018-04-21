/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_functions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 11:53:10 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 11:53:12 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

uint64_t		swap_32(uint64_t x)
{
	return (((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8)\
	| (((x) & 0xff00) << 8) | (((x) & 0xff) << 24)));
}