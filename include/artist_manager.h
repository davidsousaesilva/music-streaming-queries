/**
 * @file artist_manager.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */


#ifndef ARTISTm_H
#define ARTISTm_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct artists *Artists;
typedef struct stat_2 *Stat_2;

#include "artist.h"
#include "hashtable.h"

/**
 * @brief Função responsável por criar o catálogo de artistas
 * 
 * @param num_buckets Número de buckets com que a hashtable é criada
 * @return Artists 
 */
Artists init_artist_catalog(int num_buckets);

/**
 * @brief Função responsável por inserir um artista no catálogo
 * 
 * @param catalog     Catálogo atual de artistas
 * @param new_artist  Novo artista a ser inserido
 * @param id          Id do novo artista
 * @return true       O artista foi inserido com sucesso
 * @return false      Falha a inserir o artista
 */
bool insert_artist(Artists catalog, Artist new_artist, unsigned int id);

/**
 * @brief Função que devolve o número de artista no catálogo
 * 
 * @param catalog  Catálogo atual de artistas
 * @return int 
 */
int get_nr_artists(Artists catalog);

/**
 * @brief Função responsável por libertar a memória alocada para o catálogo de artistas
 * 
 * @param catalog  Catálogo atual de artistas
 */
void destroy_artist_catalog(Artists catalog);

/**
 * @brief Função que imprime o catálogo de artistas
 * 
 * @param catalog  Catálogo atual de artistas
 */
void print_artist_catalog(Artists catalog);

/**
 * @brief Função que ao receber um id procura o artista no catálogo e devolve-o
 * 
 * @param catalog Catálogo atual de artistas
 * @param id      Id de procura
 * @return Artist 
 */
Artist get_artist(Artists catalog, unsigned int id);

/**
 * @brief Função que verifica se um artista existe no catálogo
 * 
 * @param catalog Catálogo atual de artistas
 * @param id      Id de procura
 * @return true   O artista foi encontrado no catálogo
 * @return false  O artista não existe no catálogo
 */
bool check_if_artist_exists(Artists catalog, unsigned int id);

/**
 * @brief Função responsável por aumentar a discografia dos artistas
 * 
 * @param catalog       Catálogo atual de artistas
 * @param id            Id de procura
 * @param duration_sec  Tempo a ser incrementado
 * 
 * Uma vez que a discografia total do artista é necessária para a query2
 * esta função irá incrementar a discografia total de cada artista
 * 
 */
void acc_artists_discography(Artists catalog, unsigned int id, unsigned int duration_sec);

/**
 * @brief Função responsável por aumentar o número de albuns dos artistas
 * 
 * @param artists Lista de artistas que irão ser incrementados
 * @param catalog Catálogo atual de artistas
 * 
 * O número de albuns de cada artista é necessário para a query1.
 * Esta função durante o parse dos albums irá receber uma lista de artistas autores desse album
 * e irá atualizar o nr_albums desses artistas
 * 
 */
void update_album_count(LInt artists, Artists catalog);

/**
 * @brief Função responsável por atualizar a receita total dos artistas
 * 
 * @param artists Lista de artistas a ser atualizados
 * @param catalog Catálogo atual de artistas
 * 
 * A receita total é uma das informações pedidas na query1.
 * Esta função irá durante o parse do history receber uma lista de artistas criadores da song correspondente
 * àquela stream. A função irá verificar se o artista é individual ou coletivo. 
 * Caso seja individual apenas calcula a receita para esse artista
 * Caso seja um grupo, irá calcular quantos membros o grupo tem, quanto é a receita atribuida a cada membro
 * do grupo e irá atribuir essa receita a cada membro
 * 
 */
void update_recipe(LInt artists, Artists catalog);

/**
 * @brief Função que irá iterar a hashtable de artistas para armazenar valores da stat_2
 * 
 * @param artists Catálogo atual de artistas
 * @param stat    Estrutura stat_2
 */
void iterate_artists_stat2(Artists artists, Stat_2 stat);

#endif