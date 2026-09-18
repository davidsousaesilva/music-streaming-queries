#include "queries.h"

char *query1(char *queryArgs, Users users, Artists artists, char *separador_q, unsigned int n)
{
    char tipo = queryArgs[0];
    unsigned int id = (unsigned int)atoi(queryArgs + 1);

    if (tipo == 'U')
    {
        User user = get_user(users, id);

        if (!user)
            return strdup("\n"); // Se o utilizador não existir retorna uma linha vazia

        // Obter a informação do utilizador
        int age = get_age(user);
        char *email = get_email(user);
        char *fst_name = get_fst_name(user);
        char *last_name = get_last_name(user);
        char *country = get_country(user);

        // Calcular o tamanho necessário para o resultado
        size_t buffer_size = snprintf(NULL, 0, "%s%s%s%s%s%s%d%s%s\n",
                                      email, separador_q,
                                      fst_name, separador_q,
                                      last_name, separador_q,
                                      age, separador_q,
                                      country);

        // Alocar a memória para o resultado e escrevê-lo
        char *result = malloc(buffer_size + 1);
        if (result)
        {
            snprintf(result, buffer_size + 1, "%s%s%s%s%s%s%d%s%s\n",
                     email, separador_q,
                     fst_name, separador_q,
                     last_name, separador_q,
                     age, separador_q,
                     country);
        }
        free(email);
        free(fst_name);
        free(last_name);
        free(country);
        free_user(user);

        return result;
    }
    if (tipo == 'A')
    {
        Artist artist = get_artist(artists, id);

        if (!artist)
            return strdup("\n"); // Se o artista não existir retorna uma linha vazia

        char *name = get_artist_name(artist);
        char *type = get_type(artist);
        char *country = get_artist_country(artist);
        unsigned int nr_albums = get_artist_albums(artist); 
        double total_recipe = get_artist_recipe(artist);    

        // Calcular o tamanho necessário para o resultado
        size_t buffer_size = snprintf(NULL, 0, "%s%s%s%s%s%s%d%s%.2f\n",
                                      name, separador_q,
                                      type, separador_q,
                                      country, separador_q,
                                      nr_albums, separador_q,
                                      total_recipe);

        // Alocar a memória para o resultado e escrevê-lo
        char *result = malloc(buffer_size + 1);
        if (result)
        {
            snprintf(result, buffer_size + 1, "%s%s%s%s%s%s%d%s%.2f\n",
                     name, separador_q,
                     type, separador_q,
                     country, separador_q,
                     nr_albums, separador_q,
                     total_recipe);
        }
        // Libertar memória
        free(name);
        free(country);
        free_artist(artist);

        return result;
    }
    // Caso não seja nem artista nem utilizador retorna uma linha vazia
    return strdup("\n");
}

char *query2(char *queryArgs, Artists artists, Songs songs, Stat_2 stat_2, char *separador_q, unsigned int n)
{
    // parse linha
    char *nr = strtok_r(queryArgs, " ", &queryArgs);
    char *country = strtok_r(NULL, "\n", &queryArgs);

    unsigned int nr_artistas = (unsigned int)atoi(nr);

    char *result;

    if (country != NULL)
    {
        result = query2_filter_result(stat_2, nr_artistas, remove_quotes(country), separador_q);
    }
    else
    {
        result = query2_nofilter_result(stat_2, nr_artistas, separador_q);
    }

    return result;
}

char *query3(char *queryArgs, Users users, Songs songs, Stat_3 stat_3, char *separador_q, unsigned int n)
{

    int min_age, max_age;
    sscanf(queryArgs, "%d %d", &min_age, &max_age);
    reset_all_song_likes(songs);
    char *result;

    exec_stat_3(stat_3, songs, users, min_age, max_age);

    result = query3_result(stat_3, separador_q);

    reset_stat_3(stat_3);

    return result;
}

char *query4(char *queryArgs, Artists artists, Songs songs, Histories histories, Stat_4 stat_4, char *separador_q, unsigned int n)
{
    unsigned int artist_id, count;
    char *result = NULL;

    if (queryArgs != NULL && strlen(queryArgs) > 0)
    {
        char *begin_date = strtok_r(queryArgs, " ", &queryArgs);
        char *end_date = strtok_r(NULL, "\n", &queryArgs);

        artist_id = get_res_filter_id(stat_4, begin_date, end_date);
        count = get_res_filter_count(stat_4, begin_date, end_date);

        if (artist_id == -1 || count == -1)
            return strdup("\n");

        Artist artist = get_artist(artists, artist_id);
        char *type = get_type(artist);
        free_artist(artist);

        // Calcular o tamanho necessário para o resultado
        size_t buffer_size = snprintf(NULL, 0, "A%07d%s%s%s%d\n", artist_id, separador_q, type, separador_q, count);

        // Alocar a memória para o resultado e escrevê-lo
        result = malloc(buffer_size + 1);
        if (result)
        {
            snprintf(result, buffer_size + 1, "A%07d%s%s%s%d\n", artist_id, separador_q, type, separador_q, count);
        }
    }
    else
    {
        artist_id = get_res_no_filter_id(stat_4);
        count = get_res_no_filter_count(stat_4);

        Artist artist = get_artist(artists, artist_id);
        char *type = get_type(artist);
        free_artist(artist);

        // Calcular o tamanho necessário para o resultado
        size_t buffer_size = snprintf(NULL, 0, "A%07d%s%s%s%d\n", artist_id, separador_q, type, separador_q, count);

        // Alocar a memória para o resultado e escrevê-lo
        result = malloc(buffer_size + 1);
        if (result)
        {
            snprintf(result, buffer_size + 1, "A%07d%s%s%s%d\n", artist_id, separador_q, type, separador_q, count);
        }
    }

    return result;
}

