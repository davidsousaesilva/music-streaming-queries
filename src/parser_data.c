#include "parser_data.h"
#include "artist.h"
#include "song.h"
#include "user.h"
#include "album.h"
#include "history.h"
#define MAX_PATH_SIZE 1024

int invalid_artists_count = 0;

//===========|Auxiliares|============

// relocated to utils

//===========|ARTISTS|============
// parse_artist relocated to artist.c

static void parse_artists(char *path, Artists artists, FILE *fp, char *sep)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_size;
    int i = 0;
    FILE *error_file = fopen("resultados/artists_errors.csv", "w");
    if (error_file == NULL)
    {
        printf("Error opening artists_errors.csv for writing.\n");
        return;
    }

    fprintf(error_file, "id;name;description;recipe_per_stream;id_constituent;country;type\n");
    // Skip CSV header
    line_size = getline(&buffer, &bufsize, fp);

    // int i = 0;
    //  Contador para limitar intake
    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        // printf("Processing line: %s\n", buffer);
        //  parse_artist(buffer, artists,);
        parse_artist(buffer, artists, error_file, sep);
        i++;
    }

    free(buffer);
    fclose(error_file);
}

//===========|SONGS|============
// parse_song relocated to song.c

static void parse_songs(char *path, Songs catalog, Artists artists, Albums albums, FILE *fp, char *sep)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_size;

    FILE *error_file = fopen("resultados/musics_errors.csv", "w");
    if (error_file == NULL)
    {
        printf("Error opening musics_errors.csv for writing.\n");
        return;
    }
    fprintf(error_file, "id;title;artist_id;duration;genre;year;lyrics\n");

    line_size = getline(&buffer, &bufsize, fp);
    int i = 0;
    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        i++;
        parse_song(buffer, catalog, artists, albums, error_file, sep);
    }

    free(buffer);
    fclose(error_file);
}

//===========|USERS|============
// parse_user relocated to user.c

static void parse_users(char *path, Users users, Songs catalog, FILE *fp, char *sep)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_size;
    int i = 0;
    FILE *error_file = fopen("resultados/users_errors.csv", "w");
    if (error_file == NULL)
    {
        printf("Error opening users_errors.csv for writing.\n");
        return;
    }
    fprintf(error_file, "username;email;first_name;last_name;birth_date;country;subscription_type;liked_musics_id\n");

    // Skip CSV header
    line_size = getline(&buffer, &bufsize, fp);

    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        parse_user(buffer, users, catalog, error_file, sep);
        i++;
    }
    calculate_users_age(users);

    free(buffer);
    fclose(error_file);
}

//==========|ALBUMS|============
static void parse_albums(char *path, Albums albums, FILE *fp, char *sep, Artists artists)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_size;
    int i = 0;
    FILE *error_file = fopen("resultados/albums_errors.csv", "w");
    if (error_file == NULL)
    {
        printf("Error opening albums_errors.csv for writing.\n");
        return;
    }
    fprintf(error_file, "id;title;artist_id;year;producer\n");

    // Skip CSV header
    line_size = getline(&buffer, &bufsize, fp);

    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        parse_album(buffer, albums, error_file, sep, artists);
        i++;
    }

    free(buffer);
    fclose(error_file);
}

//=====================|History|=====================
static void parse_histories(char *path, Histories histories, FILE *fp, char *sep, Songs songs, Artists artists, Stat_4 stat_4, Stat_5 stat_5)
{
    char *buffer = NULL;
    size_t bufsize = 0;
    ssize_t line_size;
    int i = 0;
    FILE *error_file = fopen("resultados/history_errors.csv", "w");
    if (error_file == NULL)
    {
        printf("Error opening history_errors.csv for writing.\n");
        return;
    }
    fprintf(error_file, "id;user_id;timestamp;duration;platform\n");

    // Skip CSV header
    line_size = getline(&buffer, &bufsize, fp);

    while ((line_size = getline(&buffer, &bufsize, fp)) >= 0)
    {
        parse_history(buffer, histories, error_file, sep, songs, artists, stat_4, stat_5);
        i++;
    }

    free(buffer);
    fclose(error_file);
}

