/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboudjel <aboudjel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 01:43:05 by aboudjel          #+#    #+#             */
/*   Updated: 2022/04/08 02:01:48 by aboudjel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_pipex.h"

void	ft_dup2(int in, int out)
{
	dup2(in, 0);
	dup2(out, 1);
}

void	ft_closeall(t_data	*general)
{
	int	i;

	i = 0;
	while (i < (general->argc - 4) * 2)
	{
		close(general->fd[i]);
		i++;
	}
}

int	ft_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc - 1)
	{
		if (ft_strlen(argv[i]) < 1)
			return (1);
		i++;
	}
	return (0);
}
