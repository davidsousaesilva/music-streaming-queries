#include "song_manager.h"

struct songs
{
    Hashtable table;
    int nr_songs;
};

Songs init_song_catalog(int num_buckets)
{
    // Alocar memória para catálogo de músicas
    Songs catalog = malloc(sizeof(struct songs));

    if (!catalog)
        return NULL; // Falha na alocação

    catalog->nr_songs = 0;

    // Função hashtable_init para iniciar a tabela hash
    catalog->table = hashtable_init(num_buckets);
    if (!catalog->table)
    {
        free(catalog); // Libertar a memória alocada para o catálogo
        return NULL;
    }
    return catalog;
}

bool insert_song(Songs catalog, Song new_song, unsigned int id)
{
    if (!new_song)
        return false; // Falhou

    if (!hashtable_insert(catalog->table, id, new_song))
        return false; // Falhou

    catalog->nr_songs++; // Incrementa o nº de músicas no catálogo

    return true; // Sucesso
}

int get_nr_songs(Songs catalog)
{
    return catalog->nr_songs;
}

Hashtable get_song_table(Songs catalog)
{
    return catalog->table;
}

void destroy_song_catalog(Songs catalog)
{
    if (!catalog)
        return;

    // Destroi-se a hashtable e liberta-se cada música
    hashtable_destroy(catalog->table, free_song);

    // Liberta-se o catálogo
    free(catalog);
}

void print_song_catalog(Songs catalog)
{
    printf("\nNúmero de músicas no catálogo: %d\n\nVisualização simplificada da tabela depois dos insert:\n\n", catalog->nr_songs);
    hashtable_print(catalog->table, print_song);
}

bool check_if_song_exists(Songs catalog, unsigned int id)
{
    if (hashtable_lookup(catalog->table, id))
        return true;
    else
        return false;
}

void reset_all_song_likes(Songs songs)
{
    hashtable_iterate(get_song_table(songs), reset_song_likes, NULL);
}

Song get_song(unsigned int id, Songs catalog)
{
    Song song = hashtable_lookup(catalog->table, id);
    Song cpy_song = dup_song(song);
    return cpy_song;
}

void iterate_songs_stat2(Songs songs, Artists artists)
{
    hashtable_iterate(songs->table, calculate_duration_and_acc, artists);
}

void accumulate_user_likes(void *user_ptr, void *context1, void *context2, void *context3)
{
    User user = (User)user_ptr;
    Songs songs = (Songs)context1;
    int min_age = *(int *)context2;
    int max_age = *(int *)context3;

    int user_age = get_age(user);
    // printf("User Age: %d\n", user_age);  // Debug: Print each user's age

    if (user_age >= min_age && user_age <= max_age)
    {
        // printf("User age %d\n", user_age);  // Debug confirmation
        LInt list = get_liked_songs(user);
        LInt liked_songs = list;
        while (liked_songs != NULL)
        {
            Song song = (Song)hashtable_lookup(songs->table, lint_get_value(liked_songs));
            increment_song_likes(song); // Incrementar contador de likes na estrutura Song
            liked_songs = lint_get_next(liked_songs);
        }
        free_lint(list);
    }
    else
    {
        // printf("User age %d is out of range (%d - %d)\n", user_age, min_age, max_age);  // Debug confirmation
    }
}

void iterate_songs_stat3(Songs songs, Stat_3 stat)
{
    hashtable_iterate(songs->table, accumulate_genre_likes, stat);
}