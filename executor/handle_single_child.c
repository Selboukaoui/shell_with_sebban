/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_single_child.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:13:22 by asebban           #+#    #+#             */
/*   Updated: 2025/05/06 16:40:26 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void try_exec_with_fallback(char *path, char **args, char **envp, t_shell *shell)
{
	execve(path, args, envp);

	// If execve fails
	// perror("execve failed"); // Optional: print error
	(void)shell;
	if (errno == ENOENT || errno == EACCES || errno == ENOEXEC)
	{
		// Build the fallback command string
		// For example: sh -c "ls -la"
		size_t total_len = 0;
		for (int i = 0; args[i]; i++)
			total_len += strlen(args[i]) + 1;

		char *cmd = ft_malloc(total_len + 1, 1);
		if (!cmd)
		{
			

			// free_enviro(shell); // check
			ft_malloc(0,0);
			exit(1);
		}

		cmd[0] = '\0';
		for (int i = 0; args[i]; i++)
		{
			strcat(cmd, args[i]);
			if (args[i + 1])
				strcat(cmd, " ");
		}

		// Now execute: /bin/sh -c "your command string"
		char *sh_args[] = {"/bin/sh", "-c", cmd, NULL};
		execve("/bin/sh", sh_args, envp);
		perror("fallback execve failed");

		// free_enviro(shell);
		ft_malloc(0,0);
		exit(127); // fallback also failed
	}
 
	// free_enviro(shell);
	ft_malloc(0,0);
	exit(126); // original execve failed for other reasons
}

void handle_single_child(t_shell *shell)
{
	char        *path;
	char        **env_array;
	int          saved_stdin;
	int          saved_stdout;
	struct stat  st;
	char        *cmd = shell->executor->execs[0];

	saved_stdin  = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	if (!handle_redirections_single_child(shell))
	{
		
		
		// free_enviro(shell);
		ft_malloc(0,0);   
		exit(EXIT_FAILURE);
	}

	if (is_cmdline_empty(cmd))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	
		// free_enviro(shell);
		ft_malloc(0,0);
		exit(127);
	}

	if (strchr(cmd, '/'))
	{
		// Literal path
		path = cmd;
	}
	else
	{
		// 1) Try to resolve via PATH in shell->env
		path = get_path(shell, true);

		// 2) If that failed, n if our shell’s PATH is unset/empty
		if (!path)
		{
			char *path_env = get_env_value(shell->env, "PATH");
			if (!path_env || *path_env == '\0')
			{
				// build "./cmd"
				size_t len = strlen(cmd) + 3;
				path = ft_malloc(len, 1);
				if (!path)
				{
					
					// free_enviro(shell);
					ft_malloc(0,0);
					exit(126);
				}
				ft_memcpy(path, "./", 2);
				ft_memcpy(path + 2, cmd, strlen(cmd) + 1);
			}
		}

		// 3) If still no path, error out
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
			
			// free_enviro(shell);
			ft_malloc(0,0);
			exit(127);
		}
	}

	// Now verify it exists
	if (stat(path, &st) != 0)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		// if (path != cmd) free(path);
		
		// free_enviro(shell);
		ft_malloc(0,0);
		exit(127);
	}

	// Directory?
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		// if (path != cmd) free(path);
		

		// free_enviro(shell);
		ft_malloc(0,0);
		exit(126);
	}

	env_array = transform_environ_array(shell);
	if (!env_array)
	{
		// if (path != cmd) free(path);
		
		// free_enviro(shell);
		ft_malloc(0,0);
		exit(126);
	}

	execve(path, shell->executor->execs, env_array);
	try_exec_with_fallback(path, shell->executor->execs, env_array, shell);
	// execve failed
	perror("minishell");
	dup2(saved_stdin,  STDIN_FILENO);//dup in try
	dup2(saved_stdout, STDOUT_FILENO);
	// if (path != cmd) free(path);
	

	// free_enviro(shell);
	ft_malloc(0,0);
	exit(126);
}
