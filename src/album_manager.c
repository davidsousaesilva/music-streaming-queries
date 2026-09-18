#include "album_manager.h"

struct albums
{
    Hashtable table;
    int nr_albums;
};

Albums init_album_catalog(int num_buckets)
{
    Albums catalog = malloc(sizeof(struct albums));
    if (!catalog)
    {
        return NULL;
    }

    catalog->nr_albums = 0;

    catalog->table = hashtable_init(num_buckets);
    if (!catalog->table)
    {
        free(catalog);
        return NULL;
    }

    return catalog;
}

bool insert_album(Albums catalog, Album new_album, unsigned int id)
{
    if (!new_album)
    {
        return false;
    }

    if (!hashtable_insert(catalog->table, id, new_album))
    {
        return false;
    }

    catalog->nr_albums++;

    return true;
}

int get_nr_albums(Albums catalog)
{
    return catalog->nr_albums;
}

Hashtable get_album_table(Albums catalog)
{
    return catalog->table;
}

void destroy_album_catalog(Albums catalog)
{
    if (catalog)
    {
        hashtable_destroy(catalog->table, free_album);
        free(catalog);
    }
}

void print_album_catalog(Albums catalog)
{
    if (catalog)
    {
        hashtable_print(catalog->table, print_album);
    }
}

bool check_if_album_exists(Albums catalog, unsigned int id)
{
    if (hashtable_lookup(catalog->table, id) != NULL)
        return true;
    else
        return false;
}