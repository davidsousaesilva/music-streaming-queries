#include "user_manager.h"

struct users
{
    Hashtable table;
    int nr_users;
};

Users init_user_catalog(int num_buckets)
{
    // Alocar memória para o catálogo de utilizadores
    Users catalog = malloc(sizeof(struct users));

    if (!catalog)
        return NULL; // Falha na alocação

    catalog->nr_users = 0;

    // Função hashtable_init para iniciar a tabela hash
    catalog->table = hashtable_init(num_buckets);
    if (!catalog->table)
    {
        free(catalog); // Liberta a memória alocada para o catálogo
        return NULL;
    }
    return catalog;
}

bool insert_user(Users catalog, User new_user, unsigned int username)
{
    if (!new_user)
        return false; // Falhou

    if (!hashtable_insert(catalog->table, username, new_user))
        return false; // Falhou

    catalog->nr_users++; // Incrementa o nº de utilizadores no catálogo

    return true; // Sucesso
}

User get_user(Users catalog, unsigned int username)
{
    User ori_user = (User)hashtable_lookup(catalog->table, username);
    User user = NULL;

    if (ori_user != NULL)
        user = dup_user(ori_user);

    return user;
}

int get_nr_users(Users catalog)
{
    return catalog->nr_users;
}

void destroy_user_catalog(Users catalog)
{
    if (!catalog)
        return;

    // Destroi-se a hashtable e liberta-se cada música
    hashtable_destroy(catalog->table, free_user);

    // Liberta-se o catálogo
    free(catalog);
}

void print_user_catalog(Users catalog)
{
    printf("\nNúmero de utilizadores no catálogo: %d\n\nVisualização simplificada da tabela depois dos insert:\n\n", catalog->nr_users);
    hashtable_print(catalog->table, print_user);
}

bool check_if_user_exists(Users catalog, unsigned int id)
{

    if (hashtable_lookup(catalog->table, id))
        return true;
    else
        return false;
}

Hashtable get_user_table(Users users)
{
    return users->table;
}

void calculate_users_age(Users users)
{
    hashtable_iterate(get_user_table(users), update_age_for_user, NULL);
}

void iterate_users_stat3(Users users, Songs songs, int min_age, int max_age)
{
    hashtable_iterate3(users->table, accumulate_user_likes, songs, &min_age, &max_age);
}