//==Parse Data==
int parse_data(char *path, Artists artists, Songs songs, Users users, Albums albums, Histories histories, Stat_4 stat_4, Stat_5 stat_5, char *sep)
{
    // Parsing Artists
    char *artists_path = (char *)malloc(MAX_PATH_SIZE * sizeof(char));
    if (artists_path == NULL)
    {
        fprintf(stderr, "Memory allocation failed for artists_path\n");
        return 1;
    }

    snprintf(artists_path, MAX_PATH_SIZE, "%s/%s", path, "artists.csv");
    FILE *fp = fopen(artists_path, "r");
    if (fp == NULL)
    {
        printf("Could not open file %s\n", artists_path);
        return 1;
    }
    else
    {
        printf("Artists open file %s\n", artists_path);
        parse_artists(artists_path, artists, fp, sep);
        fclose(fp);
    }
    free(artists_path);

    // Parsing Albums
    char *albums_path = (char *)malloc(MAX_PATH_SIZE * sizeof(char));
    if (albums_path == NULL)
    {
        fprintf(stderr, "Memory allocation failed for albums_path\n");
        return 1;
    }

    snprintf(albums_path, MAX_PATH_SIZE, "%s/%s", path, "albums.csv");
    FILE *ap = fopen(albums_path, "r");
    if (ap == NULL)
    {
        printf("Could not open file %s\n", albums_path);
        return 1;
    }
    else
    {
        printf("Albums open file %s\n", albums_path);
        parse_albums(albums_path, albums, ap, sep, artists);
        fclose(ap);
    }
    free(albums_path);

    // Parsing Songs
    char *songs_path = (char *)malloc(MAX_PATH_SIZE * sizeof(char));
    if (songs_path == NULL)
    {
        fprintf(stderr, "Memory allocation failed for songs_path\n");
        return 1;
    }

    snprintf(songs_path, MAX_PATH_SIZE, "%s/%s", path, "musics.csv");
    FILE *sp = fopen(songs_path, "r");
    if (sp == NULL)
    {
        printf("Could not open file %s\n", songs_path);
        return 1;
    }
    else
    {
        printf("Songs open file %s\n", songs_path);
        parse_songs(songs_path, songs, artists, albums, sp, sep);
        fclose(sp);
    }
    free(songs_path);

    // Parsing Users
    char *users_path = (char *)malloc(MAX_PATH_SIZE * sizeof(char));
    if (users_path == NULL)
    {
        fprintf(stderr, "Memory allocation failed for users_path\n");
    }

    snprintf(users_path, MAX_PATH_SIZE, "%s/%s", path, "users.csv");
    FILE *up = fopen(users_path, "r");
    if (up == NULL)
    {
        printf("Could not open file %s\n", users_path);
        return 1;
    }
    else
    {
        printf("Users open file %s\n", users_path);
        parse_users(users_path, users, songs, up, sep);
        fclose(up);
    }
    free(users_path);

    // Parsing Histories
    char *histories_path = (char *)malloc(MAX_PATH_SIZE * sizeof(char));
    if (histories_path == NULL)
    {
        fprintf(stderr, "Memory allocation failed for histories_path\n");
        return 1;
    }

    snprintf(histories_path, MAX_PATH_SIZE, "%s/%s", path, "history.csv");
    FILE *hp = fopen(histories_path, "r");
    if (hp == NULL)
    {
        printf("Could not open file %s\n", histories_path);
        return 1;
    }
    else
    {
        printf("Histories open file %s\n", histories_path);
        parse_histories(histories_path, histories, hp, sep, songs, artists, stat_4, stat_5);
        fclose(hp);
    }
    free(histories_path);

    return 0;
}
