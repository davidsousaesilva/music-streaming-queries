#include "history_manager.h"

struct histories
{
    Hashtable table;
    int nr_histories;
};

Histories init_history_catalog(int num_buckets)
{
    Histories catalog = malloc(sizeof(struct histories));
    if (!catalog)
    {
        return NULL;
    }

    catalog->nr_histories = 0;

    catalog->table = hashtable_init(num_buckets);
    if (!catalog->table)
    {
        free(catalog);
        return NULL;
    }

    return catalog;
}

bool insert_history(Histories catalog, History new_history, unsigned int id)
{
    if (!new_history)
    {
        return false;
    }

    if (!hashtable_insert(catalog->table, id, new_history))
    {
        return false;
    }

    catalog->nr_histories++;

    return true;
}

int get_nr_histories(Histories catalog)
{
    return catalog->nr_histories;
}

void destroy_history_catalog(Histories catalog)
{
    if (catalog)
    {
        hashtable_destroy(catalog->table, free_history);
        free(catalog);
    }
}

// Função para imprimir o catálogo de artistas com funcao print_artist como argumento
void print_history_catalog(Histories catalog)
{
    printf("\nNúmero de historicos no catálogo: %d\n\nVisualização simplificada da tabela depois dos insert:\n\n", catalog->nr_histories);
    hashtable_print(catalog->table, print_history); // Chama a função da hashtable
}

void iterate_histories_stat_6(Stat_6 stat, Histories histories, Songs songs)
{
    hashtable_iterate2(histories->table, store_values_query_6, stat, songs);
}