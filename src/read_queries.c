#include "read_queries.h"

void read_queries(char *path_input, Artists artists, Songs songs, Users users, Albums albums, Histories histories, Stat_2 stat_2, Stat_3 stat_3, Stat_4 stat_4, Stat_5 stat_5, Stat_6 stat_6, char *SEPARATOR)
{

    FILE *fp = NULL;
    fp = fopen(path_input, "r");
    if (fp == NULL)
    {
        printf("Não abriu ficheiro %s\n", path_input);
        return;
    }

    char *buffer = NULL;
    unsigned int counter = 0;
    size_t bufsize = 1;
    ssize_t line_size;

    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        counter++;
        handle_query(buffer, artists, songs, users, albums, histories, stat_2, stat_3, stat_4, stat_5, stat_6, counter, false);
    }

    free(buffer);
    fclose(fp);
}