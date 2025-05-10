#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// You can replace these with your own libft versions:
// extern char **ft_split(const char *s, char c);
// extern char  *ft_strjoin(char const *s1, char const *s2);

// // Forward declarations
// char *replace_vars(const char *cmd);
// char *process_line(const char *line);

// Helper: join array of strings with '|' between them
static char *join_commands(char **cmds)
{
    char *result = NULL;
    char *tmp;
    size_t i = 0;
    if (!cmds) return NULL;
    while (cmds[i])
    {
        if (result == NULL)
            result = strdup(cmds[i]);
        else
        {
            tmp = result;
            // add '|' then next cmd
            result = ft_strjoin(ft_strjoin(result, "|"), cmds[i]);
            free(tmp);
        }
        i++;
    }
    return result;
}

// Replace all $VAR occurrences in cmd with getenv("VAR")
char *replace_vars(const char *cmd)
{
    const char *p = cmd;
    char *out = malloc(1);
    size_t out_len = 0;
    out[0] = '\0';

    while (*p)
    {
        if (*p == '$' && (*(p+1) == '_' || isalnum(*(p+1))))
        {
            // parse variable name
            const char *start = ++p;
            while (*p == '_' || isalnum(*p))
                p++;
            char varname[64];
            size_t len = p - start;
            if (len >= sizeof(varname)) len = sizeof(varname)-1;
            memcpy(varname, start, len);
            varname[len] = '\0';

            // lookup and append
            char *val = getenv(varname);
            if (!val) val = "";
            out = realloc(out, out_len + strlen(val) + 1);
            strcpy(out + out_len, val);
            out_len += strlen(val);
        }
        else
        {
            // copy one char
            out = realloc(out, out_len + 2);
            out[out_len++] = *p++;
            out[out_len]   = '\0';
        }
    }
    return out;
}

// Main function: split on '|', replace or skip, rejoin
char *process_line(const char *line)
{
    char **parts = split(line, '|');
    if (!parts) return NULL;

    for (size_t i = 0; parts[i]; ++i)
    {
        // if this segment has no "export", replace variables
        if (!strstr(parts[i], "export"))
        {
            char *newp = replace_vars(parts[i]);
            free(parts[i]);
            parts[i] = newp;
        }
        // else: leave parts[i] untouched
    }

    // Rejoin all parts with '|'
    char *result = join_commands(parts);

    // cleanup
    for (size_t i = 0; parts[i]; ++i)
        free(parts[i]);
    free(parts);

    return result;
}

// Example usage
int main(void)
{
    const char *input = "echo $HOME | export PATH=$PATH:/foo | echo $SHELL";
    char *out = process_line(input);
    if (out)
    {
        printf("Processed: %s\n", out);
        free(out);
    }
    return 0;
}
