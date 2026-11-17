#ifndef LINE_READER_H
# define LINE_READER_H
# include <stddef.h>

typedef struct s_line_reader
{
	char	*data;
	size_t	capacity;
	size_t	start;
	size_t	end;
	int		eof;
	int		fd;
}	t_line_reader;

int	line_reader_init(t_line_reader *reader, int fd);
void	line_reader_destroy(t_line_reader *reader);

/*
 * Returns 1 for a line, 0 for EOF, or a negative t_err on failure.
 * The returned writable, NUL-terminated view excludes the newline.
 * It is borrowed until the next call or destruction; do not free it.
 * The reader does not own the file descriptor.
 */
int	line_reader_next(t_line_reader *reader, char **line, size_t *len);
#endif
