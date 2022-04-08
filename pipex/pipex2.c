# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stddef.h>
# include <errno.h>

# define ERR_ARG "invalid number of arguments\n"
# define ERR_SPLIT "split didn't work\n"
# define ERR_MAL "struct's malloc failed\n"
# define ERR_FORK "fork didn't work"
# define ERR_OUT "outfile cannot open"
# define ERR_IN "infile cannot open"

static size_t	ft_strlcpy(char *dst, const char *src, size_t size);
static unsigned int	ft_get_nb_strs(char const *s, char c);
static void	ft_get_next_str(char **next_str, unsigned int *next_str_len, char c);
static char	**ft_split(char const *s, char c, unsigned int i);

// fd[1] = write, fd[0] = read

typedef struct s_node
{
	char	**cmd;
	char	*path; 
	struct s_node	*next;
}t_node;


typedef struct s_data {
	int 	argc;
	char 	**argv;
	char	**envp;
	char	*path;
	char	**splitpath;
	t_node	*lst;
	pid_t	*pid;
	int		infilefd;
	int		outfilefd;
	int		*fd;
	int		i;
	int		returnvalue;
}t_data;

//-----------------------------------------------------------------------------------------error/perror
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
		if(tmp->path)
			free(tmp->path);
		if (tmp->cmd)
			ft_splitfree(tmp->cmd);
		list->lst = list->lst->next;
		free(tmp);
	}
	list->lst = NULL;
}

void	ft_close(t_data *general)
{
	int	i;

	i = 0;
	while (i < ((general->argc - 3) - 1) * 2)
	{
		close(general->fd[i]);
		i++;
	}
}


void ft_error(t_data *general, int key)
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
	else if (key == 21)
	{
		perror(ERR_MAL);
		close(general->infilefd);
		close(general->outfilefd);
		ft_close(general);
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
		ft_close(general);
		free(general->pid);
		if (general->splitpath)
			free(general->splitpath);
		ft_nodeclear(general);
		free(general);
		exit(1);
	}

	else if (key == 4)
	{
		perror(ERR_FORK);
		ft_splitfree(general->splitpath);
		close(general->infilefd);
		close(general->outfilefd);
		ft_close(general);
		free(general->pid);
		free(general->fd);
		ft_nodeclear(general);
		free(general);
		exit(1);
	}
}


//----------------------------------------------------------------------------------------- utils

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
			break;
		lst = lst->next;
	}
	lst->next = new;
}

int	ft_strncmp(const char *s1, const char *s2)
{
	int i;

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

//----------------------------------------------------------------------------------------- split
static size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (!dst || !src)
		return (0);
	if (size > 0)
	{
		while (--size && src[i])
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (1);
}

static unsigned int	ft_get_nb_strs(char const *s, char c)
{
	unsigned int	i;
	unsigned int	nb_strs;

	if (!s[0])
		return (0);
	i = 0;
	nb_strs = 0;
	while (s[i] && s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c)
		{
			nb_strs++;
			while (s[i] && s[i] == c)
				i++;
			continue ;
		}
		i++;
	}
	if (s[i - 1] != c)
		nb_strs++;
	return (nb_strs);
}

static void	ft_get_next_str(char **next_str, unsigned int *next_str_len, char c)
{
	unsigned int	i;

	*next_str += *next_str_len;
	*next_str_len = 0;
	i = 0;
	while (**next_str && **next_str == c)
		(*next_str)++;
	while ((*next_str)[i])
	{
		if ((*next_str)[i] == c)
			return ;
		(*next_str_len)++;
		i++;
	}
}

static char	**ft_split(char const *s, char c, unsigned int i)
{
	char			**tab;
	char			*next_str;
	unsigned int	next_str_len;
	unsigned int	nb_strs;

	if (!s)
		return (NULL);
	nb_strs = ft_get_nb_strs(s, c);
	tab = (char **)malloc(sizeof(char *) * (nb_strs + 1));
	if (!tab)
		return (NULL);
	i = 0;
	next_str = (char *)s;
	next_str_len = 0;
	while (i < nb_strs)
	{
		ft_get_next_str(&next_str, &next_str_len, c);
		tab[i] = (char *)malloc(sizeof(char) * (next_str_len + 1));
		if (!tab[i])
			return (ft_splitfree(tab));
		ft_strlcpy(tab[i], next_str, next_str_len + 1);
		i++;
	}
	tab[i] = NULL;
	return (tab);
}

