/**
 * @file history_manager.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef HISTORYM_H
#define HISTORYM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "history.h"

typedef struct histories *Histories;
typedef struct stat_6 *Stat_6;

/**
 * @brief Função responsável por criar o catálogo de histories
 * 
 * @param num_buckets Número de buckets com que a hashtable é criada
 * @return Histories 
 */
Histories init_history_catalog(int num_buckets);

/**
 * @brief Função responsável por inserir uma history no catálogo
 * 
 * @param catalog       Catálogo atual de histories
 * @param new_history   Nova history a ser inserida
 * @param id            Id da nova history
 * @return true         A history foi inserida com sucesso
 * @return false        Falha a inserir a history
 */
bool insert_history(Histories catalog, History new_history, unsigned int id);

/**
 * @brief Função responsável por libertar a memória alocada para o catálogo de histories
 * 
 * @param catalog Catálogo atual de histories
 */
void destroy_history_catalog(Histories catalog);


/**
 * @brief FUnção que imprime o catálogo de histories
 * 
 * @param catalog Catálogo atual de histories
 */
void print_history_catalog(Histories catalog);


/**
 * @brief Função responsável por iterar o catálogo de histories para armazenar informações relativas à query6
 * 
 * @param stat      Estatisticas query6
 * @param histories Catálogo atual de histories
 * @param songs     Catálogo atual de songs
 */
void iterate_histories_stat_6(Stat_6 stat, Histories histories, Songs songs);

#endif