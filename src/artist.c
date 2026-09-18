#include "artist.h"
#include "lint.h"
#include "utils.h"

struct artist
{
    unsigned int id;
    char *name;
    char *description;
    double recipe_p_s;
    LInt id_const; // lista de id's de artistas
    char *country;
    bool group;                         // false = individual | true = grupo
    unsigned int total_discography_sec; // query 2
    unsigned int nr_albums;
    double recipe_t;
};

Artist create_artist(unsigned int id, const char *name, const char *description,
                     double recipe_p_s, LInt id_const, const char *country, bool group)
{
    // Alocar memória para o novo artista
    Artist new_artist = malloc(sizeof(struct artist));
    if (!new_artist)
    {
        return NULL; // Falha na alocação
    }

    // Preencher os dados do novo artista
    new_artist->id = id;
    new_artist->name = strdup(name);               // Copia o nome
    new_artist->description = strdup(description); // Copia a descrição
    new_artist->recipe_p_s = recipe_p_s;
    new_artist->id_const = id_const;
    // print_lint(id_const);
    new_artist->country = strdup(country); // Copia o país
    new_artist->group = group;
    new_artist->total_discography_sec = 0;
    new_artist->nr_albums = 0;
    new_artist->recipe_t = 0.00;
    return new_artist; // Retorna o ponteiro para o novo artista
}

Artist dup_artist(Artist ori_artist)
{
    Artist cpy_artist = malloc(sizeof(struct artist));
    if (!cpy_artist)
        return NULL;

    cpy_artist->id = ori_artist->id;
    cpy_artist->name = strdup(ori_artist->name);
    cpy_artist->description = strdup(ori_artist->description);
    cpy_artist->recipe_p_s = ori_artist->recipe_p_s;
    cpy_artist->id_const = lint_dup_list(ori_artist->id_const);
    cpy_artist->country = strdup(ori_artist->country);
    cpy_artist->group = ori_artist->group;
    cpy_artist->total_discography_sec = ori_artist->total_discography_sec;
    cpy_artist->nr_albums = ori_artist->nr_albums;
    cpy_artist->recipe_t = ori_artist->recipe_t;

    return cpy_artist;
}

void free_artist(void *artist_ptr)
{
    // Converte o ponteiro genérico para Artist
    Artist artist = (Artist)artist_ptr;
    if (artist)
    {
        free(artist->name); // Liberta o nome
        free(artist->description);
        free_lint(artist->id_const); // Liberta lista de ids
        free(artist->country);
        free(artist); // Liberta a estrutura do artista
    }
}

// Função para imprimir as informações de um artista
void print_artist(void *artist_ptr)
{
    Artist artist = (Artist)artist_ptr; // Cast
    if (artist)
    {
        printf(" [Chave %d, Nome: %s, Discografia: %u, Lista: [",
               artist->id, artist->name, artist->total_discography_sec);
        print_lint(artist->id_const);
        printf("] ->");
    }
}

void store_values_query2(void *artist_ptr, void *stat_2_ptr)
{
    Artist artist = (Artist)artist_ptr;
    Stat_2 stat_2 = (Stat_2)stat_2_ptr;

    add_to_stat_2(artist->id, artist->name, artist->total_discography_sec, artist->country, artist->group, stat_2);
}

void acc_artist_discography(Artist artist, unsigned int duration_sec)
{
    if (artist)
        artist->total_discography_sec += duration_sec;
}

unsigned int get_artist_id(Artist artist)
{
    return artist->id;
}

char *get_artist_name(Artist artist)
{
    return strdup(artist->name);
}

unsigned int get_artist_total_discography(Artist artist)
{
    return artist->total_discography_sec;
}

char *get_artist_country(Artist artist)
{
    return strdup(artist->country);
}

char *get_type(Artist artist)
{
    char *resposta = "individual";
    if (artist->group == true)
    {
        resposta = "group";
    }
    return resposta;
}

