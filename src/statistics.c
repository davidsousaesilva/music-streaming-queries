#include "statistics.h"

////////////////////////////////////////////////////////////////////////
//////////               -->     query 2    <--               //////////

struct artist_discography
{
    int id;
    char *name;
    int total_discography_sec;
    char *country; // filter
    bool group;
};

struct stat_2
{
    Artist_Discography *array;
    unsigned int size;     // nº elementos ja inseridos
    unsigned int capacity; // igual ao nr_artists de artist_catalog
};

Stat_2 init_stat_2(Artists catalog)
{
    // Alocar memória para a estrutura stat_2
    Stat_2 stat = (Stat_2)malloc(sizeof(struct stat_2));
    if (stat == NULL)
    {
        return NULL;
    }

    unsigned int nr = get_nr_artists(catalog);

    // Definir o tamanho e alocar memória para o array de Artist_Discography
    stat->capacity = nr;
    stat->size = 0;
    stat->array = (Artist_Discography *)calloc(nr, sizeof(Artist_Discography)); // inicia todos os elementos com 0
    if (stat->array == NULL)
    {
        free(stat);
        return NULL;
    }

    return stat;
}

void destroy_stat_2(Stat_2 stat)
{
    if (stat != NULL)
    {
        if (stat->array != NULL)
        {
            for (int i = 0; i < stat->size; i++)
            {
                free(stat->array[i].country);
                free(stat->array[i].name);
            }

            free(stat->array);
        }
        free(stat);
    }
}

