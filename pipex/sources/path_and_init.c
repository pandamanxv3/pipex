/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_and_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboudjel <aboudjel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 01:43:35 by aboudjel          #+#    #+#             */
/*   Updated: 2022/04/08 16:05:25 by aboudjel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_pipex.h"

void	extractpath(t_data *general)
{
	int	i;

	i = 0;
	while (general->envp[i])
	{
		if (ft_strncmp("PATH=", general->envp[i]))
		{
			general->path = general->envp[i];
			return ;
		}
		i++;
	}
	ft_error(general, 5);
}

void	findpath(t_data *general)
{
	t_node	*tmp;
	int		returnvalue;
	int		i;

	tmp = general->lst;
	while (tmp)
	{
		i = 0;
		if (tmp->cmd[0] == NULL)
			tmp->path = NULL;
		else
		{
			ft_strjoin(tmp, tmp->path, tmp->cmd[0], general);
			returnvalue = access(tmp->path, F_OK);
			while (returnvalue == -1 && general->splitpath[i])
			{
				ft_strjoin(tmp, general->splitpath[i], "/", general);
				ft_strjoin(tmp, tmp->path, tmp->cmd[0], general);
				returnvalue = access(tmp->path, F_OK);
				i++;
			}
		}
		tmp = tmp->next;
	}
}

void	getpath(t_data *general)
{
	extractpath(general);
	ft_dispatchsplit(general);
	findpath(general);
}

t_data	*initializing2(t_data *general)
{
	int	j;

	general->fd = malloc(sizeof(int) * ((general->argc - 3) - 1) * 2);
	if (!general->fd)
		ft_error(general, 2);
	j = 0;
	while (j < general->argc - 4)
	{
		if (pipe(general->fd + j * 2) < 0)
			ft_error(general, 21);
		j++;
	}
	general->pid = malloc(sizeof(int) * (general->argc - 3));
	if (!general->pid)
		ft_error(general, 21);
	j = 2;
	general->lst = NULL;
	general->lst = ft_nodenew(general->argv[j], general);
	while (j < general->argc - 2)
	{
		j++;
		ft_nodeadd_back(general->lst,
			ft_nodenew(general->argv[j], general));
	}
	return (general);
}

t_data	*initializing(int argc, char *argv[], char *envp[])
{
	t_data	*general;

	general = malloc(sizeof(t_data));
	if (!general)
	{
		perror(ERR_MAL);
		exit(1);
	}
	general->infilefd = open(argv[1], O_RDONLY);
	if (general->infilefd < 0)
		ft_error(general, 0);
	general->outfilefd = open(argv[argc - 1],
			O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (general->outfilefd < 0)
		ft_error(general, 1);
	general->argc = argc;
	general->argv = argv;
	general->envp = envp;
	general->i = 0;
	general->returnvalue = 0;
	general->splitpath = NULL;
	return (initializing2(general));
}
