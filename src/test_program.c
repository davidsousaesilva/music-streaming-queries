#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/resource.h>

#include "handle_query.h"
#include "song_manager.h"
#include "artist_manager.h"
#include "user_manager.h"
#include "album_manager.h"
#include "history_manager.h"
#include "parser_data.h"
#include "statistics.h"

#define MAX_LINE_LENGTH 1024

// Função para comparar dois arquivos linha por linha
int compare_files(const char *file1, const char *file2)
{
    FILE *fp1 = fopen(file1, "r");
    FILE *fp2 = fopen(file2, "r");

    if (!fp1 || !fp2)
    {
        perror("Erro ao abrir um dos arquivos\n");
        if (fp1)
            fclose(fp1);
        if (fp2)
            fclose(fp2);
        return -1;
    }

    char line1[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
    int line_num = 1;

    // Lê os arquivos linha a linha e compara
    while (fgets(line1, sizeof(line1), fp1) && fgets(line2, sizeof(line2), fp2))
    {
        if (strcmp(line1, line2) != 0)
        {
            printf("-----\nDiferença encontrada na linha %d:\n", line_num);
            printf("  %s: %s", file1, line1);
            printf("  %s: %s", file2, line2);
            fclose(fp1);
            fclose(fp2);
            return 1;
        }
        line_num++;
    }

    fclose(fp1);
    fclose(fp2);
    return 0;
}

// Função para escrever o resultado das queries
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

void test_queries(char *path_input, char *dir_result, Artists artists, Songs songs, Users users, Histories histories, Stat_2 stat_2, Stat_3 stat_3, Stat_4 stat_4, Stat_5 stat_5, Stat_6 stat_6)
{
    clock_t start_time, end_time;
    double query1_time = 0, query2_time = 0, query3_time = 0, query4_time = 0, query5_time = 0, query6_time = 0;
    double query1_count = 0, query2_count = 0, query3_count = 0, query4_count = 0, query5_count = 0, query6_count = 0;

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

    printf("----------------------------------------------------iDiff\n");

    // Resolver as queries e escrever o seu output
    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        char *saveptr;
        char *query_nr = strtok_r(buffer, " ", &saveptr);
        char *queryArgs = strtok_r(NULL, "\n", &saveptr);
        int query = atoi(query_nr);
        char *result = "";

        counter++;

        char *separador_q = ";";

        if (query_nr[1] == 'S')
        {
            separador_q = "=";
        }

        if (query == 1)
        {
            query1_count++;
            start_time = clock();
            result = query1(queryArgs, users, artists, separador_q, counter);
            end_time = clock();
            query1_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }
        else if (query == 2)
        {
            query2_count++;
            start_time = clock();
            result = query2(queryArgs, artists, songs, stat_2, separador_q, counter);
            end_time = clock();
            query2_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }
        else if (query == 3)
        {
            query3_count++;
            start_time = clock();
            result = query3(queryArgs, users, songs, stat_3, separador_q, counter);
            end_time = clock();
            query3_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }
        else if (query == 4)
        {
            query4_count++;
            start_time = clock();
            result = query4(queryArgs, artists, songs, histories, stat_4, separador_q, counter);
            end_time = clock();
            query4_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }
        else if (query == 5)
        {
            query5_count++;
            start_time = clock();
            result = query5(queryArgs, users, stat_5, separador_q, counter);
            end_time = clock();
            query5_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }
        else if (query == 6)
        {
            query6_count++;
            start_time = clock();
            result = query6(queryArgs, stat_6, histories, songs, separador_q, counter);
            end_time = clock();
            query6_time += ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        }

        write_output(result, counter);

        char resultados[100];
        snprintf(resultados, sizeof(resultados), "resultados/command%d_output.txt", counter);

        char resultados_esperados[100];
        snprintf(resultados_esperados, sizeof(resultados_esperados), "%s/command%d_output.txt", dir_result, counter);

        int compare_result = compare_files(resultados, resultados_esperados);
        if (compare_result != 0)
        {
            printf("Diferença encontrada na query %d!\n-----\n", counter);
        }
        free(result);
    }

    free(buffer);
    fclose(fp);

    // Imprimir os tempos de execução
    printf("\nTempo médio da Query 1: %.4f segundos\n", query1_time / query1_count);
    printf("Tempo médio da Query 2: %.4f segundos\n", query2_time / query2_count);
    printf("Tempo médio da Query 3: %.4f segundos\n", query3_time / query3_count);
    printf("Tempo médio da Query 4: %.4f segundos\n", query4_time / query4_count);
    printf("Tempo médio da Query 5: %.4f segundos\n", query5_time / query5_count);
    printf("Tempo médio da Query 6: %.4f segundos\n", query6_time / query6_count);

    printf("----------------------------------------------------eDiff\n");

}

// Função principal que configura os dados e inicia os testes
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Erro! Deve utilizar ./programa-testes dataset/ inputs.txt resultados-esperados/\n");
        return -1;
    }

    printf("\n--------------------------------------------------------------iTest\n");

    clock_t start_time, end_time;
    start_time = clock();

    char *path_csvs = argv[1];
    char *path_input = argv[2];
    char *dir_result = argv[3];

    // Fazer parse do dataset
    char *SEPARATOR = ";";
    Artists artist_catalog = init_artist_catalog(1000);
    Songs song_catalog = init_song_catalog(100000);
    Users user_catalog = init_user_catalog(200000);
    Albums album_catalog = init_album_catalog(10000);
    Histories history_catalog = init_history_catalog(1200000);

    Stat_4 stat_4 = init_stat_4();

    Stat_5 stat_5 = init_stat_5();


    parse_data(path_csvs, artist_catalog, song_catalog, user_catalog, album_catalog, history_catalog, stat_4, stat_5, SEPARATOR);

    // Iniciar estatísticas para as queries
    Stat_2 stat_2 = init_stat_2(artist_catalog);
    exec_stat_2(stat_2, song_catalog, artist_catalog);
    Stat_3 stat_3 = init_stat_3(10);

    exec_stat_4(stat_4);

    Stat_6 stat_6 = init_stat_6();

    // Executar os testes
    test_queries(path_input, dir_result, artist_catalog, song_catalog, user_catalog, history_catalog, stat_2, stat_3, stat_4, stat_5, stat_6);

    destroy_stat_2(stat_2);
    destroy_stat_3(stat_3);
    destroy_stat_4(stat_4);
    destroy_stat_5(stat_5);
    destroy_stat_6(stat_6);
    destroy_song_catalog(song_catalog);
    destroy_artist_catalog(artist_catalog);
    destroy_user_catalog(user_catalog);
    destroy_history_catalog(history_catalog);
    destroy_album_catalog(album_catalog);

    end_time = clock();

    double tempo_total = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Tempo total do programa: %f segundos\n", tempo_total);

    // Calcular e mostrar a memória usada
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    double memory_usage_mb = r_usage.ru_maxrss / 1024.0; // Converte o uso de memória de KB para MB
    printf("\nMemória usada: %.2f MB\n", memory_usage_mb);
    printf("--------------------------------------------------------------eTest\n\n");

    return 0;
}