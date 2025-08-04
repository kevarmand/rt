#include "rt.h"



char	*get_type_name(t_type type)
{
	if (type == SPHERE)
		return ("sphere ");
	if (type == PLANE)
		return ("plane ");
	if (type == CYLINDER)
		return ("cylinder ");
	if (type == TRIANGLE)
		return ("triangle ");
	if (type == LIGHT)
		return ("light ");
	if (type == CAM)
		return ("camera ");
	return ("object ");
}

char	*get_name(t_type type, int i)
{
	char	*num;
	char	*name;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	name = ft_strjoin2(get_type_name(type), num);
	free(num);
	return (name);
}


/***
 * faudra peut etre retourner un int pour gerer les erreurs quoi que on peut exit
 */
void	name_lst(t_list *lst)
{
	int i;
	
	i = 0;
	printf("Name list:\n");
	while (lst)
	{
		lst->name = get_name(lst->type, i);
		if (lst->name == NULL)
		{
			fprintf(stderr, "Error: Failed to allocate memory for name.\n");
			exit(EXIT_FAILURE);
		}

		printf("Name: \"%s\", Type: %d\n", lst->name, lst->type);
		lst = lst->next;
		i++;

	}
}