void print_stat_2(Stat_2 stat)
{
    if (stat == NULL)
    {
        printf("A estrutura stat_2 não foi iniciada.\n");
        return;
    }

    if (stat->array == NULL)
    {
        printf("O array de Artist_Discography está vazio.\n");
        return;
    }

    printf("\n\nStat_2 - Número de artistas - capacidade: %d\n", stat->capacity);
    printf("Stat_2 - Número de artistas - preenchidos: %d\n", stat->size);
    printf("ID\tdiscography\ttype\t%-8sName\t%-24sPais\n", "", "");
    printf("----------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < stat->capacity; i++)
    {
        printf("%d\t", stat->array[i].id);
        print_sec_to_time_format(stat->array[i].total_discography_sec);
        if (stat->array[i].group)
            printf("\tgroup");
        else
            printf("\tindiv");
        printf("\t\t%-25s", stat->array[i].name);
        printf("\t%-20s\n", stat->array[i].country);
    }
    printf("---------------------------------------------------------------------------------\n\n");
}

// Função para adicionar ou atualizar um artista
void add_to_stat_2(int id, char *name, int duration, char *country, bool group, Stat_2 stat)
{
    // Verificar se a estrutura está inicializada
    if (stat == NULL)
    {
        printf("A estrutura stat_2 não foi iniciada.\n");
        return;
    }

    // inserir novo artista se houver capacidade
    if (stat->size < stat->capacity)
    {
        // Insere o novo artista
        stat->array[stat->size].id = id;
        stat->array[stat->size].name = strdup(name);
        stat->array[stat->size].total_discography_sec = duration;
        stat->array[stat->size].country = strdup(country);
        stat->array[stat->size].group = group;
        stat->size++; // Incrementa o tamanho
    }
    else
    {
        printf("Capacidade máxima atingida. Não foi possível inserir o artista com ID %d.\n", id);
    }
}

// Função de comparação para qsort
static int compare_total_discography(const void *a, const void *b)
{
    // Converte os ponteiros para ponteiros de artist_discography
    const struct artist_discography *artistA = (const struct artist_discography *)a;
    const struct artist_discography *artistB = (const struct artist_discography *)b;

    int res = artistB->total_discography_sec - artistA->total_discography_sec;
    if (res == 0)
    {
        if (get_artist_id((Artist)artistA) < get_artist_id((Artist)artistB))
            return -1;
        else
            return 1;
    }
    return res;
}

// Função para ordenar o array por total_discography_sec
void sort_stat_2(Stat_2 stat)
{
    if (stat == NULL || stat->array == NULL || stat->size == 0)
    {
        printf("A estrutura stat_2 não foi iniciada ou está vazia.\n");
        return;
    }

    // Ordena o array usando qsort e a função de comparação
    qsort(stat->array, stat->size, sizeof(struct artist_discography), compare_total_discography);
}

void exec_stat_2(Stat_2 stat, Songs songs, Artists artists)
{
    // iterar a tabela de musicas, calcular a duracao e ir a cada artista de cada musica acumular a duraçao
    iterate_songs_stat2(songs, artists);

    //  iterar a tabela de artistas, e preencher o array stat com a informaçao
    iterate_artists_stat2(artists, stat);

    // ordenar o array
    sort_stat_2(stat);
}

char *query2_nofilter_result(Stat_2 stat, unsigned int topN, char *sep)
{
    if (topN > stat->capacity)
        topN = stat->capacity;

    if (topN == 0)
        return strdup("\n");

    char *result = calloc(1, sizeof(char)); // Inicia com ""
    size_t current_size = 1;                // Tamanho atual do buffer
    char *line = NULL;                      // Buffer temporário para cada linha

    for (int i = 0; i < topN; i++)
    {
        char *time_str = get_sec_to_time_format(stat->array[i].total_discography_sec);

        size_t line_length = snprintf(NULL, 0, "%s%s%s%s%s%s%s\n",
                                      stat->array[i].name, sep, stat->array[i].group ? "group" : "individual", sep, time_str, sep, stat->array[i].country);

        line = malloc(line_length + 1); // +1 para o '\0'
        if (line == NULL)
        {
            printf("Erro ao alocar memória!\n");
            free(result);
            return NULL;
        }

        snprintf(line, line_length + 1, "%s%s%s%s%s%s%s\n",
                 stat->array[i].name, sep, stat->array[i].group ? "group" : "individual", sep, time_str, sep, stat->array[i].country);

        free(time_str); // Liberta `time_str` após o uso

        size_t new_size = current_size + line_length;

        char *temp = realloc(result, new_size);
        if (temp == NULL)
        {
            printf("Erro ao alocar memória!\n");
            free(result);
            free(line);
            return NULL;
        }
        result = temp;

        strcat(result, line);
        current_size = new_size;

        free(line);
    }
    return result;
}

char *query2_filter_result(Stat_2 stat, unsigned int topN, char *country, char *sep)
{
    if (topN > stat->capacity)
        topN = stat->capacity;

    char *result = calloc(1, sizeof(char)); // Inicia com ""
    size_t current_size = 1;                // Tamanho atual do buffer
    char *line = NULL;
    unsigned int count = 0;

    for (int i = 0; i < stat->size && count < topN; i++)
    {
        // printf("Comparando artista com país: %s\n", stat->array[i].country);
        if (strcmp(stat->array[i].country, country) != 0)
        {
            continue;
        }

        count++;

        char *time_str = get_sec_to_time_format(stat->array[i].total_discography_sec);

        size_t line_length = snprintf(NULL, 0, "%s%s%s%s%s%s%s\n",
                                      stat->array[i].name, sep, stat->array[i].group ? "group" : "individual", sep, time_str, sep, stat->array[i].country);

        line = malloc(line_length + 1);
        if (line == NULL)
        {
            printf("Erro ao alocar memória!\n");
            free(result);
            return NULL;
        }

        snprintf(line, line_length + 1, "%s%s%s%s%s%s%s\n",
                 stat->array[i].name, sep, stat->array[i].group ? "group" : "individual", sep, time_str, sep, stat->array[i].country);

        free(time_str);

        size_t new_size = current_size + line_length;

        char *temp = realloc(result, new_size);
        if (temp == NULL)
        {
            printf("Erro ao alocar memória!\n");
            free(result);
            free(line);
            return NULL;
        }
        result = temp;

        strcat(result, line);
        current_size = new_size;

        free(line);
    }
    if (count == 0)
    {
        free(result);
        result = strdup("\n");
    }

    return result;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      query 3       ////////////////////////////////////////

struct genre_likes
{
    char *genre;
    unsigned int total_likes;
};

// Estrutura para armazenar a lista de popularidade de gêneros
struct stat_3
{
    Genre_Likes *array;
    unsigned int size;
    unsigned int capacity;
};

// Inicializa a estrutura Stat_3
Stat_3 init_stat_3(unsigned int capacity)
{

    Stat_3 stat = (Stat_3)malloc(sizeof(struct stat_3));
    if (stat == NULL)
        return NULL;

    stat->array = (Genre_Likes *)calloc(capacity, sizeof(Genre_Likes));
    if (stat->array == NULL)
    {
        free(stat);
        return NULL;
    }

    stat->size = 0;
    stat->capacity = capacity;
    return stat;
}

// Liberta a memória alocada para Stat_3
void destroy_stat_3(Stat_3 stat)
{
    if (stat)
    {
        for (unsigned int i = 0; i < stat->size; i++)
        {
            free(stat->array[i].genre);
        }
        free(stat->array);
        free(stat);
    }
}

// Adiciona ou atualiza o total de likes para um gênero específico
void add_likes_to_genre(char *genre, int likes, Stat_3 stat)
{
    if (!stat || !genre)
        return;

    // Check if genre already exists in the array
    for (unsigned int i = 0; i < stat->size; i++)
    {
        if (strcmp(stat->array[i].genre, genre) == 0)
        {
            stat->array[i].total_likes += likes;
            return;
        }
    }

    if (stat->size >= stat->capacity)
    {
        unsigned int new_capacity = stat->capacity * 2;
        Genre_Likes *new_array = realloc(stat->array, new_capacity * sizeof(Genre_Likes));
        if (new_array == NULL)
        {
            printf("Falha ao realocar memória da Stat_3\n");
            return;
        }
        stat->array = new_array;
        stat->capacity = new_capacity;
    }

    // Add new genre entry
    stat->array[stat->size].genre = strdup(genre);
    if (stat->array[stat->size].genre == NULL)
    {
        printf("Falha ao alocar memória para o gênero\n");
        return;
    }
    stat->array[stat->size].total_likes = likes;
    stat->size++;
}

void print_stat_3(Stat_3 stat)
{
    if (stat == NULL)
    {
        printf("A estrutura stat_3 não foi iniciada\n");
        return;
    }

    if (stat->array == NULL || stat->size == 0)
    {
        printf("O array de genre_likes está vazio\n");
        return;
    }

    printf("Estatísticas de popularidade dos gêneros:\n");
    for (unsigned int i = 0; i < stat->size; i++)
    {
        printf("Gênero: %s, Total likes: %u\n", stat->array[i].genre, stat->array[i].total_likes);
    }
}

// Função de comparação para ordenação por popularidade decrescente e ordem alfabética
static int compare_genres(const void *a, const void *b)
{
    const struct genre_likes *genreA = (const struct genre_likes *)a;
    const struct genre_likes *genreB = (const struct genre_likes *)b;

    if (genreB->total_likes != genreA->total_likes)
    {
        return genreB->total_likes - genreA->total_likes;
    }
    return strcmp(genreA->genre, genreB->genre);
}

// Função para ordenar os gêneros
void sort_genres(Stat_3 stat)
{
    qsort(stat->array, stat->size, sizeof(Genre_Likes), compare_genres);
}

// Função para restaurar as estátisticas do stat3
void reset_stat_3(Stat_3 stat)
{
    if (stat)
        for (unsigned int i = 0; i < stat->size; i++)
            stat->array[i].total_likes = 0;
}

void exec_stat_3(Stat_3 stat, Songs songs, Users users, int min_age, int max_age)
{
    /*Iterar a tabela de users, se estiver no age range, percorrer a LInt liked_m_id para incrementar
    o nr de likes na struct song    */
    iterate_users_stat3(users, songs, min_age, max_age);

    /*Iterar a tabela de songs e acumular os likes no genero respetivo*/
    iterate_songs_stat3(songs, stat);

    sort_genres(stat);
}

char *query3_result(Stat_3 stat, char *separador_q)
{
    if (stat == NULL || stat->array == NULL || stat->size == 0 || stat->array->total_likes == 0)
    {
        return strdup("\n"); // Retorna uma linha vazia
    }

    int buffer_size = stat->size * 50;
    char *result = (char *)malloc(buffer_size);
    if (result == NULL)
        return NULL;

    result[0] = '\0'; // Inicializa a string resultante como vazia

    for (unsigned int i = 0; i < stat->size; i++)
    {
        if (stat->array[i].genre == NULL)
            continue; // Verifica se `genre` é NULL para evitar problemas

        char line[50];
        snprintf(line, sizeof(line), "%s%s%u\n", stat->array[i].genre, separador_q, stat->array[i].total_likes);

        if (strlen(result) + strlen(line) >= buffer_size - 1)
        {
            buffer_size *= 2;
            result = realloc(result, buffer_size);
            if (result == NULL)
                return NULL;
        }
        strcat(result, line);
    }
    return result;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      query 4       ////////////////////////////////////////

struct cell
{
    int artist_id;
    int total;
};

struct stat_4
{
    Cell **matriz_total; // ignorar linha 0
    Cell **matriz_top10; // ignorar linha 0
    int res_nofilter;
    int res_nofilter_count; // Novo campo para contar a frequência
};

// Função para inicializar a estrutura Stat_4
Stat_4 init_stat_4()
{
    // Alocar memória para a estrutura principal
    Stat_4 stat = (Stat_4)malloc(sizeof(struct stat_4));
    if (!stat)
        return NULL;

    // Inicializar res_nofilter e res_nofilter_count
    stat->res_nofilter = -1;
    stat->res_nofilter_count = 0;

    // Alocar memória para matriz_total (367 x 10005)
    stat->matriz_total = (Cell **)malloc(367 * sizeof(Cell *));
    if (!stat->matriz_total)
    {
        free(stat);
        return NULL;
    }
    for (int i = 0; i < 367; i++)
    {
        stat->matriz_total[i] = (Cell *)malloc(10005 * sizeof(Cell));
        if (!stat->matriz_total[i])
        {
            for (int j = 0; j < i; j++)
                free(stat->matriz_total[j]);
            free(stat->matriz_total);
            free(stat);
            return NULL;
        }
        for (int j = 0; j < 10005; j++)
        {
            stat->matriz_total[i][j] = (Cell)malloc(sizeof(struct cell));
            if (!stat->matriz_total[i][j])
            {
                for (int k = 0; k < j; k++)
                    free(stat->matriz_total[i][k]);
                free(stat->matriz_total[i]);
                for (int l = 0; l < i; l++)
                {
                    for (int m = 0; m < 10005; m++)
                        free(stat->matriz_total[l][m]);
                    free(stat->matriz_total[l]);
                }
                free(stat->matriz_total);
                free(stat);
                return NULL;
            }
            stat->matriz_total[i][j]->artist_id = -1;
            stat->matriz_total[i][j]->total = 0;
        }
    }

    // Alocar memória para matriz_top10 (367 x 10)
    stat->matriz_top10 = (Cell **)malloc(367 * sizeof(Cell *));
    if (!stat->matriz_top10)
    {
        for (int i = 0; i < 367; i++)
        {
            for (int j = 0; j < 10005; j++)
                free(stat->matriz_total[i][j]);
            free(stat->matriz_total[i]);
        }
        free(stat->matriz_total);
        free(stat);
        return NULL;
    }
    for (int i = 0; i < 367; i++)
    {
        stat->matriz_top10[i] = (Cell *)malloc(10 * sizeof(Cell));
        if (!stat->matriz_top10[i])
        {
            for (int j = 0; j < i; j++)
                free(stat->matriz_top10[j]);
            for (int k = 0; k < 367; k++)
            {
                for (int l = 0; l < 10005; l++)
                    free(stat->matriz_total[k][l]);
                free(stat->matriz_total[k]);
            }
            free(stat->matriz_top10);
            free(stat->matriz_total);
            free(stat);
            return NULL;
        }
        for (int j = 0; j < 10; j++)
        {
            stat->matriz_top10[i][j] = (Cell)malloc(sizeof(struct cell));
            if (!stat->matriz_top10[i][j])
            {
                for (int k = 0; k < j; k++)
                    free(stat->matriz_top10[i][k]);
                free(stat->matriz_top10[i]);
                for (int l = 0; l < i; l++)
                {
                    for (int m = 0; m < 10; m++)
                        free(stat->matriz_top10[l][m]);
                    free(stat->matriz_top10[l]);
                }
                for (int n = 0; n < 367; n++)
                {
                    for (int o = 0; o < 10005; o++)
                        free(stat->matriz_total[n][o]);
                    free(stat->matriz_total[n]);
                }
                free(stat->matriz_top10);
                free(stat->matriz_total);
                free(stat);
                return NULL;
            }
            stat->matriz_top10[i][j]->artist_id = -1;
            stat->matriz_top10[i][j]->total = 0;
        }
    }

    return stat;
}

// Função para imprimir o conteúdo de Stat_4
void print_stat_4(Stat_4 stat)
{
    if (!stat)
    {
        printf("Stat_4 is NULL\n");
        return;
    }

    printf("\nres_nofilter: %d\n\n", stat->res_nofilter);
    printf("\nres_nofilter_count: %d\n\n", stat->res_nofilter_count);

    // printf("matriz_total:\n");
    // for (int i = 1; i < 367; i++)
    // {
    //     for (int j = 0; j < 10005; j++)
    //     {
    //         printf("[%d,%d] ", stat->matriz_total[i][j]->artist_id, stat->matriz_total[i][j]->total);
    //     }
    //     printf("\n");
    // }

    printf("\nmatriz_top10:\n\n");
    for (int i = 0; i < 367; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            printf("[%d,%d] ", stat->matriz_top10[i][j]->artist_id, stat->matriz_top10[i][j]->total);
        }
        printf("\n");
    }
}

// Função para Libertar a memória alocada para Stat_4
void destroy_stat_4(Stat_4 stat)
{
    if (!stat)
        return;

    for (int i = 0; i < 367; i++)
    {
        for (int j = 0; j < 10005; j++)
        {
            free(stat->matriz_total[i][j]);
        }
        free(stat->matriz_total[i]);
    }
    free(stat->matriz_total);

    for (int i = 0; i < 367; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            free(stat->matriz_top10[i][j]);
        }
        free(stat->matriz_top10[i]);
    }
    free(stat->matriz_top10);

    free(stat);
}

// Função otimizando a inserção de dados usando hashing
void insert_stat_4(Songs songs, Stat_4 stat, unsigned int song_id, char *date, char *duration)
{
    int w = calculate_week(date);            // date -> "yyyy/mm/dd"
    int seconds = time_to_seconds(duration); // duration -> "hh:mm:ss"

    Song song = get_song(song_id, songs);

    LInt artists = get_author(song);
    LInt current = artists;

    while (current != NULL) // para cada artista
    {
        unsigned int artist_id = lint_get_value(current);
        int index = artist_id; // Usar o hash (index direto)

        if (stat->matriz_total[w][index]->artist_id == -1)
        {
            // Inicializar nova entrada
            stat->matriz_total[w][index]->artist_id = artist_id;
            stat->matriz_total[w][index]->total = seconds;
        }
        else
        {
            // Atualizar total existente
            stat->matriz_total[w][index]->total += seconds;
        }

        current = lint_get_next(current);
    }

    free_lint(artists);
    free_song(song);
}

// Função para construir a matriz_top10 com base na matriz_total
void build_top10(Stat_4 stat)
{
    for (int w = 0; w < 367; w++)
    {
        // Array auxiliar para ordenação
        Cell temp[10005];
        int count = 0;

        // Copiar elementos da linha w da matriz_total para o array auxiliar
        for (int i = 0; i < 10005; i++)
        {
            if (stat->matriz_total[w][i]->artist_id != -1)
            {
                temp[count++] = stat->matriz_total[w][i];
            }
        }

        // Ordenar o array auxiliar com base em total (decrescente) e artist_id (crescente)
        for (int i = 0; i < count - 1; i++)
        {
            for (int j = i + 1; j < count; j++)
            {
                if (temp[i]->total < temp[j]->total ||
                    (temp[i]->total == temp[j]->total && temp[i]->artist_id > temp[j]->artist_id))
                {
                    Cell swap = temp[i];
                    temp[i] = temp[j];
                    temp[j] = swap;
                }
            }
        }

        // Preencher a matriz_top10 com os 10 primeiros elementos do array ordenado
        for (int i = 0; i < 10; i++)
        {
            if (i < count)
            {
                stat->matriz_top10[w][i]->artist_id = temp[i]->artist_id;
                stat->matriz_top10[w][i]->total = temp[i]->total;
            }
            else
            {
                stat->matriz_top10[w][i]->artist_id = -1;
                stat->matriz_top10[w][i]->total = 0;
            }
        }
    }
}

// Função para calcular o res_nofilter com base na matriz_top10
void calculate_res_nofilter(Stat_4 stat)
{
    // Array auxiliar para contar a frequência de cada artist_id
    int frequency[10005] = {0};
    int max_artist_id = -1;
    int max_count = 0;

    for (int w = 0; w < 367; w++)
    {
        for (int i = 0; i < 10; i++)
        {
            int artist_id = stat->matriz_top10[w][i]->artist_id;
            if (artist_id != -1)
            {
                frequency[artist_id] += 1;

                // Atualizar o artist_id mais frequente
                if (frequency[artist_id] > max_count ||
                    (frequency[artist_id] == max_count && artist_id < max_artist_id))
                {
                    max_artist_id = artist_id;
                    max_count = frequency[artist_id];
                }
            }
        }
    }

    // Armazenar o resultado no campo res_nofilter e res_nofilter_count
    stat->res_nofilter = max_artist_id;
    stat->res_nofilter_count = max_count;
}

void exec_stat_4(Stat_4 stat)
{
    build_top10(stat);
    calculate_res_nofilter(stat);
}

int get_res_no_filter_id(Stat_4 stat)
{
    return (stat->res_nofilter);
}

int get_res_no_filter_count(Stat_4 stat)
{
    return (stat->res_nofilter_count);
}

int get_res_filter_id(Stat_4 stat, char *date1, char *date2)
{
    if (!stat || !date1 || !date2)
        return -1;

    int week_start = calculate_week(date1);
    int week_end = calculate_week(date2);

    // Verificar se os intervalos são válidos
    if (week_start < 0)
        week_start = 0;
    if (week_end > 366)
        week_end = 366;

    // Array auxiliar para contar a frequência de cada artist_id
    int frequency[10000] = {0};
    int max_artist_id = -1;
    int max_count = 0;

    for (int w = week_start; w <= week_end; w++)
    {
        for (int i = 0; i < 10; i++)
        {
            int artist_id = stat->matriz_top10[w][i]->artist_id;
            if (artist_id != -1)
            {
                frequency[artist_id]++;

                // Atualizar o artist_id mais frequente
                if (frequency[artist_id] > max_count ||
                    (frequency[artist_id] == max_count && artist_id < max_artist_id))
                {
                    max_artist_id = artist_id;
                    max_count = frequency[artist_id];
                }
            }
        }
    }

    return max_artist_id;
}

int get_res_filter_count(Stat_4 stat, char *date1, char *date2)
{
    if (!stat || !date1 || !date2)
        return -1;

    int week_start = calculate_week(date1);
    int week_end = calculate_week(date2);

    // Verificar se os intervalos são válidos
    if (week_start < 0)
        week_start = 0;
    if (week_end > 366)
        week_end = 366;

    // Array auxiliar para contar a frequência de cada artist_id
    int frequency[10000] = {0};
    int max_count = 0;

    for (int w = week_start; w <= week_end; w++)
    {
        for (int i = 0; i < 10; i++)
        {
            int artist_id = stat->matriz_top10[w][i]->artist_id;
            if (artist_id != -1)
            {
                frequency[artist_id]++;

                // Atualizar a frequência máxima
                if (frequency[artist_id] > max_count)
                {
                    max_count = frequency[artist_id];
                }
            }
        }
    }

    return max_count;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      query 5       ////////////////////////////////////////

struct stat_5
{
    int **matriz;
    char **idUsers;     // array strings 400005 elementos
    char **namesGenres; // array strings 10 elementos
};

#define NUM_USERS 400005

#define METAL 0
#define REGGAE 1
#define JAZZ 2
#define POP 3
#define CLASSICAL 4
#define ELECTRONIC 5
#define COUNTRY 6
#define BLUES 7
#define HIP_HOP 8
#define ROCK 9

const char *GENRES[] = {"Metal", "Reggae", "Jazz", "Pop", "Classical", "Electronic", "Country", "Blues", "Hip Hop", "Rock"};

Stat_5 init_stat_5()
{
    // Aloca a estrutura
    Stat_5 stat = (Stat_5)malloc(sizeof(struct stat_5));

    // Aloca a matriz
    stat->matriz = (int **)malloc(NUM_USERS * sizeof(int *));
    for (int i = 0; i < NUM_USERS; i++)
    {
        stat->matriz[i] = (int *)malloc(10 * sizeof(int));
        // Inicializa os valores da matriz com 0
        for (int j = 0; j < 10; j++)
        {
            stat->matriz[i][j] = 0;
        }
    }

    // Aloca idUsers e inicializa os valores com o formato "A%d07i"
    stat->idUsers = (char **)malloc(NUM_USERS * sizeof(char *));
    for (int i = 0; i < NUM_USERS; i++)
    {
        stat->idUsers[i] = (char *)malloc(8 * sizeof(char));
        sprintf(stat->idUsers[i], "%07d", i+1);
    }

    // Aloca e inicializa namesGenres com os valores de GENRES
    stat->namesGenres = (char **)malloc(10 * sizeof(char *));
    for (int j = 0; j < 10; j++)
    {
        stat->namesGenres[j] = (char *)malloc((strlen(GENRES[j]) + 1) * sizeof(char)); // Aloca espaço para o nome do gênero
        strcpy(stat->namesGenres[j], GENRES[j]);                                       // Copia o nome do gênero para a estrutura
    }

    return stat;
}

char **stat_5_get_users(Stat_5 stat)
{
    // Aloca um novo array para as strings de idUsers
    char **copy = (char **)malloc(NUM_USERS * sizeof(char *));
    for (int i = 0; i < NUM_USERS; i++)
    {
        copy[i] = (char *)malloc(8 * sizeof(char)); // Aloca espaço para a string "A%07d"
        strcpy(copy[i], stat->idUsers[i]);          // Copia a string de idUsers para a nova memória
    }
    return copy;
}

void free_stat_5_users(char **users)
{
    if (users)
    {
        for (int i = 0; i < NUM_USERS; i++)
        {
            free(users[i]); // Liberta cada string individualmente
        }
        free(users); // Liberta o array de ponteiros
    }
}

char **stat_5_get_genres(Stat_5 stat)
{
    // Aloca um novo array para as strings de genres
    char **copy = (char **)malloc(10 * sizeof(char *));
    for (int i = 0; i < 10; i++)
    {
        copy[i] = (char *)malloc((strlen(stat->namesGenres[i]) + 1) * sizeof(char)); // Aloca memória para o nome do gênero
        strcpy(copy[i], stat->namesGenres[i]);                                       // Copia o nome do gênero
    }
    return copy;
}

void free_stat_5_genres(char **genres)
{
    if (genres)
    {
        for (int i = 0; i < 10; i++)
        {
            free(genres[i]); // Liberta cada string de gênero individualmente
        }
        free(genres); // Liberta o array de ponteiros
    }
}

void free_stat_5_matriz(int **matriz)
{
    if (matriz)
    {
        for (int i = 0; i < NUM_USERS; i++)
        {
            free(matriz[i]); // Liberta cada linha da matriz
        }
        free(matriz); // Liberta o array de ponteiros para as linhas
    }
}

int **stat_5_get_matriz(Stat_5 stat)
{
    // Aloca uma nova matriz
    int **copy = (int **)malloc(NUM_USERS * sizeof(int *));
    for (int i = 0; i < NUM_USERS; i++)
    {
        copy[i] = (int *)malloc(10 * sizeof(int));          // Aloca memória para cada linha da matriz
        memcpy(copy[i], stat->matriz[i], 10 * sizeof(int)); // Copia os dados da matriz original para a nova matriz
    }
    return copy;
}

void destroy_stat_5(Stat_5 stat)
{
    if (stat)
    {
        // Liberta memória da matriz
        for (int i = 0; i < NUM_USERS; i++)
        {
            free(stat->matriz[i]);
        }
        free(stat->matriz);

        // Liberta memória de idUsers
        for (int i = 0; i < NUM_USERS; i++)
        {
            free(stat->idUsers[i]);
        }
        free(stat->idUsers);

        // Liberta memória de namesGenres
        for (int j = 0; j < 10; j++)
        {
            free(stat->namesGenres[j]);
        }
        free(stat->namesGenres);

        // Liberta a estrutura em si
        free(stat);
    }
}

void print_stat_5(Stat_5 stat, int max_lines)
{
    printf("\nStat_5 Report:\n\n");
    printf("%-10s", "User");
    for (int j = 0; j < 10; j++)
    {
        printf("%-12s", GENRES[j]);
    }
    printf("\n");

    int lines_to_print = max_lines < NUM_USERS ? max_lines : NUM_USERS;

    for (int i = 0; i < lines_to_print; i++)
    {
        printf("User %-6d", i + 1);
        for (int j = 0; j < 10; j++)
        {
            printf("%-12d", stat->matriz[i][j]);
        }
        printf("\n");
    }

    if (lines_to_print < NUM_USERS)
    {
        printf("(...) (Only showing %d of %d users)\n", lines_to_print, NUM_USERS);
    }

    printf("\n\nIDS\n\n");
    for (int i = 0; i < lines_to_print && i < NUM_USERS; i++)
    {
        printf("%s\n", stat->idUsers[i]);
    }

    printf("\n\nGENRES\n\n");
    for (int i = 0; i < lines_to_print && i < 10; i++)
    {
        printf("%s\n", stat->namesGenres[i]);
    }
}

void update_stat_5(Stat_5 stat, int user_id, const char *genre)
{
    if (user_id < 0 || user_id >= NUM_USERS)
    {
        fprintf(stderr, "Erro: user_id %d fora do intervalo permitido (0-%d).\n", user_id, NUM_USERS - 1);
        return;
    }

    for (int j = 0; j < 10; j++)
    {
        if (strcmp(genre, GENRES[j]) == 0)
        {
            stat->matriz[user_id - 1][j]++;
            return;
        }
    }

    fprintf(stderr, "Erro: Género '%s' não encontrado.\n", genre);
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////      query 6       ////////////////////////////////////////

struct genre_cell
{
    char *genre;   // genero
    int total_sec; // quantidade de segundos ouvidos desse genero
};

struct artist_cell
{
    int artist_id;
    int nr_songs_listen; // nr musicas ouvidas desse artista
    int total_sec;       // tempo total ouvido desse artista
};

struct user_cell
{
    int total_sec_listen;
    int *what_songs_listen; // array 1050000 unidades
    int songs_listen;
    Artist_Cell *artists_data; // array 10005 unidades
    int *songs_p_day_data;     // array 366 unidades
    Genre_Cell *genres_data;   // array 10 unidades
    int *albums_data;          // array 100005 unidades
    int *hour_data;            // array de 24 unidades
    int **matriz_what_songs;   // matriz NUM_ARTISTS x NUM_SONGS
};

struct stat_6
{
    User_Cell data; // informacao para ('user_id' && 'year')
    int user_id;
    int year;
    int n;
};

// Define os nomes dos gêneros
// const char *GENRES[] = {"Metal", "Reggae", "Jazz", "Pop", "Classical", "Electronic", "Country", "Blues", "Hip Hop", "Rock"};
#define NUM_GENRES 10
#define NUM_HOURS 24
#define NUM_DAYS 367 // pq se ignora dia 0
#define NUM_ARTISTS 10005
#define NUM_ALBUMS 100005
#define MAX_SONGS 1050000

Stat_6 init_stat_6()
{
    // Aloca a estrutura principal
    Stat_6 stat = (Stat_6)malloc(sizeof(struct stat_6));

    // Inicializa campos de Stat_6
    stat->user_id = -1;
    stat->year = -1;
    stat->n = -1;

    // Aloca a estrutura User_Cell
    stat->data = (User_Cell)malloc(sizeof(struct user_cell));

    User_Cell user = stat->data;

    // Inicializa campos de User_Cell
    user->total_sec_listen = 0;
    user->songs_listen = 0;

    // Aloca e inicializa what_songs_listen
    user->what_songs_listen = (int *)calloc(MAX_SONGS, sizeof(int));

    // Aloca e inicializa songs_p_day_data
    user->songs_p_day_data = (int *)calloc(NUM_DAYS, sizeof(int));

    // Aloca e inicializa artists_data
    user->artists_data = (Artist_Cell *)malloc(NUM_ARTISTS * sizeof(Artist_Cell));

    for (int i = 0; i < NUM_ARTISTS; i++)
    {
        user->artists_data[i] = (Artist_Cell)malloc(sizeof(struct artist_cell));
        user->artists_data[i]->artist_id = -1;
        user->artists_data[i]->nr_songs_listen = 0;
        user->artists_data[i]->total_sec = 0;
    }

    // Aloca e inicializa genres_data
    user->genres_data = (Genre_Cell *)malloc(NUM_GENRES * sizeof(Genre_Cell));

    for (int i = 0; i < NUM_GENRES; i++)
    {
        user->genres_data[i] = (Genre_Cell)malloc(sizeof(struct genre_cell));
        user->genres_data[i]->genre = strdup(GENRES[i]);
        user->genres_data[i]->total_sec = 0;
    }

    // Aloca e inicializa albums_data
    user->albums_data = (int *)calloc(NUM_ALBUMS, sizeof(int));

    // Aloca e inicializa hour_data
    user->hour_data = (int *)calloc(NUM_HOURS, sizeof(int));

    // Aloca e inicializa matriz_what_songs
    user->matriz_what_songs = (int **)calloc(NUM_ARTISTS, sizeof(int *));
    for (int i = 0; i < NUM_ARTISTS; i++)
    {
        user->matriz_what_songs[i] = (int *)calloc(1000, sizeof(int));
    }

    return stat;
}

void reset_stat_6(Stat_6 stat)
{
    if (!stat || !stat->data)
        return;

    stat->user_id = -1;
    stat->year = -1;
    stat->n = -1;

    User_Cell user = stat->data;

    user->total_sec_listen = 0;
    user->songs_listen = 0;

    if (user->what_songs_listen)
    {
        memset(user->what_songs_listen, 0, MAX_SONGS * sizeof(int));
    }

    if (user->songs_p_day_data)
    {
        memset(user->songs_p_day_data, 0, NUM_DAYS * sizeof(int));
    }

    if (user->artists_data)
    {
        for (int i = 0; i < NUM_ARTISTS; i++)
        {
            if (user->artists_data[i])
            {
                user->artists_data[i]->artist_id = -1;
                user->artists_data[i]->nr_songs_listen = 0;
                user->artists_data[i]->total_sec = 0;
            }
        }
    }

    if (user->genres_data)
    {
        for (int i = 0; i < NUM_GENRES; i++)
        {
            if (user->genres_data[i])
            {
                user->genres_data[i]->total_sec = 0;
            }
        }
    }

    if (user->albums_data)
    {
        memset(user->albums_data, 0, NUM_ALBUMS * sizeof(int));
    }

    if (user->hour_data)
    {
        memset(user->hour_data, 0, NUM_HOURS * sizeof(int));
    }

    if (user->matriz_what_songs)
    {
        for (int i = 0; i < NUM_ARTISTS; i++)
        {
            if (user->matriz_what_songs[i])
            {
                memset(user->matriz_what_songs[i], 0, 1000 * sizeof(int));
            }
        }
    }
}

void destroy_stat_6(Stat_6 stat)
{
    if (!stat)
        return;

    User_Cell user = stat->data;
    if (user)
    {
        if (user->what_songs_listen)
            free(user->what_songs_listen);
        if (user->songs_p_day_data)
            free(user->songs_p_day_data);

        if (user->artists_data)
        {
            for (int i = 0; i < NUM_ARTISTS; i++)
            {
                if (user->artists_data[i])
                    free(user->artists_data[i]);
            }
            free(user->artists_data);
        }

        if (user->genres_data)
        {
            for (int i = 0; i < NUM_GENRES; i++)
            {
                if (user->genres_data[i])
                {
                    if (user->genres_data[i]->genre)
                        free(user->genres_data[i]->genre);
                    free(user->genres_data[i]);
                }
            }
            free(user->genres_data);
        }

        if (user->albums_data)
            free(user->albums_data);

        if (user->hour_data)
            free(user->hour_data);

        if (user->matriz_what_songs)
        {
            for (int i = 0; i < NUM_ARTISTS; i++)
            {
                if (user->matriz_what_songs[i])
                    free(user->matriz_what_songs[i]);
            }
            free(user->matriz_what_songs);
        }

        free(user);
    }

    free(stat);
}

void print_stat_6(Stat_6 stat, int n)
{
    if (!stat || !stat->data)
    {
        printf("Stat_6 is NULL.\n");
        return;
    }

    // Imprimir campos principais
    printf("User ID: %d\n", stat->user_id);
    printf("Year: %d\n", stat->year);
    printf("N: %d\n", stat->n);

    User_Cell user = stat->data;

    // Imprimir campos de User_Cell
    printf("Total seconds listened: %d\n", user->total_sec_listen);
    printf("Songs listened: %d\n", user->songs_listen);

    // Imprimir o array what_songs_listen
    printf("Songs listened (first %d elements):\n", n);
    for (int i = 0; i < n && i < MAX_SONGS; i++)
    {
        printf("Song %d: %d\n", i, user->what_songs_listen[i]);
    }
    if (n < MAX_SONGS)
        printf("...\n");

    // Imprimir o array songs_p_day_data
    printf("Songs listened per day (first %d days):\n", n);
    for (int i = 0; i < n && i < NUM_DAYS; i++)
    {
        printf("Day %d: %d\n", i, user->songs_p_day_data[i]);
    }
    if (n < NUM_DAYS)
        printf("...\n");

    // Imprimir dados dos artistas (artists_data)
    printf("Artists data (first %d artists):\n", n);
    for (int i = 0; i < n && i < NUM_ARTISTS; i++)
    {
        Artist_Cell artist = user->artists_data[i];

        printf("Slot: %d, Artist %d, Songs listened: %d, Total seconds: %d\n",
               i, artist->artist_id, artist->nr_songs_listen, artist->total_sec);
    }
    if (n < NUM_ARTISTS)
        printf("...\n");

    // Imprimir dados dos gêneros (genres_data)
    printf("Genres data (first %d genres):\n", n);
    for (int i = 0; i < n && i < NUM_GENRES; i++)
    {
        Genre_Cell genre = user->genres_data[i];
        if (genre->genre)
        {
            printf("Genre %d - %s, Total seconds: %d\n", i, genre->genre, genre->total_sec);
        }
    }
    if (n < NUM_GENRES)
        printf("...\n");

    // Imprimir dados dos álbuns (albums_data)
    printf("\nAlbums (first %d elements):\n", n);
    for (int i = 0; i < n && i < NUM_ALBUMS; i++)
    {
        printf("Album %d,  Total seconds %d\n", i, user->albums_data[i]);
    }
    if (n < NUM_ALBUMS)
        printf("...\n");

    // Imprimir dados das horas (hour_data)
    printf("Hours (first %d elements):\n", n);
    for (int i = 0; i < n && i < NUM_HOURS; i++)
    {
        printf("Hour %d,  Total seconds %d\n", i, user->hour_data[i]);
    }
    if (n < NUM_HOURS)
        printf("...\n");

    // Imprimir matriz matriz_what_songs
    printf("\nMatrix of songs listened (first %d artists, first %d songs):\n", n, n);
    for (int i = 0; i < n && i < NUM_ARTISTS; i++)
    {
        for (int j = 0; j < n && j < MAX_SONGS; j++)
        {
            printf("%d ", user->matriz_what_songs[i][j]);
        }
        printf("\n");
    }
    if (n < NUM_ARTISTS || n < MAX_SONGS)
        printf("...\n");
}

int get_stat_6_id(Stat_6 stat)
{
    return (stat->user_id);
}

int get_stat_6_year(Stat_6 stat)
{
    return (stat->year);
}

void exec_stat_6(Stat_6 stat, Histories histories, Songs songs, unsigned int user_id, unsigned int year, int n)
{
    stat->user_id = user_id;
    stat->year = year;
    stat->n = n; // pode ser -1 ou atribuido

    iterate_histories_stat_6(stat, histories, songs);
}

void add_data_stat_6(Stat_6 stat, Songs songs, unsigned int song_id, Date date, Time time, Time duration)
{
    User_Cell user = stat->data;

    Song song = get_song(song_id, songs);

    LInt artist_list = get_author(song);

    int seconds = calculate_duration(duration);
    ///////////////////////////////////////////////////

    // tempo total reproducao
    user->total_sec_listen += seconds;

    // nr de musicas diferentes ouvidas
    if (user->what_songs_listen[song_id] == 0)
    {
        (user->songs_listen)++;
        user->what_songs_listen[song_id] = 1;
    }

    // musicas por dia
    int day = day_of_year(get_year(date), get_month(date), get_day(date));
    (user->songs_p_day_data[day])++;

    // horas
    int hour = get_hour(time);
    user->hour_data[hour] += seconds;

    // albuns
    user->albums_data[get_album_id(song)] += seconds;

    // generos
    char *song_genre = get_genre(song);
    int i;

    for (i = 0; i < NUM_GENRES; i++)
    {
        if (!strcmp(song_genre, user->genres_data[i]->genre))
            break;
    }
    user->genres_data[i]->total_sec += seconds;

    // artistas
    LInt current = artist_list;

    while (current != NULL)
    {
        if (user->artists_data[lint_get_value(current)]->artist_id == -1)
            user->artists_data[lint_get_value(current)]->artist_id = lint_get_value(current);
        user->artists_data[lint_get_value(current)]->total_sec += seconds;

        for (int k = 0; k < 1000; k++)
        {
            if (user->matriz_what_songs[lint_get_value(current)][k] == 0)
            {
                user->matriz_what_songs[lint_get_value(current)][k] = song_id;
                user->artists_data[lint_get_value(current)]->nr_songs_listen++;
                break;
            }

            if (user->matriz_what_songs[lint_get_value(current)][k] == song_id)
            {
                break;
            }
        }

        current = lint_get_next(current);
    }

    ///////////////////////////////////////////////////
    free_song(song);
    free(song_genre);
    free_lint(artist_list);
}

// Função de comparação para qsort
int compareArtists(const void *a, const void *b)
{
    Artist_Cell artistA = *(Artist_Cell *)a;
    Artist_Cell artistB = *(Artist_Cell *)b;

    // Comparar pelo total de segundos ou pelo ID do artista em caso de empate
    if (artistA->total_sec != artistB->total_sec)
    {
        return artistB->total_sec - artistA->total_sec; // Ordem decrescente
    }
    return artistA->artist_id - artistB->artist_id; // Ordem crescente de IDs
}

char *query_6_res(Stat_6 stat, char *sep_q)
{
    if (stat->data->total_sec_listen == 0)
        return strdup("\n");

    // Ordenar artistas com qsort
    qsort(stat->data->artists_data, NUM_ARTISTS, sizeof(Artist_Cell), compareArtists);

    // -> listening time
    char *listening_time = sec_to_string(stat->data->total_sec_listen);

    // -> #musics
    int musics = stat->data->songs_listen;

    // -> artist
    int artist = stat->data->artists_data[0]->artist_id;

    // -> day
    int day = 0;
    for (int i = 0; i < 366; i++)
    {
        if (stat->data->songs_p_day_data[i] > stat->data->songs_p_day_data[day] ||
            (stat->data->songs_p_day_data[i] == stat->data->songs_p_day_data[day] &&
             i > day))
            day = i;
    }
    // printf("\n\n\n%d\n\n\n", day);
    char *date = calculate_date(stat->year, day);

    // -> genre
    int g = 0;
    for (int i = 0; i < 10; i++)
    {
        if (stat->data->genres_data[i]->total_sec > stat->data->genres_data[g]->total_sec ||
            (stat->data->genres_data[i]->total_sec == stat->data->genres_data[g]->total_sec &&
             strcmp(stat->data->genres_data[i]->genre, stat->data->genres_data[g]->genre) < 0))
            g = i;
    }
    char *genre = strdup(stat->data->genres_data[g]->genre);

    // -> album
    int album = 0;
    for (int i = 0; i < NUM_ALBUMS; i++)
    {
        if (stat->data->albums_data[i] > stat->data->albums_data[album] ||
            (stat->data->albums_data[i] == stat->data->albums_data[album] &&
             i < album))
            album = i;
    }

    // -> hour
    int hour = 0;
    for (int i = 0; i < NUM_HOURS; i++)
    {
        if (stat->data->hour_data[i] > stat->data->hour_data[hour] ||
            (stat->data->hour_data[i] == stat->data->hour_data[hour] &&
             i < hour))
            hour = i;
    }

    // Calcula o tamanho necessário para armazenar a string final
    size_t result_size = snprintf(NULL, 0, "%s%s%d%sA%07d%s%s%s%s%sAL%06d%s%02d\n",
                                  listening_time, sep_q, musics, sep_q, artist, sep_q,
                                  date, sep_q, genre, sep_q, album, sep_q, hour);

    // Aloca memória para a string resultante
    char *result = malloc(result_size + 1); // +1 para o terminador nulo

    // Formata a string final
    snprintf(result, result_size + 1, "%s%s%d%sA%07d%s%s%s%s%sAL%06d%s%02d\n",
             listening_time, sep_q, musics, sep_q, artist, sep_q,
             date, sep_q, genre, sep_q, album, sep_q, hour);

    free(date);
    free(genre);
    free(listening_time);

    // Retorna a string completa
    return result;
}

char *query_6_res_filter(Stat_6 stat, char *sep_p)
{
    if (stat->data->artists_data[0]->artist_id == -1)
        return strdup("");

    int n = stat->n;
    // Estimativa inicial para o tamanho da string final.
    int buffer_size = 1024;
    char *result = (char *)malloc(buffer_size * sizeof(char));

    // Inicializa a string resultante com uma string vazia
    result[0] = '\0';

    for (int i = 0; i < n; i++)
    {
        if (stat->data->artists_data[i]->artist_id == -1)
            continue; // Ignora as entradas onde artist_id é -1

        // Prepara a string de tempo de escuta
        char *listening_time = sec_to_string(stat->data->artists_data[i]->total_sec);

        // Estima o espaço necessário para a nova linha
        int needed_size = snprintf(NULL, 0, "A%07d%s%d%s%s\n",
                                   stat->data->artists_data[i]->artist_id,
                                   sep_p,
                                   stat->data->artists_data[i]->nr_songs_listen,
                                   sep_p, listening_time) +
                          1;

        // Realoca o buffer se necessário
        if (strlen(result) + needed_size >= buffer_size)
        {
            buffer_size = strlen(result) + needed_size + 1024; // Aumenta o buffer
            result = (char *)realloc(result, buffer_size);
        }

        // Adiciona a linha à string final
        snprintf(result + strlen(result), needed_size, "A%07d%s%d%s%s\n",
                 stat->data->artists_data[i]->artist_id,
                 sep_p,
                 stat->data->artists_data[i]->nr_songs_listen,
                 sep_p, listening_time);

        free(listening_time); // Liberta a memória alocada para o tempo de escuta
    }

    return result; // Retorna a string concatenada
}