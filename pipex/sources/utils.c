/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboudjel <aboudjel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 01:44:20 by aboudjel          #+#    #+#             */
/*   Updated: 2022/04/08 14:30:43 by aboudjel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_pipex.h"

t_node	*ft_nodenew(char *cmd, t_data *general)
{
	t_node	*n;

	n = malloc(sizeof(t_node));
	if (!n)
		ft_error(general, 3);
	n->cmd = ft_split(cmd, ' ', 0);
	if (n->cmd == NULL)
		ft_error(general, 3);
	n->path = NULL;
	n->next = NULL;
	return (n);
}

void	ft_nodeadd_back(t_node *lst, t_node *new)
{
	if (!lst || !new)
		return ;
	while (lst)
	{
		if (!lst->next)
			break ;
		lst = lst->next;
	}
	lst->next = new;
}

int	ft_strncmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

void	ft_strjoin(t_node *lst, char *s1, char *s2, t_data *general)
{
	char	*dst;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dst = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2)) + 1);
	if (!dst)
		ft_error(general, 3);
	while (s1 && s1[i])
	{
		dst[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		dst[i + j] = s2[j];
		j++;
	}
	dst[i + j] = '\0';
	if (s1)
		free(lst->path);
	lst->path = dst;
}
