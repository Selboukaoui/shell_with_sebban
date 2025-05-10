#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool is_redirection(const char *s)
{
    return strcmp(s, ">") == 0 || strcmp(s, "<") == 0 ||
           strcmp(s, ">>") == 0 || strcmp(s, "<<") == 0;
}

void fix_redirection_order(char **cmd) {
    int i = 0;

    while (cmd[i]) {
        // Work within a pipe segment
        int seg_start = i;
        int seg_end = i;

        // Find end of this pipe segment
        while (cmd[seg_end] && strcmp(cmd[seg_end], "|") != 0)
            seg_end++;

        // Process only the first redirection in this segment
        for (int j = seg_start; j < seg_end - 2; j++) {
            if (is_redirection(cmd[j]) &&
                !is_redirection(cmd[j + 1]) &&
                !is_redirection(cmd[j + 2]) &&
                strcmp(cmd[j + 1], "|") != 0 &&
                strcmp(cmd[j + 2], "|") != 0) {

                // Swap j+1 and j+2
                char *tmp = cmd[j + 1];
                cmd[j + 1] = cmd[j + 2];
                cmd[j + 2] = tmp;
                break; // Only fix first redirection per segment
            }
        }

        i = seg_end;
        if (cmd[i])
            i++; // Skip the '|'
    }
}


int main()
{
    char *cmd[] = {"ls", ">", "out", ">", "grep", "txt", NULL};
    fix_redirection_order(cmd);

// Should print: ls test > out | grep txt
    for (int i = 0; cmd[i]; i++) {
        printf("%s ", cmd[i]);
    }
}
