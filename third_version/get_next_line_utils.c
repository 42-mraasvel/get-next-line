/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line_utils.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/05 09:26:53 by mraasvel      #+#    #+#                 */
/*   Updated: 2020/11/07 14:10:24 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "get_next_line.h"

void		*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;

	d = dest;
	s = src;
	if (dest == 0 && src == 0)
		return (dest);
	while (n > 0)
	{
		*d = *s;
		d++;
		s++;
		n--;
	}
	return (dest);
}

/*
** Called in case of error somewhere else.
** Free each element of the list.
** Set start to NULL
*/

int			ft_clear_lst(t_buffer **start)
{
	t_buffer	*finder;

	finder = *start;
	while (finder != 0)
	{
		finder = finder->next;
		free((*start)->buffer);
		free(*start);
		*start = finder;
	}
	return (-1);
}

t_buffer	*ft_create_elem(int fd)
{
	t_buffer	*new;

	if (fd == -1)
		return (0);
	new = (t_buffer*)malloc(1 * sizeof(t_buffer));
	if (new == 0)
		return (0);
	new->buffer = (char*)malloc(BUFFER_SIZE + 1);
	if (new->buffer == 0)
	{
		free(new);
		return (0);
	}
	new->next = 0;
	new->position = 0;
	new->fd = fd;
	return (new);
}

/*
** returns : -1 for malloc error.
** returns :  0 for fd is already there.
** returns :  1 if fd was added successfully.
*/

int			add_fd_to_lst(t_buffer **start, int fd)
{
	t_buffer	*finder;

	finder = *start;
	if (*start == 0)
	{
		*start = ft_create_elem(fd);
		if (*start == 0)
			return (-1);
		return (1);
	}
	while (finder->next != 0)
	{
		if (finder->fd == fd)
			return (0);
		finder = finder->next;
	}
	if (finder->fd == fd)
		return (0);
	finder->next = ft_create_elem(fd);
	if (finder->next == 0)
		return (-1);
	return (1);
}

/*
** 1. Delete fd from list.
** 2. If there's only one element, set start to NULL.
** 3. Find element with matching fd.
** 4. Make previous point to the next in line.
** 5. Free the element.
*/

int			del_fd_from_list(t_buffer **start, int fd)
{
	t_buffer	*finder;
	t_buffer	*previous;

	finder = *start;
	previous = 0;
	if (finder->next == 0)
		*start = 0;
	while (finder != 0)
	{
		if (finder->fd == fd)
		{
			if (previous == 0)
				*start = finder->next;
			else
				previous->next = finder->next;
			free(finder->buffer);
			free(finder);
			break ;
		}
		previous = finder;
		finder = finder->next;
	}
	return (-1);
}
