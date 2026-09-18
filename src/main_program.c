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
#include "../recomendador/recomendador.h"

#include <sys/resource.h>
#include <unistd.h>

#include <sys/resource.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Erro! Deve utilizar: ./programa-principal dataset/ inputs.txt\n");
        return 1;
    }

    char *path_csvs = argv[1];
    char *path_input = argv[2];

    char *SEPARATOR = ";";
    Artists artist_catalog = init_artist_catalog(10000);
    Songs song_catalog = init_song_catalog(1050000);
    Users user_catalog = init_user_catalog(400000);
    Albums album_catalog = init_album_catalog(100000);
    Histories history_catalog = init_history_catalog(13000000);

    Stat_4 stat_4 = init_stat_4();

    Stat_5 stat_5 = init_stat_5();

    parse_data(path_csvs, artist_catalog, song_catalog, user_catalog, album_catalog, history_catalog, stat_4, stat_5, SEPARATOR);

    exec_stat_4(stat_4);

    Stat_2 stat_2 = init_stat_2(artist_catalog);
    exec_stat_2(stat_2, song_catalog, artist_catalog);

    Stat_3 stat_3 = init_stat_3(10);

    Stat_6 stat_6 = init_stat_6();

    read_queries(path_input, artist_catalog, song_catalog, user_catalog, album_catalog, history_catalog, stat_2, stat_3, stat_4, stat_5, stat_6, SEPARATOR);

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

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    printf("\nMemória máxima residente (RSS): %.2f MB\n", usage.ru_maxrss / 1024.0);

    return 0;
}
