/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: mraasvel <mraasvel@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2020/11/05 09:26:57 by mraasvel      #+#    #+#                 */
/*   Updated: 2020/11/09 14:02:21 by mraasvel      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h" // change this header if bonus

/*
** Counts bytes to read in from buffer.
** returns bytes available to be read.
** sets position to 0 if no more bytes left in buffer.
*/

size_t	count_bytes(t_buffer *entry)
{
	size_t	i;
	size_t	bytes_read;

	i = entry->position;
	while (entry->buffer[i] && entry->buffer[i] != '\n'
	&& i < (size_t)BUFFER_SIZE)
		i++;
	bytes_read = i - entry->position;
	if (entry->buffer[i] == '\n' && i + 1 == (size_t)BUFFER_SIZE)
		entry->position = 0;
	else if (entry->buffer[i] == '\0' || i == (size_t)BUFFER_SIZE)
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
** At the end it checks if a newline was encountered.
*/

int		copy_buffer_to_line(t_buffer *entry, char **line)
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
	if (entry->buffer[pos + i] == '\n' && i + pos != (size_t)BUFFER_SIZE)
		return (0);
	return (1);
}

/*
** return : -1 for malloc error
** return :  0 EOF has been read.
** return :  1 Line has been read.
** 1. Read file into buffer.
**    NULL terminate buffer.
** 2. Call copy_buffer_to_line.
**    This will clear the buffer
** 3. Return 1 if line was found in buffer,
**    else continue reading.
*/

int		read_in_file(t_buffer *entry, char **line)
{
	ssize_t	read_return;
	int		line_return;

	while (1)
	{
		read_return = read(entry->fd, entry->buffer, BUFFER_SIZE);
		if (read_return == -1 && entry->line_size != 0)
			free(*line);
		if (read_return == -1)
			return (-1);
		entry->buffer[read_return] = '\0';
		line_return = copy_buffer_to_line(entry, line);
		if (line_return == -1 && entry->line_size != 0)
			free(*line);
		if (line_return == -1)
			return (-1);
		if (read_return == 0)
			return (0);
		if (line_return == 0)
			return (1);
	}
}

/*
** Will get next line from buffer, if it's not cleared.
** If no complete line was found in buffer,
** read_in_file is called.
** If a complete line was found, 1 is returned.
** return: -1 on error.
*/

int		process_fd(char **line, t_buffer *entry)
{
	int	return_value;

	if (entry->position != 0)
	{
		return_value = copy_buffer_to_line(entry, line);
		if (return_value == -1)
			return (-1);
		if (return_value == 0)
			return (1);
	}
	return_value = read_in_file(entry, line);
	return (return_value);
}

/*
** 1. Add fd to linked list if not there yet.
** 2. If it was there, check the buffer for a line.
** 3. Update line_size if necessary.
** 4. Call the function to read in the next buffer.
** 5. Check for error returns,
** if any error was found: free all fds.
** 6. If return is (0): EOF was found;
** remove current fd fromt he list.
** 7. On EOF: return whatever was in buffer,
** if buffer is empty: return empty freeable line.
*/

int		get_next_line(int fd, char **line)
{
	static t_buffer	*start = 0;
	t_buffer		*entry;
	int				return_value;

	if (line == 0 || BUFFER_SIZE <= 0)
		return (ft_clear_lst(&start));
	return_value = add_fd_to_lst(&start, fd);
	if (return_value == -1)
		return (ft_clear_lst(&start));
	entry = start;
	while (entry->fd != fd)
		entry = entry->next;
	entry->line_size = 0;
	return_value = process_fd(line, entry);
	if (return_value == -1)
		return (ft_clear_lst(&start));
	if (return_value == 0)
		del_fd_from_list(&start, fd);
	return (return_value);
}
