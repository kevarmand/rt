#include "line_reader.h"
#include "errors.h"
#include "libft.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_READER_BLOCK_SIZE 65536

int	line_reader_init(t_line_reader *reader, int fd)
{
	*reader = (t_line_reader){0};
	reader->fd = fd;
	reader->capacity = LINE_READER_BLOCK_SIZE;
	reader->data = malloc(reader->capacity + 1);
	if (!reader->data)
		return (ERR_MALLOC);
	return (SUCCESS);
}

void	line_reader_destroy(t_line_reader *reader)
{
	free(reader->data);
	*reader = (t_line_reader){0};
}

static int	line_reader_grow(t_line_reader *reader)
{
	size_t	capacity;
	char	*data;

	if (reader->capacity == INT_MAX)
		return (ERR_PARS);
	capacity = reader->capacity * 2;
	if (capacity > INT_MAX)
		capacity = INT_MAX;
	data = malloc(capacity + 1);
	if (!data)
		return (ERR_MALLOC);
	ft_memcpy(data, reader->data, reader->end);
	free(reader->data);
	reader->data = data;
	reader->capacity = capacity;
	return (SUCCESS);
}

static int	line_reader_fill(t_line_reader *reader, size_t *scan)
{
	ssize_t	bytes;
	int		status;

	if (reader->start != 0)
	{
		ft_memmove(reader->data, reader->data + reader->start,
			reader->end - reader->start);
		*scan -= reader->start;
		reader->end -= reader->start;
		reader->start = 0;
	}
	if (reader->end == reader->capacity)
	{
		status = line_reader_grow(reader);
		if (status != SUCCESS)
			return (status);
	}
	bytes = read(reader->fd, reader->data + reader->end,
			reader->capacity - reader->end);
	while (bytes < 0 && errno == EINTR)
		bytes = read(reader->fd, reader->data + reader->end,
				reader->capacity - reader->end);
	if (bytes < 0)
		return (ERR_READ);
	reader->end += (size_t)bytes;
	reader->eof = (bytes == 0);
	return (SUCCESS);
}

int	line_reader_next(t_line_reader *reader, char **line, size_t *len)
{
	size_t	scan;
	char	*newline;
	int		status;

	scan = reader->start;
	while (!reader->eof)
	{
		newline = ft_memchr(reader->data + scan, '\n', reader->end - scan);
		if (newline)
		{
			*line = reader->data + reader->start;
			*len = (size_t)(newline - *line);
			*newline = '\0';
			reader->start = (size_t)(newline - reader->data) + 1;
			return (1);
		}
		scan = reader->end;
		status = line_reader_fill(reader, &scan);
		if (status != SUCCESS)
			return (status);
	}
	if (reader->start == reader->end)
		return (0);
	*line = reader->data + reader->start;
	*len = reader->end - reader->start;
	reader->data[reader->end] = '\0';
	reader->start = reader->end;
	return (1);
}