void	ft_dispatchsplit(t_data *list)
{
	char	**tab;
	int		i;

	list->splitpath = ft_split(list->path+5, ':', 0);
	if(list->splitpath == NULL)
		ft_error(list, 1);
}
//----------------------------------------------------------------------------------------- 

int	extractpath(t_data *general)
{
	int	i;

	i = 0;
	while(general->envp[i])
	{
		if (ft_strncmp("PATH=", general->envp[i]))
		{
			general->path = general->envp[i];
			return(1);
		}
		i++;
	}
	return(0);
}

int	findpath(t_data *general)
{
	t_node	*tmp;
	int		returnvalue;
	int		i;

	tmp = general->lst;
	while(tmp)
	{
		i = 0;
		returnvalue = -1;
		while( returnvalue == -1 && general->splitpath[i])
		{
			ft_strjoin(tmp, general->splitpath[i], "/", general);
			ft_strjoin(tmp, tmp->path, tmp->cmd[0], general);
			returnvalue = access(tmp->path, F_OK);
			i++;
		}
		// puts("c bon");
		// if (returnvalue == -1)
		// 	ft_error(general, 4);
		tmp = tmp->next;
	}
}

int	getpath(t_data *general)
{
	int	i;

	i = 0;
	if (!extractpath(general))
		return(0);
	ft_dispatchsplit(general);
	findpath(general);
	// printf("%s\n", general->lst->path);
	// printf("%s\n", general->lst->next->path);
	// printf("%s\n", general->lst->cmd[0]);
	// // printf("%s\n", general->lst->cmd[1]);
	// printf("%s\n", general->lst->next->cmd[0]);
}

t_data *initializing2(t_data *general)
{
	int j;

	general->fd = malloc(sizeof(int) * ((general->argc - 3) - 1) * 2);
	if (!general->fd)
		ft_error(general, 2);
	j = 0;
	while (j < general->argc - 4)
	{
		if(pipe(general->fd + j * 2) < 0)
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
	return(general);
}

t_data *initializing(int argc, char *argv[], char *envp[])
{
	int		j;
	t_data	*general;

	j = 2;
	general = malloc(sizeof(t_data));
	if (!general)
	{
		perror(ERR_MAL);
		exit(1);
	}
	general->infilefd = open(argv[1], O_RDONLY);
	if (general->infilefd < 0)
		ft_error(general, 0);
	general->outfilefd = open(argv[argc - 1], O_TRUNC | O_CREAT | O_RDWR, 0000644);
	if (general->outfilefd < 0)
		ft_error(general, 1);
	general->argc = argc;
	general->argv = argv;
	general->envp = envp;
	general->i = 0;
	general->returnvalue = 0;
	general->splitpath = NULL;
	return(initializing2(general));
}


//----------------------------------------------------------------------------------------- 

void	ft_dup2(int in, int out)
{
	dup2(in, 0); //stdin read
	dup2(out, 1); //stdout write
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

void	ft_pipexin(t_data *general, t_node *lst)
{
	general->pid[general->i] = fork();
	if (general->pid[general->i] < 0)
			ft_error(general, 4);
	else if (general->pid[general->i] == 0)
	{
		ft_dup2(general->infilefd, general->fd[1]);
		ft_closeall(general); // placement ?
		if (execve(lst->path, lst->cmd, general->envp) == -1)
		{
			write(2, "command not found: ", 19);
			write(2, lst->cmd[0], ft_strlen(lst->cmd[0]));
			write(2, "\n", 1);
			exit(1);
		}
	}
}
void	ft_pipexloop(t_data *general, t_node *lst)
{

	general->pid[general->i] = fork();
	if(general->pid[general->i] < 0)
		ft_error(general, 4);
	else if(general->pid[general->i] == 0)
	{
		ft_dup2(general->fd[general->i * 2 - 2], general->fd[general->i * 2 + 1]);
		ft_closeall(general); // placement ?
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
		ft_closeall(general); // placement ?
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
	general->i++;
	lst = lst->next;
	while(lst->next)
	{
		ft_pipexloop(general, lst);
		lst = lst->next;
		general->i++;
	}
	ft_pipexout(general, lst);
	while (j <= general->i)
	{
		waitpid(general->pid[j], NULL, 0);
		j++;
	}

	ft_closeall(general);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	*general;
	int	returnvalue;

	if (envp == NULL)
		return 1;
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