char *query5(char *queryArgs, Users users, Stat_5 stat_5, char *sep_q, int n)
{
    char *idUtilizadorAlvo = strtok_r(queryArgs + 1, " ", &queryArgs);
    int numRecomendacoes = atoi(queryArgs);

    if (numRecomendacoes == 0)
    {
        return strdup("\n");
    }

    if(!check_if_user_exists(users, atoi(idUtilizadorAlvo)) || numRecomendacoes == 0)
        return strdup("\n");

    int **matriz = stat_5_get_matriz(stat_5);
    char **usersA = stat_5_get_users(stat_5);
    char **genresA = stat_5_get_genres(stat_5);

    char **recomendados = recomendaUtilizadores(
        idUtilizadorAlvo,
        matriz,
        usersA,
        genresA,
        400005,
        10,
        numRecomendacoes);

    size_t buffer_size = 0;

    for (int i = 0; i < numRecomendacoes; i++) {
        if (recomendados[i]) {
            buffer_size += strlen(recomendados[i]) + 2; // +2 para 'U' e '\n'
        }
    }

    // Se não houver recomendações, alocar apenas espaço para '\n' e o terminador nulo
    char *result = malloc((buffer_size > 0 ? buffer_size : 1) + 1); // +1 para o terminador nulo
    if (!result) {
        perror("Erro ao alocar memória para result");
        return NULL; 
    }

    if (recomendados[0] == NULL) {
        strcpy(result, "\n"); // Apenas adiciona '\n' se não houver recomendações
    } else {
        result[0] = '\0'; // Inicializa `result` como uma string vazia
        for (int i = 0; i < numRecomendacoes; i++) {
            if (recomendados[i]) {
                size_t len = strlen(recomendados[i]);
                char temp[len + 3]; // +3 para 'U', '\n', e o terminador nulo
                temp[0] = 'U';      // Adiciona 'U' no início
                strcpy(temp + 1, recomendados[i]); // Copia a string original após o 'U'
                temp[len + 1] = '\n';              // Adiciona '\n' no final
                temp[len + 2] = '\0';              // Adiciona o terminador nulo

                strcat(result, temp); // Concatena a string modificada ao resultado
            }
        }
    }

    free_stat_5_matriz(matriz);
    free_stat_5_users(usersA);
    free_stat_5_genres(genresA);

    for (int i = 0; i < numRecomendacoes; i++) {
        recomendados[i] = NULL;
    }
    free(recomendados);

    return result;
}



char *query6(char *queryArgs, Stat_6 stat_6, Histories histories, Songs songs, char *sep_q, int n)
{
    char *user_id_str = strtok_r(queryArgs, " ", &queryArgs);
    char *year_str = strtok_r(NULL, " ", &queryArgs);
    char *n_artists_str = strtok_r(NULL, "\n", &queryArgs);

    if (year_str == NULL) 
    {
        return strdup("\n");
    }

    unsigned int user_id = (unsigned int)atoi(user_id_str + 1);
    unsigned int year = (unsigned int)atoi(year_str);
    unsigned int n_artists;
    if (n_artists_str == NULL)
        n_artists = -1;
    else
        n_artists = atoi(n_artists_str);

    // Chama a função exec_stat_6 para processar os dados
    exec_stat_6(stat_6, histories, songs, user_id, year, n_artists);

    // Obtém os resultados
    char *result = query_6_res(stat_6, sep_q);
    char *result_filter = query_6_res_filter(stat_6, sep_q);

    // Aloca memória para a string final (tamanho combinado das duas)
    int total_length = strlen(result) + strlen(result_filter) + 1; // +1 para o terminador de string '\0'
    char *both_result = (char *)malloc(total_length * sizeof(char));

    // Concatena result e result_filter
    strcpy(both_result, result);        // Copia o conteúdo de result
    strcat(both_result, result_filter); // Concatena o conteúdo de result_filter

    // Liberta as strings result e result_filter
    free(result);
    free(result_filter);

    reset_stat_6(stat_6);

    return both_result; // Retorna a string combinada
}
