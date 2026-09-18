#include "artist_manager.h"

struct artists
{
    Hashtable table; // Um ponteiro para a tabela hash
    int nr_artists;  // Número de artistas
};

// Função para iniciar o catálogo de artistas
Artists init_artist_catalog(int num_buckets)
{
    // Alocar memória para o catálogo de artistas
    Artists catalog = malloc(sizeof(struct artists));
    if (!catalog)
    {
        return NULL; // Falhou
    }

    // Iniciar o número de artistas
    catalog->nr_artists = 0;

    // Usar a função hashtable_init para iniciar a tabela hash
    catalog->table = hashtable_init(num_buckets);
    if (!catalog->table)
    {
        free(catalog); // Liberta a memória alocada para o catálogo
        return NULL;   // Falhou
    }

    return catalog; // Retorna o ponteiro para o catálogo criado
}

// Função para inserir um artista no catálogo
bool insert_artist(Artists catalog, Artist new_artist, unsigned int id)
{
    if (!new_artist)
    {
        return false; // Falhou
    }

    // Insere o artista na hashtable usando a função genérica hashtable_insert
    if (!hashtable_insert(catalog->table, id, new_artist))
    {
        return false; // Falhou
    }

    // Incrementa o número de artistas no catálogo
    catalog->nr_artists++;

    return true; // Sucesso
}

int get_nr_artists(Artists catalog)
{
    return catalog->nr_artists;
}

Hashtable get_artist_table(Artists catalog)
{
    return catalog->table;
}

Artist get_artist(Artists catalog, unsigned int id)
{
    Artist artist = (Artist)hashtable_lookup(catalog->table, id);
    Artist cpy_artist = NULL;

    if (artist != NULL)
    {
        cpy_artist = dup_artist(artist);
    }
    return cpy_artist;
}

void destroy_artist_catalog(Artists catalog)
{
    if (!catalog)
        return;

    // Chama a função genérica para destruir a hashtable,
    // passando a função free_artist para libertar cada artista
    hashtable_destroy(catalog->table, free_artist);

    // Liberta o próprio catálogo
    free(catalog);
}

// Função para imprimir o catálogo de artistas com funcao print_artist como argumento
void print_artist_catalog(Artists catalog)
{
    printf("\nNúmero de artistas no catálogo: %d\n\nVisualização simplificada da tabela depois dos insert:\n\n", catalog->nr_artists);
    hashtable_print(catalog->table, print_artist); // Chama a função da hashtable
}

bool check_if_artist_exists(Artists catalog, unsigned int id)
{

    if (hashtable_lookup(catalog->table, id))
        return true;
    else
        return false;
}

void acc_artists_discography(Artists catalog, unsigned int id, unsigned int duration_sec)
{
    Artist artist = hashtable_lookup(catalog->table, id);

    if (artist != NULL)
        acc_artist_discography(artist, duration_sec);
}

void update_album_count(LInt artists, Artists catalog)
{
    LInt current = artists;

    while (current != NULL)
    {
        Artist artist = hashtable_lookup(catalog->table, lint_get_value(current));

        if (artist != NULL)
        {
            increment_artist_nr_albums(artist);
        }
        current = lint_get_next(current);
    }
}

void update_recipe(LInt artists, Artists catalog)
{
    LInt current = artists;
    while (current != NULL)
    {
        Artist artist = hashtable_lookup(catalog->table, lint_get_value(current));

        if (isIndividual(artist))
        {
            calculate_recipe(artist);
        }
        else
        {
            // Banda

            // printf("Banda encontrada:%s\n", get_artist_name(artist));

            // É preciso calcular a receita da banda
            calculate_recipe(artist);
            int member_count = 0;
            LInt list1 = get_members(artist);
            LInt members = list1;

            // Calculamos o nr de membros
            while (members != NULL)
            {
                member_count++;
                members = lint_get_next(members);
            }

            free_lint(list1);

            // Calculamos a parte que cada membro irá receber
            double cut = get_rps(artist) / member_count;
            // printf("%.2f\n", cut);

            // Adicionamos essa parte a cada membro
            LInt list2 = get_members(artist); // Obtém a lista de membros
            LInt update = list2;
            // Verifique se a lista de membros está vazia
            if (update == NULL)
            {
                printf("A banda não tem membros.\n");
            }
            while (update != NULL)
            {
                unsigned int member_id = lint_get_value(update);
                // printf("Procurar o membro com ID: %u\n", member_id);

                // Verifica se o ID do membro está correto (não 0)
                if (member_id == 0)
                {
                    printf("Erro: O ID do membro é 0, o que não é esperado!\n");
                }

                Artist member = hashtable_lookup(catalog->table, member_id);

                if (member != NULL)
                {
                    // printf("Membro encontrado!\n");
                    recipe_from_group(member, cut);
                }
                else
                {
                    printf("Membro não encontrado na tabela de hash.\n");
                }

                update = lint_get_next(update); // Avança para o próximo membro
            }
            free_lint(list2);
        }
        current = lint_get_next(current);
    }
}

void iterate_artists_stat2(Artists artists, Stat_2 stat)
{
    hashtable_iterate(artists->table, store_values_query2, stat);
}