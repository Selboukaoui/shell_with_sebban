/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_handler_multi.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 12:38:38 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 16:21:42 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	handle_redirections_pipeline(int *pipefd,
										t_executor *cur)
{
	if (cur->fd_in != STDIN_FILENO)
	{
		if (dup2(cur->fd_in, STDIN_FILENO) == -1)
			return (FAIL_SYSCALL);
		close(cur->fd_in);
	}
	if (cur->fd_out != STDOUT_FILENO)
	{
		if (dup2(cur->fd_out, STDOUT_FILENO) == -1)
			return (FAIL_SYSCALL);
		close(cur->fd_out);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return (OKAY);
}

static char	*path_join(const char *dir, const char *file)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, file);
	return (full);
}

static	char	*execute_other_helper(t_executor *current)
{
	char		*candidate;
	struct stat	st;
	int			i;
	size_t		len;
	char		*dotcmd;

	i = 0;
	if (current->execs[0][0] == '/' || current->execs[0][0] == '.')
	{
		if (stat(current->execs[0], &st) == 0)
			return (ft_strdup(current->execs[0]));
		return (NULL);
	}
	if (current->path)
	{
		while (current->path[i])
		{
			candidate = path_join(current->path[i], current->execs[0]);
			if (!candidate)
				return (NULL);
			if (stat(candidate, &st) == 0)
				return (candidate);
			i++;
		}
	}
	if (!current->path)
	{
		len = 2 + ft_strlen(current->execs[0]) + 1;
		dotcmd = ft_malloc(len, 1);
		if (!dotcmd)
			return (NULL);
		ft_memcpy(dotcmd, "./", 2);
		ft_memcpy(dotcmd + 2, current->execs[0],
			ft_strlen(current->execs[0]) + 1);
		if (stat(dotcmd, &st) == 0)
			return (dotcmd);
	}
	return (NULL);
}

static	void	try_exec_with_fallback(char *path, char **args, char **envp, t_shell *shell)
{
	size_t	total_len;
	int		i;

	i = 0;
	execve(path, args, envp);
	(void)shell;
	if (errno == ENOENT || errno == EACCES || errno == ENOEXEC)
	{
		total_len = 0;
		while (args[i])
			total_len += ft_strlen(args[i++]) + 1;
		char *cmd = ft_malloc(total_len + 1, 1);
		if (!cmd)
			(ft_malloc(0,0), exit(1));
		cmd[0] = '\0';
		for (int i = 0; args[i]; i++)
		{
			strcat(cmd, args[i]);
			if (args[i + 1])
				strcat(cmd, " ");
		}
		char *sh_args[] = {"/bin/sh", "-c", cmd, NULL};
		execve("/bin/sh", sh_args, envp);
		perror("fallback execve failed");
		ft_malloc(0,0);
		exit(127);
	}
	ft_malloc(0,0);
	exit(126);
}

int	execute_other(t_executor *current, t_info *info)
{
	char	*path;
	char	**env_array;
	struct	stat	st;

	path = execute_other_helper(current);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(current->execs[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_malloc(0,0);
		exit(127);
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		ft_malloc(0,0);
		exit(126);
	}

	env_array = transform_environ_array(info->shell);
	if (!env_array)
		(ft_malloc(0, 0), exit(126));
	execve(path, current->execs, env_array);
	try_exec_with_fallback(path, current->execs,env_array, info->shell);
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(current->execs[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		(ft_malloc(0, 0), exit(126));
	}
	else
	{
		perror("minishell");
		(ft_malloc(0,0), exit(126));
	}
}

static	void	execute_builtin_child(t_info *info, t_executor *cur)
{
	int exit_code;
	char **args;

	args = cur->execs;
	exit_code = 0;
	if (ft_strcmp(args[0], "echo") == 0)
		exit_code = echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		exit_code = cd(info->shell, args);
	else if (ft_strcmp(args[0], "pwd") == 0)
		exit_code = pwd(info->shell->env);
	else if (ft_strcmp(args[0], "export") == 0)
		exit_code = export(info->shell, args);
	else if (ft_strcmp(args[0], "unset") == 0)
		exit_code = unset(info->shell, args);
	else if (ft_strcmp(args[0], "env") == 0)
		exit_code = env(info->shell);
	else if (ft_strcmp(args[0], "exit") == 0)
		exit_code = exit_builtin(info->shell, args, 1);
	ft_malloc(0, 0);
	exit(exit_code);
}

int child_handler_multi(int *fildes, t_executor *current, t_info *info)
{
	if (handle_redirections_pipeline(fildes, current) == FAIL_SYSCALL)
		(ft_malloc(0, 0), exit(FAIL_SYSCALL_CHILD));
	if (is_cmdline_empty(current->execs[0]))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(current->execs[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		(ft_malloc(0, 0), exit(127));
	}
	if (is_builtin(current->execs[0]))
		execute_builtin_child(info, current);
	else
		execute_other(current, info);
	return (FAIL_SYSCALL_CHILD);
}