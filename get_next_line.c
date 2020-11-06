/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/05 09:26:57 by mraasvel      #+#    #+#                 */
/*   Updated: 2020/11/06 17:22:47 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "get_next_line.h"

/*
** returns bytes available to be read.
** sets position to 0 if no more bytes left in buffer.
*/

size_t	count_bytes(t_buffer *entry)
{
	size_t	i;
	size_t	bytes_read;

	i = entry->position;
	while (entry->buffer[i] && entry->buffer[i] != '\n' && i < BUFFER_SIZE)
		i++;
	bytes_read = i - entry->position;
	if (entry->buffer[i] == '\n' && i == BUFFER_SIZE - 1)
		entry->position = 0;
	else if (entry->buffer[i] == '\0' || i == BUFFER_SIZE)
		entry->position = 0;
	else if (entry->buffer[i] == '\n' && entry->buffer[i + 1] == '\0')
		entry->position = 0;
	else
		entry->position = i + 1;
	return (bytes_read);
}

/*
** return : -1 For malloc error.
** return :  0 Finished reading line.
** return :  1 Line has been updated, but is not finished.
** Will update line_size in the list node.
** At the end it checks if a newline or null terminator was encountered.
*/

ssize_t	copy_buffer_to_line(t_buffer *entry, char **line)
{
	size_t	pos;
	size_t	new_size;
	size_t	i;
	char	*new_line;

	pos = entry->position;
	new_size = count_bytes(entry);
	new_line = (char*)malloc((new_size + entry->line_size + 1) * sizeof(char));
	if (new_line == 0)
		return (-1);
	new_line[new_size + entry->line_size] = '\0';
	ft_memcpy(new_line, *line, entry->line_size);
	ft_memcpy(new_line + (entry->line_size), (entry->buffer) + pos, new_size);
	if (entry->line_size != 0)
		free(*line);
	*line = new_line;
	entry->line_size = new_size + entry->line_size;
	i = 0;
	while (entry->buffer[pos + i] && entry->buffer[pos + i] != '\n'
	&& i < new_size)
		i++;
	if ((entry->buffer[pos + i] == '\n' || entry->buffer[pos + i] == '\0')
	&& i + pos != BUFFER_SIZE)
		return (0);
	return (1);
}

/*
** return : -1 for malloc error
** return :  0 EOF has been read.
** return :  1 Line has been read.
*/

int		read_in_file(t_buffer *entry, char **line, ssize_t line_read)
{
	ssize_t	read_return;

	while (1)
	{
		read_return = read(entry->fd, entry->buffer, BUFFER_SIZE);
		if (read_return == -1 && entry->line_size != 0)
			free(*line);
		if (read_return == -1)
			return (-1);
		entry->buffer[read_return] = 0;
		if (line_read == 0 && read_return != 0)
			return (1);
		if (line_read == 0 && read_return == 0)
			return (0);
		line_read = copy_buffer_to_line(entry, line);
		if (line_read == -1 && entry->line_size != 0)
			free(*line);
		if (line_read == -1)
			return (-1);
		if (line_read == 0 && entry->position != 0)
			return (1);
	}
	return (0);
}

/*
** 1. Add fd to linked list if not there yet.
** 2. If it was there, check the buffer for a line.
** 3. Update line_size if necessary.
** 4. Call the function to read in the next buffer.
*/

int		get_next_line(int fd, char **line)
{
	static t_buffer	*start = 0;
	t_buffer		*entry;
	ssize_t			return_value;

	return_value = (ssize_t)add_fd_to_lst(&start, fd);
	if (return_value == -1)
		return (ft_clear_lst(&start));
	entry = start;
	while (entry->next != 0 && entry->fd != fd)
		entry = entry->next;
	entry->line_size = 0;
	if (return_value == 0)
	{
		return_value = copy_buffer_to_line(entry, line);
		if (return_value == -1)
			return (ft_clear_lst(&start));
		if (return_value == 0 && entry->position != 0)
			return (1);
	}
	return_value = read_in_file(entry, line, return_value);
	if (return_value == -1)
		return (ft_clear_lst(&start));
	if (return_value == 0)
		del_fd_from_list(&start, fd);
	return (return_value);
}
