/**
 * @file song.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef SONG_H
#define SONG_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lint.h"
#include "duration.h"
#include "artist_manager.h"
#include "statistics.h"

typedef struct artists *Artists;
typedef struct song *Song;
typedef struct albums *Albums;

/**
 * @brief Função responsável por criar e atribuir cada parametro da song
 * 
 * @param id        Id da song
 * @param name      Titulo da song
 * @param artist_id Lista de artistas autores da song
 * @param album_id  Album da song
 * @param duration  Duração da song
 * @param genre     Gênero da song
 * @param year      Ano de lançamento
 * @param lyrics    Letra 
 * @return Song 
 */
Song create_song(unsigned int id, const char *name, LInt artist_id, int album_id,
                 Time duration, const char *genre, unsigned short year,
                 const char *lyrics);

/**
 * @brief Função que duplica uma song
 * 
 * @param ori_song 
 * @return Song 
 * 
 * Para permitir a segurança dos dados, quando é necessário trabalhar sobre uma song 
 * é mais seguro utilizar uma cópia desta, daí a necessidade de existir esta função
 */
Song dup_song(Song ori_song);

/**
 * @brief Função que liberta a memória alocada para uma song
 * 
 * @param song_ptr 
 */
void free_song(void *song_ptr);

/**
 * @brief Função que imprime uma song
 * 
 * @param song_ptr 
 */
void print_song(void *song_ptr);

/**
 * @brief Função responsável por calcular a duração de uma song e aumentar a discografia de uma artista
 * 
 * @param song_ptr Apontador da música
 * @param ptr      Catálogo de artistas
 */
void calculate_duration_and_acc(void *song_ptr, void *ptr);

/**
 * @brief Função que devolve o id de uma song
 * 
 * @param song 
 * @return unsigned int 
 */
unsigned int get_song_id(Song song);

/**
 * @brief Função que devolve os likes de uma song
 * 
 * @param song 
 * @return int 
 */
int get_song_likes(Song song);

/**
 * @brief Função que devolve o gênero de uma song
 * 
 * @param song 
 * @return char* 
 */
char *get_genre(Song song);

/**
 * @brief Função que devolve o id do album de uma song
 * 
 * @param song 
 * @return unsigned int 
 */
unsigned int get_album_id(Song song);

/**
 * @brief Função que incrementa o número de likes de uma song
 * 
 * @param song 
 */
void increment_song_likes(Song song);

/**
 * @brief Função que torna os likes de uma song em 0
 * 
 * @param song_ptr 
 * @param context 
 */
void reset_song_likes(void *song_ptr, void *context);

/**
 * @brief Função que devolve a lista de autores de uma song
 * 
 * @param song 
 * @return LInt 
 */
LInt get_author(Song song);

/**
 * @brief Função que incrementa os likes de um gênero
 * 
 * @param song_ptr Apontador da song
 * @param context  Estatisticas query3
 */
void accumulate_genre_likes(void *song_ptr, void *context);

/**
 * @brief Função que recebe a informação de uma song e faz parse do mesmo
 * 
 * @param buffer     Informação recebida
 * @param catalog    Catálogo de songs
 * @param artists    Catálogo de artistas
 * @param albums     Catálogo de albums
 * @param error_file Ficheiro caso entrada seja inválida
 * @param sep        Separador de informação para este tipo de ficheiro
 */
void parse_song(char *buffer, Songs catalog, Artists artists, Albums albums, FILE *error_file, char *sep);

#endif