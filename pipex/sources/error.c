/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboudjel <aboudjel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 01:44:47 by aboudjel          #+#    #+#             */
/*   Updated: 2022/04/08 12:44:33 by aboudjel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_pipex.h"

char	**ft_splitfree(char **tab)
{
	unsigned int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

void	ft_nodeclear(t_data *list)
{
	t_node	*tmp;

	while (list->lst)
	{
		tmp = list->lst;
		if (tmp->path)
			free(tmp->path);
		if (tmp->cmd)
			ft_splitfree(tmp->cmd);
		list->lst = list->lst->next;
		free(tmp);
	}
	list->lst = NULL;
}

void	ft_error3(t_data *general, int key)
{
	if (key == 4)
	{
		perror(ERR_FORK);
		ft_splitfree(general->splitpath);
		close(general->infilefd);
		close(general->outfilefd);
		ft_closeall(general);
		free(general->pid);
		free(general->fd);
		ft_nodeclear(general);
		free(general);
		exit(1);
	}
	else if (key == 5)
	{
		write(2, ERR_ENV, sizeof(ERR_ENV));
		close(general->infilefd);
		close(general->outfilefd);
		free(general->fd);
		ft_closeall(general);
		free(general->pid);
		ft_nodeclear(general);
		free(general);
		exit(1);
	}
}

void	ft_error2(t_data *general, int key)
{
	if (key == 21)
	{
		perror(ERR_MAL);
		close(general->infilefd);
		close(general->outfilefd);
		ft_closeall(general);
		free(general->fd);
		free(general);
		exit(1);
	}
	else if (key == 3)
	{
		perror(ERR_MAL);
		close(general->infilefd);
		close(general->outfilefd);
		free(general->fd);
		ft_closeall(general);
		free(general->pid);
		if (general->splitpath)
			free(general->splitpath);
		ft_nodeclear(general);
		free(general);
		exit(1);
	}
	ft_error3(general, key);
}

void	ft_error(t_data *general, int key)
{
	if (key == 0)
	{
		perror(ERR_IN);
		free(general);
		exit(0);
	}
	else if (key == 1)
	{
		perror(ERR_OUT);
		close(general->infilefd);
		free(general);
		exit(1);
	}
	else if (key == 2)
	{
		perror(ERR_MAL);
		close(general->infilefd);
		close(general->outfilefd);
		free(general);
		exit(1);
	}
	ft_error2(general, key);
}
