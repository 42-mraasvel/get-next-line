/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/03 13:17:56 by mraasvel      #+#    #+#                 */
/*   Updated: 2020/11/09 14:02:28 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H //change this define if bonus

# include <stddef.h>

typedef struct		s_buffer
{
	char			*buffer;
	size_t			position;
	size_t			line_size;
	int				fd;
	struct s_buffer	*next;
}					t_buffer;

int					get_next_line(int fd, char **line);

/*
** utility functions
*/

void				*ft_memcpy(void *dest, const void *src, size_t n);
int					add_fd_to_lst(t_buffer **start, int fd);
int					del_fd_from_list(t_buffer **start, int fd);
int					ft_clear_lst(t_buffer **start);

#endif
