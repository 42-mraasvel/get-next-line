/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/26 23:35:27 by mraasvel      #+#    #+#                 */
/*   Updated: 2020/11/27 18:51:20 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif

typedef enum	e_ret
{
	error = -1,
	eof_read,
	line_read,
	cont_read
}				t_ret;

typedef struct	s_buffer
{
	char	buffer[BUFFER_SIZE + 1];
	int		position;
}				t_buffer;

int				get_next_line(int fd, char **line);

#endif
