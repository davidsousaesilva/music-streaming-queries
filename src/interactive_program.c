#include <stdio.h>
#include <stdlib.h>

#include "artist_manager.h"
#include "album_manager.h"
#include "song_manager.h"
#include "user_manager.h"
#include "history_manager.h"
#include "lint.h"
#include "time.h"
#include "parser_data.h"
#include "statistics.h"
#include "read_queries.h"

int main()
{
    char path_csvs[256];
    char path_input[256];

    char *SEPARATOR = ";";
    Artists artist_catalog = init_artist_catalog(10000);
    Songs song_catalog = init_song_catalog(1050000);
    Users user_catalog = init_user_catalog(400000);
    Albums album_catalog = init_album_catalog(100000);
    Histories history_catalog = init_history_catalog(13000000);

    Stat_4 stat_4 = init_stat_4();

    Stat_5 stat_5 = init_stat_5();
    
    int sucess = 1;

    while (sucess != 0)
    {
        printf("Introduza o caminho dos ficheiros de dados:");
    
        if (fgets(path_csvs, sizeof(path_csvs), stdin) == NULL)
        {
            printf("Erro! Deve introduzir a diretoria com os dados.\n");
            return 1;
        }
        path_csvs[strcspn(path_csvs, "\n")] = '\0'; // Remover newline

        sucess =  parse_data(path_csvs, artist_catalog, song_catalog, user_catalog, album_catalog, history_catalog, stat_4, stat_5, SEPARATOR);
    
        if (sucess != 0)
        {
            printf("Erro ao carregar os dados. Por favor introduza a diretoria corretamente\n");
        }
    }
    
    exec_stat_4(stat_4);

    Stat_2 stat_2 = init_stat_2(artist_catalog);
    exec_stat_2(stat_2, song_catalog, artist_catalog);

    Stat_3 stat_3 = init_stat_3(10);

    Stat_6 stat_6 = init_stat_6();

    int counter = 0;

    bool respostas = true;

    printf("A começar abordagem das queries, quando quiser terminar o processo escreva quit ou clique ENTER!\n");

    while (respostas)
    {
        printf("Introduza a query que deseja executar:");
        if (fgets(path_input, sizeof(path_input), stdin) == NULL)
        {
            printf("Erro! Deve introduzir a query corretamente\n");
            return 1;
        }
            path_input[strcspn(path_input, "\n")] = '\0'; //Remover newline

            if (strcmp(path_input, "quit") == 0 || strlen(path_input) == 0)
            {
                printf("Fim do programa!\n");
                respostas = false;
            }
            else
            handle_query(path_input, artist_catalog, song_catalog, user_catalog, album_catalog, history_catalog, stat_2, stat_3, stat_4, stat_5, stat_6,
                    counter, true);
    }


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
    

    return 0;
}