#include "song.h"
#include "lint.h"
#include "utils.h"
#include "album.h"
#include "album_manager.h"

struct song
{
    unsigned int id;
    char *name;
    LInt artist_id; // lista de id's de artistas
    int album_id;
    Time duration;
    char *genre;
    unsigned short year;
    char *lyrics;
    int likes; // Query3
};

Song create_song(unsigned int id, const char *name, LInt artist_id, int album_id,
                 Time duration, const char *genre, unsigned short year,
                 const char *lyrics)
{
    // Alocar memória para a nova música
    Song new_song = malloc(sizeof(struct song));
    if (!new_song)
        return NULL; // Falha na alocação

    // Preencher os dados da nova música
    new_song->id = id;
    new_song->name = strdup(name); // Copia o nome
    new_song->artist_id = artist_id;
    new_song->album_id = album_id;
    new_song->duration = duration;
    new_song->genre = strdup(genre); // Copia o gênero
    new_song->year = year;
    new_song->lyrics = strdup(lyrics); // Copia as lyrics
    new_song->likes = -1;              // nao calculado

    return new_song;
}

Song dup_song(Song ori_song)
{
    Song cpy_song = malloc(sizeof(struct song));
    if (!cpy_song)
        return NULL;

    cpy_song->id = ori_song->id;
    cpy_song->name = strdup(ori_song->name);
    cpy_song->artist_id = lint_dup_list(ori_song->artist_id);
    cpy_song->album_id = ori_song->album_id;
    cpy_song->duration = dup_time(ori_song->duration);
    cpy_song->genre = strdup(ori_song->genre);
    cpy_song->year = ori_song->year;
    cpy_song->lyrics = strdup(ori_song->lyrics);
    cpy_song->likes = ori_song->likes;

    return cpy_song;
}

void free_song(void *song_ptr)
{
    Song song = (Song)song_ptr;
    if (song)
    {
        free(song->name);
        free_lint(song->artist_id);
        free_time(song->duration);
        free(song->genre);
        free(song->lyrics);
        free(song);
    }
}

void print_song(void *song_ptr)
{
    Song song = (Song)song_ptr;
    if (song)
    {
        {
            printf(" [Chave %d, Nome: %s, Album ID: %d,",
                   song->id, song->name, song->album_id);
            print_time(song->duration);
            printf(", Lista: [");
            print_lint(song->artist_id);
            printf("] ->");
        }
    }
}

void calculate_duration_and_acc(void *song_ptr, void *ptr)
{
    Song song = (Song)song_ptr;
    Artists artists = (Artists)ptr;

    unsigned int duration_sec = calculate_duration(song->duration);

    LInt current = song->artist_id;
    unsigned int id;
    while (current)
    {

        id = lint_get_value(current);
        acc_artists_discography(artists, id, duration_sec);
        current = lint_get_next(current);
    }
}

void increment_song_likes(Song song)
{
    song->likes++;
}

unsigned int get_song_id(Song song)
{
    return song->id;
}

char *get_genre(Song song)
{
    return strdup(song->genre);
}

int get_song_likes(Song song)
{
    return song->likes;
}

void reset_song_likes(void *song_ptr, void *context)
{
    Song song = (Song)song_ptr;
    song->likes = 0; // Ajusta os likes da música para zero
}

LInt get_author(Song song)
{
    LInt author = lint_dup_list(song->artist_id);
    return author;
}

unsigned int get_album_id(Song song)
{
    return song->album_id;
}

void parse_song(char *buffer, Songs catalog, Artists artists, Albums albums, FILE *error_file, char *sep)
{
    // Parse CSV line
    // printf("Parsing new song entry: %s\n", buffer);
    char *copyline = strdup(buffer);
    char *saveptr;

    char *id_str = strtok_r(buffer, sep, &saveptr);
    char *name = strtok_r(NULL, sep, &saveptr);
    char *artist_id_str = strtok_r(NULL, sep, &saveptr);
    char *album_id_str = strtok_r(NULL, sep, &saveptr);
    char *duration_str = strtok_r(NULL, sep, &saveptr);
    char *genre = strtok_r(NULL, sep, &saveptr);
    char *year_str = strtok_r(NULL, sep, &saveptr);
    char *lyrics = strtok_r(NULL, sep, &saveptr);
    lyrics[0] = '\0';

    bool valid = true;

    char *clean_album_id = remove_quotes(album_id_str);

    unsigned int album_id = 0;

    if (album_id_str[0] == '[')
        valid = false;
    else
    {
        album_id = strtoul(clean_album_id + 2, NULL, 10);
        // Validate Album exists
        if (check_if_album_exists(albums, album_id) == false)
        {
            valid = false;
        }
    }

    unsigned int id = strtoul(id_str + 2, NULL, 10);
    int year = atoi(remove_quotes(year_str));
    // printf("year:  %d\n", year);

    // Validate year
    if (year > 2025)
    {
        valid = false;
    }

    LInt artist_ids = NULL;
    if (artist_id_str[1] != '[')
    {
        valid = false;
    }
    else
    {
        // Validate Liked Musics
        artist_ids = parse_id_list(artist_id_str);
    }
    // print_lint(artist_ids);

    // printf("Parsed artist IDs: ");
    LInt temp = artist_ids;
    while (temp != NULL)
    {
        unsigned int artist_id = lint_get_value(temp);
        // printf("%u ", artist_id);
        //  Check if the artist exists
        if (!check_if_artist_exists(artists, artist_id))
        {
            // printf("\nArtist ID %u does not exist in the artists.\n", artist_id);
            valid = false;
        }
        temp = lint_get_next(temp);
    }
    // printf("\n");

    // Validate Duration
    if (duration_str[0] == '"' && duration_str[strlen(duration_str) - 1] == '"')
    {
        duration_str++;
        duration_str[strlen(duration_str) - 1] = '\0';
    }
    Time duration = parse_time(duration_str);
    if (is_valid_duration(duration_str))
    {
        // Time duration = parse_time(duration_str);
        if (!duration)
        {
            //     // printf("Invalid duration format: %s\n", duration_str);
            valid = false;
        }
        // else
        // {
        //     // printf("Parsed duration: ");
        //     print_time(duration);
        // }
    }
    else
    {
        // printf("Duration format is invalid: %s\n", duration_str);
        valid = false;
    }

    // Insert song if valid, else log to error file
    if (valid)
    {
        // printf("Song is valid. Inserting song with ID: %u\n", id);
        Song song = create_song(id, remove_quotes(name), artist_ids, album_id, duration, remove_quotes(genre), year, lyrics);
        insert_song(catalog, song, id);
    }
    else
    {
        // printf("Song is invalid. Logging to error file.\n");
        fprintf(error_file, "%s", copyline);
        free_time(duration);
        free_lint(artist_ids);
    }

    free(copyline);
}

void accumulate_genre_likes(void *song_ptr, void *context)
{
    Song song = (Song)song_ptr;
    Stat_3 stat = (Stat_3)context;

    char *genre = strdup(song->genre);
    int likes = song->likes;

    // printf("Gênero: %s, Likes obtidos: %d\n", genre, likes);

    add_likes_to_genre(genre, likes, stat);

    free(genre);
}