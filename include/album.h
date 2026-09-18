/**
 * @file album.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef ALBUM_H
#define ALBUM_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lint.h"
#include "duration.h"
#include "artist_manager.h"

typedef struct album *Album;
typedef struct albums *Albums;

/**
 * @brief Função responsável por criar e atribuir cada parametro do album
 * 
 * @param id        Id do album
 * @param title     Titulo do album
 * @param artist_id Lista de artistas autores do album
 * @param year      Ano de lançamento do album
 * @param producer  Produtor do album
 * @param artists   Catalogo de artistas
 * 
 * A função create_album para além de criar um albúm também invoca a função update_album_count
 * que vai armazenar nos artistas informação necessária para a query1
 * 
 * @return Album 
 */
Album create_album(unsigned int id, const char *title, LInt artist_id, unsigned short year, const char *producer, Artists artists);

/**
 * @brief Função responsável por libertar a memória alocada para cada album
 * 
 * @param album_ptr 
 */
void free_album(void *album_ptr);

/**
 * @brief Função que imprime um album
 * 
 * @param album_ptr 
 */
void print_album(void *album_ptr);

/**
 * @brief Função que recebe a informação de um album e faz parse do mesmo
 * 
 * @param buffer      Informação recebida
 * @param albums      Catálogo de albums
 * @param error_file  Ficheiro para caso seja uma entrada inválida
 * @param sep         Separador de informação para este ficheiro
 * @param artists     Catálogo de artistas
 */
void parse_album(char *buffer, Albums albums, FILE *error_file, char *sep, Artists artists);



#endif