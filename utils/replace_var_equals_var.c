/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var_equals_var.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:52:22 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 14:11:52 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static	int	var_name_len(const char *seg, int pos)
{
	int	len;

	len = 0;
	while (seg[pos + len] && (ft_isalnum(seg[pos + len])
			|| seg[pos + len] == '_'))
		len++;
	return (len);
}

static	void	append_var(const char *seg, t_shell *shell,
				char *result, int ptrs[2])
{
	int		start;
	int		len;
	char	*name;
	char	*val;

	start = ptrs[0] + 1;
	len = var_name_len(seg, start);
	name = ft_malloc(len + 1, 1);
	if (!name)
		return ;
	ft_strncpy(name, &seg[start], len);
	name[len] = '\0';
	val = get_env_value(shell->env, name);
	if (!val)
		val = "";
	ft_strcpy(&result[ptrs[1]], val);
	ptrs[1] += ft_strlen(val);
	ptrs[0] = start + len;
}

char	*ft_replace_var(const char *seg, t_shell *shell)
{
	int		ptrs[2];
	int		total;
	char	*result;

	if (!seg || !shell)
		return (NULL);
	ptrs[0] = 0;
	ptrs[1] = 0;
	total = ft_strlen(seg) + all_value((char *)seg, shell) + 1;
	result = ft_malloc(total, 1);
	if (!result)
		return (NULL);
	while (seg[ptrs[0]])
	{
		if (seg[ptrs[0]] == '$'
			&& (ft_isalpha(seg[ptrs[0] + 1]) || seg[ptrs[0] + 1] == '_'))
			append_var(seg, shell, result, ptrs);
		else
			result[ptrs[1]++] = seg[ptrs[0]++];
	}
	result[ptrs[1]] = '\0';
	return (result);
}

static	char	*ft_check_segments(char *seg, t_shell *shell)
{
	int		i;
	bool	has_equal;

	i = 0;
	has_equal = false;
	while (seg[i])
	{
		if (seg[i] == '=')
		{
			has_equal = true;
			break ;
		}
		if (seg[i] == '$')
			break ;
		i++;
	}
	if (has_equal && seg[i] != '$')
		return (seg);
	else
		// while (seg[i])
		// 	if (seg[i++] == '=')
		// 		return (ft_replace_var(seg, shell));
	// return (seg);
		return (ft_replace_var(seg, shell));
}

char	*replace_var_equals_var(char *input, t_shell *shell)
{
	char	**tokens;
	size_t	i;
	char	*output;
	char	*tmp;
	char	*new_output;

	tokens = ft_split(input, ' ');
	if (!tokens)
		return (NULL);
	output = ft_strdup("");
	i = 0;
	while (tokens[i])
	{
		tmp = ft_check_segments(tokens[i], shell);
		new_output = ft_strjoin(output, tmp);
		output = new_output;
		if (tokens[i + 1])
		{
			new_output = ft_strjoin(output, " ");
			output = new_output;
		}
		i++;
	}
	return (output);
}
