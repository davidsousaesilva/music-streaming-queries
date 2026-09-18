/**
 * @file artist.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef ARTIST_H
#define ARTIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct artist *Artist;
typedef struct hashtable *Hashtable;
typedef struct song *Song;
typedef struct artists *Artists;
typedef struct songs *Songs;
typedef struct users *Users;
typedef struct stat_2 *Stat_2;

#include "lint.h"
#include "statistics.h"

/**
 * @brief Função responsável por criar e atribuir cada parametro do artista
 * 
 * @param id           Id do artista
 * @param name         Nome do artista
 * @param description  Descrição do artista
 * @param recipe_p_s   Receita que o artista recebe por cada stream 
 * @param id_const     Lista de ids de artistas constituentes da banda caso se trate de um grupo
 * @param country      Pais do artista
 * @param group        Bool que indica se um artista é um grupo ou individual
 * @return Artist 
 */
Artist create_artist(unsigned int id, const char *name, const char *description,
                     double recipe_p_s, LInt id_const, const char *country, bool group);

/**
 * @brief Função responsável por libertar a memória alocada para o artista
 * 
 * @param artist_ptr 
 */
void free_artist(void *artist_ptr);

/**
 * @brief Função que imprime um artista
 * 
 * @param artist_ptr 
 */
void print_artist(void *artist_ptr);

/**
 * @brief Função que duplica um artista
 * 
 * @param ori_artist 
 * @return Artist 
 * 
 * Para permitir segurança dos dados, quando é necessário trabalhar sobre um artista
 * é mais seguro utilizar uma cópia deste daí a necessidade de criar esta função
 */
Artist dup_artist(Artist ori_artist);

/**
 * @brief Função responsável por armazenar os dados relativos a cada artista na stat_2
 * 
 * @param artist_ptr 
 * @param stat_2_ptr 
 */
void store_values_query2(void *artist_ptr, void *stat_2_ptr);

/**
 * @brief Função que aumenta a discografia total de cada artista
 * 
 * @param artist        
 * @param duration_sec Tempo de discografia que irá ser acrescentado
 */
void acc_artist_discography(Artist artist, unsigned int duration_sec);

/**
 * @brief Função que devolve o id de um artista
 * 
 * @param artist 
 * @return unsigned int 
 */
unsigned int get_artist_id(Artist artist);

/**
 * @brief Função que devolve o nome do artista
 * 
 * @param artist 
 * @return char* 
 */
char *get_artist_name(Artist artist);

/**
 * @brief Função que devolve a discografia total de um artista
 * 
 * @param artist 
 * @return unsigned int 
 */
unsigned int get_artist_total_discography(Artist artist);

/**
 * @brief Função que devolve o país de um artista
 * 
 * @param artist 
 * @return char* 
 */
char* get_artist_country(Artist artist);

/**
 * @brief Função que devolve o tipo de um artista(individual ou group)
 * 
 * @param artist 
 * @return char* 
 */
char* get_type(Artist artist);

/**
 * @brief Função que devolve a receita total de um artista
 * 
 * @param artist 
 * @return double 
 */
double get_artist_recipe(Artist artist);

/**
 * @brief Função que adiciona a receita que um artista deve receber da sua presença num grupo
 * 
 * @param artist 
 * @param cut    Parte de uma música que o membro pertencente de uma banda deve receber
 */
void recipe_from_group(Artist artist, double cut);

/**
 * @brief Função que incrementa a receita total de um artista
 * 
 * @param artist 
 */
void calculate_recipe(Artist artist);

/**
 * @brief Função que incrementa o número de albuns de um artista
 * 
 * @param artist 
 */
void increment_artist_nr_albums(Artist artist);

/**
 * @brief Função que devolve o número de albuns de um artista
 * 
 * @param artist 
 * @return unsigned int 
 */
unsigned int get_artist_albums(Artist artist);

/**
 * @brief Devolve a lista de membros de uma banda
 * 
 * @param artist 
 * @return LInt 
 */
LInt get_members(Artist artist);

/**
 * @brief Verifica se um artista é individual ou group
 * 
 * @param artist 
 * @return true  O artista é individual
 * @return false O artista é group
 */
bool isIndividual(Artist artist);

/**
 * @brief Função que devolve a receita por stream de um artista
 * 
 * @param artist 
 * @return double 
 */
double get_rps(Artist artist);

/**
 * @brief Função que recebe a informação de um artista e faz parse do mesmo
 * 
 * @param buffer        Informação recebida
 * @param artists       Catálogo de artistas
 * @param error_file    Ficheiro caso entrada seja inválida
 * @param sep           Separador de informação para este ficheiro
 */
void parse_artist(char *buffer, Artists artists, FILE *error_file, char *sep);

#endif