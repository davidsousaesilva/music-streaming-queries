/**
 * @file album_manager.h
 * @author David Silva, João Aleixo, João Costa 
 * 
 */

#ifndef ALBUMm_H
#define ALBUMm_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "album.h"

typedef struct albums *Albums;
typedef struct album *Album;


/**
 * @brief Função responsável por criar o catálogo de albuns
 * 
 * @param num_buckets Número de buckets com que a hashtable é criada
 * @return Albums 
 */
Albums init_album_catalog(int num_buckets);

/**
 * @brief Função responsável por inserir um album no catálogo
 * 
 * @param catalog   Atual catálogo de albums
 * @param new_album Album que irá ser inserido
 * @param id        Id do album a ser inserido
 * @return true     O album foi inserido com sucesso
 * @return false    Não foi possível inserir o album
 */
bool insert_album(Albums catalog, Album new_album, unsigned int id);

/**
 * @brief Função que pretende obter o número de albuns no catálogo
 * 
 * @param catalog Atual catálogo de albuns
 * @return int 
 */
int get_nr_albums(Albums catalog);

/**
 * @brief Função para obter a hashtable da estrutura albums
 * 
 * @param catalog Atual catálogo de albuns
 * @return Hashtable 
 */
Hashtable get_album_table(Albums catalog);

/**
 * @brief Função responsável por libertar a memória alocada para o catálogo de albuns
 * 
 * @param catalog Atual catálogo de albuns
 */
void destroy_album_catalog(Albums catalog);

/**
 * @brief Função que imprime o catálogo de albuns
 * 
 * @param catalog Atual catálogo de albuns
 */
void print_album_catalog(Albums catalog);

/**
 * @brief Função que ao receber um id verifica se existe no catálogo
 * 
 * @param catalog Atual catálogo de albuns
 * @param id      Id de procura
 * @return true   O album existe no catálogo
 * @return false  O album não existe no catálogo
 */
bool check_if_album_exists(Albums catalog, unsigned int id);

#endif
