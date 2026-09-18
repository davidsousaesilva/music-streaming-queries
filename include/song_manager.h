/**
 * @file song_manager.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */


#ifndef SONGm_H
#define SONGm_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "song.h"
#include "hashtable.h"
#include "user.h"

typedef struct songs *Songs;
typedef struct users *Users;
typedef struct stat_3 *Stat_3;

/**
 * @brief Função responsável por inicializar o catálogo de songs
 * 
 * @param num_buckets Números de buckets com que a hashtable é criada
 * @return Songs 
 */
Songs init_song_catalog(int num_buckets);

/**
 * @brief Função responsável por inserir uma música no catálogo
 * 
 * @param catalog   Atual catálogo de songs
 * @param new_song  Song que será inserida
 * @param id        Id da song a ser inserida
 * @return true     A song foi inserida com sucesso
 * @return false    Não foi possível inserir a song
 */
bool insert_song(Songs catalog, Song new_song, unsigned int id);

/**
 * @brief Função para obter o números de songs no catálogo
 * 
 * @param catalog Atual catálogo de songs
 * @return int 
 */
int get_nr_songs(Songs catalog);

/**
 * @brief Função responsável por libertar a memória alocada para o catálogo de songs
 * 
 * @param catalog 
 */
void destroy_song_catalog(Songs catalog);

/**
 * @brief Função que imprime o catálogo de songs
 * 
 * @param catalog 
 */
void print_song_catalog(Songs catalog);

/**
 * @brief Função que ao receber um id verifica se existe no catálogo
 * 
 * @param catalog Atual catálogo de songs
 * @param id      Id de procura
 * @return true   A song existe no catálogo
 * @return false  A song não existe no catálogo
 */
bool check_if_song_exists(Songs catalog, unsigned int id);

/**
 * @brief Função responsável por retornar os likes de todas as músicas a 0
 * 
 * @param songs 
 */
void reset_all_song_likes(Songs songs);

/**
 * @brief Função que retorna uma song caso ela exista no catálogo
 * 
 * @param id      Id de procura
 * @param catalog Atual catálogo de songs
 * @return Song 
 */
Song get_song(unsigned int id, Songs catalog);

/**
 * @brief Função responsável por iterar o catálogo de songs para preencher o stat2
 * 
 * @param songs 
 * @param artists 
 */
void iterate_songs_stat2(Songs songs, Artists artists);

/**
 * @brief Funcão responsável por acumular os likes de cada song
 * 
 * @param user_ptr Apontador de um utilizador
 * @param context1 Atual catálogo de songs
 * @param context2 Min_age
 * @param context3 Max_age
 */
void accumulate_user_likes(void *user_ptr, void *context1, void *context2, void *context3);

/**
 * @brief Função responsável por acumular os likes de cada gênero do catálogo de songs
 * 
 * @param songs Atual catálogo de songs
 * @param stat  Estatisticas query3
 */
void iterate_songs_stat3(Songs songs, Stat_3 stat);

#endif