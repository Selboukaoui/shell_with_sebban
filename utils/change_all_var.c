/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_all_var.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asebban <asebban@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 18:01:31 by asebban           #+#    #+#             */
/*   Updated: 2025/05/09 18:24:19 by asebban          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	toggle_quote(t_varctx *ctx)
{
	char	c;

	c = ctx->in[ctx->pos[0]];
	if (c == '\'' && !ctx->double_s)
	{
		ctx->out[ctx->pos[1]++] = c;
		ctx->single = !ctx->single;
		ctx->pos[0]++;
	}
	else if (c == '"' && !ctx->single)
	{
		ctx->out[ctx->pos[1]++] = c;
		ctx->double_s = !(ctx->double_s);
		ctx->pos[0]++;
	}
}

static void	write_char(t_varctx *ctx)
{
	ctx->out[ctx->pos[1]++] = ctx->in[ctx->pos[0]++];
}

static void	handle_var(t_varctx *ctx)
{
	char	name[PATH_MAX];
	int		k;
	size_t	*p;
	char	*val;

	k = 0;
	p = ctx->pos;
	p[0]++;
	while ((ft_isalnum(ctx->in[p[0]]) || ctx->in[p[0]] == '_')
		&& k < PATH_MAX)
		name[k++] = ctx->in[p[0]++];
	name[k] = '\0';
	val = get_env_value(ctx->sh->env, name);
	if (val)
		write_value(ctx, val);
}

static	void	process_change_all_var(t_varctx *ctx)
{
	char	c;

	while (ctx->in[ctx->pos[0]])
	{
		toggle_quote(ctx);
		c = ctx->in[ctx->pos[0]];
		if ((c == '\'' && !ctx->double_s)
			|| (c == '"' && !ctx->single))
		{
			ctx->pos[0]++;
			continue ;
		}
		else if (c == '$' && !ctx->single
			&& ctx->in[ctx->pos[0] + 1]
			&& (ft_isalnum(ctx->in[ctx->pos[0] + 1])
				|| ctx->in[ctx->pos[0] + 1] == '_'))
			handle_var(ctx);
		else
			write_char(ctx);
	}
}

char	*change_all_var(const char *str, t_shell *sh)
{
	t_varctx	ctx;
	int			len;

	if (!str)
		return (NULL);
	len = calculate_max_len(str, sh);
	ctx.out = ft_malloc(len + 1, 1);
	if (!ctx.out)
		return (NULL);
	ctx.in = str;
	ctx.sh = sh;
	ctx.single = false;
	ctx.double_s = false;
	ctx.pos[0] = 0;
	ctx.pos[1] = 0;
	process_change_all_var(&ctx);
	ctx.out[ctx.pos[1]] = '\0';
	return (ctx.out);
}