LInt get_members(Artist artist)
{
    return lint_dup_list(artist->id_const);
}

bool isIndividual(Artist artist)
{
    bool resposta = false;
    if (artist->group == false)
    {
        resposta = true;
    }
    return resposta;
}

void increment_artist_nr_albums(Artist artist)
{
    artist->nr_albums++;
}

unsigned int get_artist_albums(Artist artist)
{
    return artist->nr_albums;
}

void recipe_from_group(Artist artist, double cut)
{
    artist->recipe_t += cut;
    // printf("%s: %f\n", artist->name, artist->recipe_t);
}

void calculate_recipe(Artist artist)
{
    artist->recipe_t += artist->recipe_p_s;
    // printf("%f\n", artist->recipe_p_s)
}

double get_rps(Artist artist)
{
    return artist->recipe_p_s;
}

double get_artist_recipe(Artist artist)
{
    return artist->recipe_t;
}

void parse_artist(char *buffer, Artists artists, FILE *error_file, char *sep)
{
    if (!buffer || !sep)
        return;

    // Duplicate the line
    char *copyline = strdup(buffer);
    if (!copyline)
    {
        fprintf(stderr, "Memory allocation failed for copyline\n");
        exit(EXIT_FAILURE);
    }

    char *saveptr;

    // Parse CSV line
    char *id_str = strtok_r(buffer, sep, &saveptr);
    char *name = strtok_r(NULL, sep, &saveptr);
    strtok_r(NULL, sep, &saveptr);  // Skip the description in CSV.
    char *description = strdup(""); // Default empty description.
    if (!description)
    {
        fprintf(stderr, "Memory allocation failed for description\n");
        free(copyline);
        exit(EXIT_FAILURE);
    }

    char *recipe_p_s_str = strtok_r(NULL, sep, &saveptr);
    char *id_const_str = strtok_r(NULL, sep, &saveptr);
    // printf("%s\n", id_const_str);
    char *country = strtok_r(NULL, sep, &saveptr);
    char *group_str = strtok_r(NULL, sep, &saveptr);

    // Check mandatory fields.
    if (!id_str || !name || !recipe_p_s_str || !id_const_str || !country || !group_str)
    {
        fprintf(error_file, "%s", copyline);
        free(copyline);
        free(description);
        return;
    }

    // Parse values.
    unsigned int id = strtoul(id_str + 2, NULL, 10); // Skip the leading `A"`.
    // printf("%s\n", recipe_p_s_str);
    double recipe_p_s = atof(recipe_p_s_str + 1);
    // printf("%f\n", recipe_p_s);
    bool is_group = false;
    bool valid = true;

    // Clean and validate `group_str`.
    clean_string(group_str);
    if (strcasecmp(group_str, "group") == 0)
    {
        is_group = true;
    }
    else if (strcasecmp(group_str, "individual") != 0)
    {
        valid = false;
    }

    // Handle constituents based on `is_group`.
    LInt id_constituents = NULL;
    if (is_group)
    {
        if (strcmp(id_const_str, "\"[]\"") == 0)
        {
            valid = false; // Group must have constituents.
        }
        else if (id_const_str[1] != '[')
        {
            valid = false; // Invalid format for constituents.
        }
        else
        {
            id_constituents = parse_id_list(id_const_str);
            if (!id_constituents)
                valid = false; // Parsing failed.
        }
    }
    else if (strcmp(id_const_str, "\"[]\"") != 0)
    {
        valid = false; // Individual must not have constituents.
    }
    // print_lint(id_constituents);

    // Insert artist if valid
    if (valid)
    {
        Artist artist = create_artist(
            id,
            remove_quotes(name),
            remove_quotes(description),
            recipe_p_s,
            id_constituents,
            remove_quotes(country),
            is_group);
        insert_artist(artists, artist, id);
    }
    else
    {
        fprintf(error_file, "%s", copyline);
        free_lint(id_constituents);
    }

    free(copyline);
    free(description);
}
