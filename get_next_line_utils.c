/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: syaseen <syaseen@learner.42.tech>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/10 11:31:40 by syaseen           #+#    #+#             */
/*   Updated: 2026/10/11 12:54:17 by syaseen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*(unsigned char *)s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (*(unsigned char *)s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*pto;
	unsigned char	*pfrom;

	if (!dest && !src)
		return (NULL);
	pto = (unsigned char *)dest;
	pfrom = (unsigned char *)src;
	while (n-- > 0)
		*pto++ = *pfrom++;
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*final;

	if (!s2)
	{
		free(s1);
		return (NULL);
	}
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	final = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!final)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(final, s1, len1);
	ft_memcpy(final + len1, s2, len2);
	final[len1 + len2] = '\0';
	free(s1);
	return (final);
}
