/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboudjel <aboudjel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 01:41:54 by aboudjel          #+#    #+#             */
/*   Updated: 2022/04/08 16:05:03 by aboudjel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_pipex.h"

void	ft_pipexin(t_data *general, t_node *lst)
{
	general->pid[general->i] = fork();
	if (general->pid[general->i] < 0)
		ft_error(general, 4);
	else if (general->pid[general->i] == 0)
	{
		ft_dup2(general->infilefd, general->fd[1]);
		ft_closeall(general);
		if (!lst->path || execve(lst->path, lst->cmd, general->envp) == -1)
		{
			if (lst->path)
				write(2, "command not found: ", 19);
			else
				write(2, "permission denied:", 18);
			write(2, lst->cmd[0], ft_strlen(lst->cmd[0]));
			write(2, "\n", 1);
			exit(1);
		}
	}
}

void	ft_pipexloop(t_data *general, t_node *lst)
{
	general->pid[general->i] = fork();
	if (general->pid[general->i] < 0)
		ft_error(general, 4);
	else if (general->pid[general->i] == 0)
	{
		ft_dup2(general->fd[general->i * 2 - 2],
			general->fd[general->i * 2 + 1]);
		ft_closeall(general);
		if (execve(lst->path, lst->cmd, general->envp) == -1)
		{
			write(2, "command not found: ", 19);
			write(2, lst->cmd[0], ft_strlen(lst->cmd[0]));
			write(2, "\n", 1);
			exit(1);
		}	
	}
}

void	ft_pipexout(t_data *general, t_node *lst)
{
	if (access(lst->path, F_OK) == -1)
		general->returnvalue = 127;
	general->pid[general->i] = fork();
	if (general->pid[general->i] < 0)
		ft_error(general, 4);
	else if (general->pid[general->i] == 0)
	{
		ft_dup2(general->fd[general->i * 2 - 2], general->outfilefd);
		ft_closeall(general);
		if (execve(lst->path, lst->cmd, general->envp) == -1)
		{
			write(2, "command not found: ", 19);
			write(2, lst->cmd[0], ft_strlen(lst->cmd[0]));
			write(2, "\n", 1);
			exit(1);
		}
	}
}

void	ft_pipexdispatch(t_data *general)
{
	t_node	*lst;
	int		j;

	j = 0;
	lst = general->lst;
	ft_pipexin(general, lst);
	close(general->infilefd);
	close(general->fd[1]);
	general->i++;
	lst = lst->next;
	while (lst->next)
	{
		ft_pipexloop(general, lst);
		close(general->fd[general->i * 2 - 2]);
		close(general->fd[general->i * 2 + 1]);
		lst = lst->next;
		general->i++;
	}
	ft_pipexout(general, lst);
	close(general->fd[general->i * 2 - 2]);
	close(general->outfilefd);
	while (j <= general->i)
		waitpid(general->pid[j++], NULL, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	*general;
	int		returnvalue;

	if (argc != 5)
	{
		write(2, "error: wrong arguments\n", 23);
		exit(EXIT_FAILURE);
	}
	if (envp == NULL)
	{
		write(2, "error: no environnements\n", 25);
		exit(EXIT_FAILURE);
	}
	general = initializing(argc, argv, envp);
	getpath(general);
	ft_pipexdispatch(general);
	ft_nodeclear(general);
	ft_splitfree(general->splitpath);
	free(general->pid);
	free(general->fd);
	returnvalue = general->returnvalue;
	free(general);
	return (returnvalue);
}
