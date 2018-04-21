/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_digit_addr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ibouchla <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 13:18:10 by ibouchla          #+#    #+#             */
/*   Updated: 2018/04/20 13:18:14 by ibouchla         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm_otool.h>

static void	print_digit_addr(size_t p, uint8_t base)
{
	const char	*s;

	s = "0123456789abcdef";
	if (p > 0)
	{
		print_digit_addr(p / base, base);
		ft_putchar(s[(p % base)]);
	}
}

void		show_digit_addr(size_t addr, uint8_t base, uint8_t n_char)
{
	size_t	len;
	size_t	addr_len;

	addr_len = ft_udigitlen(addr, base);
	len = (addr_len > n_char) ? 0 : n_char - addr_len;
	len = (addr == 0) ? len + 1 : len;
	while (len--)
		ft_putchar('0');
	print_digit_addr(addr, base);
}
