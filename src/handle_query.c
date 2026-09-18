#include "handle_query.h"

static void write_output(char *result, int n)
{
    char resultados[100];
    snprintf(resultados, sizeof(resultados), "resultados/command%d_output.txt", n);

    FILE *file = fopen(resultados, "w");
    if (!file)
    {
        fprintf(stderr, "Erro ao abrir o ficheiro %s para escrita\n", resultados);
        return;
    }

    fprintf(file, "%s", result); // Escreve o resultado da query no ficheiro
    fclose(file);
}

void handle_query(char *buffer, Artists artists, Songs songs, Users users, Albums albums, Histories histories, Stat_2 stat_2, Stat_3 stat_3, Stat_4 stat_4, Stat_5 stat_5, Stat_6 stat_6, unsigned int n, bool interact_mode)
{
    char *saveptr;
    char *query_nr = strtok_r(buffer, " ", &saveptr);
    char *queryArgs = strtok_r(NULL, "\n", &saveptr);
    int query = atoi(query_nr);
    char *result = "";

    if (queryArgs == NULL && query != 4)
    {
        printf("Dados inválidos\n");
        return;
    }

    char *separador_q = ";";

    if (query_nr[1] == 'S')
    {
        separador_q = "=";
    }

    if (query == 1)
    {
        result = query1(queryArgs, users, artists, separador_q, n);
    }
    else if (query == 2)
    {
        result = query2(queryArgs, artists, songs, stat_2, separador_q, n);
    }
    else if (query == 3)
    {
        result = query3(queryArgs, users, songs, stat_3, separador_q, n);
    }
    else if (query == 4)
    {
        result = query4(queryArgs, artists, songs, histories, stat_4, separador_q, n);
    }
    else if (query == 5)
    {
        result = query5(queryArgs, users, stat_5, separador_q, n);
    }
    else if (query == 6)
    {
        result = query6(queryArgs, stat_6, histories, songs, separador_q, n);
    }
    else 
    {
        printf("Introduza uma query válida[1,6]\n");
        return;
    }

    if (interact_mode == false)
    {
        // printf("Command %d:\t\"%d %s\"\n", n, query, queryArgs);
        write_output(result, n);
    }
    else
    {
        printf("Output:\n%s", result);
    }
    free(result);
}
