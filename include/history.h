/**
 * @file history.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */
#ifndef HISTORY_H
#define HISTORY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lint.h"
#include "utils.h"
#include "duration.h"
#include "date.h"
#include "song_manager.h"
#include "artist_manager.h"
#include "statistics.h"

typedef struct history *History;
typedef struct histories *Histories;
typedef struct stat_4 *Stat_4;
typedef struct stat_5 *Stat_5;

/**
 * @brief Função responsável por criar e atribuir cada parametro da history
 * 
 * @param id               Id da history
 * @param user_id          Id do user
 * @param song_id          Id da song
 * @param timestamp_date   Data
 * @param timestamp_time   Time
 * @param duration         Tempo ouvido
 * @param mobile           Bool que indica se ouviu em mobile ou não
 * @param catalog          Catálogo de músicas
 * @param artists          Catálogo de artistas
 * @return History 
 * 
 * Durante a execução da função create_history é também executada a função update_recipe
 * para atualizar a receita total ganha pelos artistas, informação necessária
 * para a query1
 */
History create_history(unsigned int id, unsigned int user_id, unsigned int song_id, Date timestamp_date, Time timestamp_time, Time duration, bool mobile, Songs catalog, Artists artists);

/**
 * @brief Função responsável por libertar a memória alocada para a history
 * 
 * @param history_ptr 
 */
void free_history(void *history_ptr);

/**
 * @brief Função que recebe a informação de uma history e faz parse desta
 * 
 * @param buffer        Informação recebida
 * @param histories     Catálogo de histories
 * @param error_file    Ficheiro caso entrada seja inválida
 * @param sep           Seperador de informação para este ficheiro
 * @param catalog       Catálogo de songs
 * @param artists       Catálogo de artistas
 * @param stat_4        Estatisticas para a query4
 * @param stat_5        Estatisticas para a query5
 */
void parse_history(char *buffer, Histories histories, FILE *error_file, char *sep, Songs catalog, Artists artists, Stat_4 stat_4, Stat_5 stat_5);

/**
 * @brief Função que imprime uma history
 * 
 * @param history_ptr 
 */
History create_history(unsigned int id, unsigned int user_id, unsigned int song_id, Date timestamp_date, Time timestamp_time, Time duration, bool mobile, Songs catalog, Artists artists);


/**
 * @brief Função responsável por armazenar os dados necessários para a query6
 * 
 * @param history_ptr 
 * @param stat_6_ptr 
 * @param songs_ptr 
 */
void store_values_query_6(void *history_ptr, void *stat_6_ptr, void *songs_ptr);

void print_history(void *history_ptr);

#endif