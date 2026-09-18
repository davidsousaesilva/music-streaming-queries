#include "album.h"
#include "lint.h"
#include "utils.h"
#include "album_manager.h"

struct album
{
    unsigned int id;
    char *title;
    LInt artist_id; // lista de id's de artistas
    unsigned short year;
    char *producer;
};

Album create_album(unsigned int id, const char *title, LInt artist_id, unsigned short year, const char *producer, Artists artists)
{

    Album new_album = malloc(sizeof(struct album));
    if (!new_album)
        return NULL;
    new_album->id = id;
    new_album->title = strdup(title);
    new_album->artist_id = artist_id;
    new_album->year = year;
    new_album->producer = strdup(producer);

    update_album_count(artist_id, artists);

    return new_album;
}

void free_album(void *album_ptr)
{
    Album album = (Album)album_ptr;
    if (album)
    {
        free(album->title);
        free_lint(album->artist_id);
        free(album->producer);
        free(album);
    }
}

void print_album(void *album_ptr)
{
    Album album = (Album)album_ptr;
    if (album)
    {
        printf(" [Chave %d, Título: %s,",
               album->id, album->title);
        printf(" Lista: [");
        print_lint(album->artist_id);
        printf("] ->");
    }
}

void parse_album(char *buffer, Albums albums, FILE *error_file, char *sep, Artists artists)
{
    char *copyline = strdup(buffer);
    char *saveptr;

    char *id_str = strtok_r(buffer, sep, &saveptr);
    char *title = strtok_r(NULL, sep, &saveptr);
    char *artist_id_str = strtok_r(NULL, sep, &saveptr);
    char *year_str = strtok_r(NULL, sep, &saveptr);
    char *producer = strtok_r(NULL, sep, &saveptr);
    unsigned int id = strtoul(id_str + 3, NULL, 10);
    LInt artist_id = parse_id_list(artist_id_str);
    unsigned short year = (unsigned short)strtoul(year_str + 1, NULL, 10);
    if (artist_id == NULL)
    {
        fprintf(error_file, "%s", copyline);
    }
    else
    {
        Album album = create_album(id, remove_quotes(title), artist_id, year, remove_quotes(producer), artists);
        insert_album(albums, album, id);
        // printf("Valid Album:\n");
        // printf("  ID: %u\n", id);
        // printf("  Title: %s\n", remove_quotes(title));
        // printf("  Artist IDs: ");
        // print_lint(artist_id); // Assuming print_id_list prints the linked list of IDs
        // printf("  Year: %u\n", year);
        // printf("  Producer: %s\n\n", remove_quotes(producer));
    }
    free(copyline);
}
