/**
 * @file statistics.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */
#ifndef STATS_H
#define STATS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "artist.h"
#include "user_manager.h"
#include "artist_manager.h"
#include "song_manager.h"
#include "utils.h"
#include "history_manager.h"


//////////////////////Query2///////////////////////

typedef struct artist_discography Artist_Discography;
typedef struct stat_2 *Stat_2;

/**
 * @brief Função responsável por inicializar as estatisticas para query2
 * 
 * @param catalog Catálogo de artistas
 * @return Stat_2 
 */
Stat_2 init_stat_2(Artists catalog);

/**
 * @brief Função responsável por libertar a memória alocada para a stat_2
 * 
 * @param stat 
 */
void destroy_stat_2(Stat_2 stat);

/**
 * @brief Função que imprime a stat_2
 * 
 * @param stat 
 */
void print_stat_2(Stat_2 stat);

/**
 * @brief Função que organiza a stat_2
 * 
 * @param stat 
 */
void sort_stat_2(Stat_2 stat);

/**
 * @brief Função resposável por preencher o stat_2
 * 
 * @param stat     Estatisticas para a query2
 * @param songs    Catálogo de songs
 * @param artists  Catálogo de artistas
 * 
 * A função irá fazer uma iteração do catálogo de songs e de artistas armazenando os 
 * dados necessários na stat_2
 * 
 */
void exec_stat_2(Stat_2 stat, Songs songs, Artists artists);

/**
 * @brief Função para adicionar ou atualizar um artista
 * 
 * @param id        Id do artista
 * @param name      Nome do artista
 * @param duration  Discografia total
 * @param country   Pais do artista
 * @param group     Grupo ou individual
 * @param stat      Estatisticas para a query2
 */
void add_to_stat_2(int id, char *name, int duration, char *country, bool group, Stat_2 stat);

/**
 * @brief Função que gera o resultado para a query2 caso esta não tenha filtro
 * 
 * @param stat      Estatisticas para a query2
 * @param topN      Número pedido na query2
 * @param sep       Separador do output da query2
 * @return char*    Resultado
 */
char *query2_nofilter_result(Stat_2 stat, unsigned int topN, char *sep);

/**
 * @brief Função que gera o resultado para a query2 caso esta tenha filtro
 * 
 * @param stat      Estatisticas para a query2
 * @param topN      Número pedido na query2
 * @param country   País de filtro
 * @param sep       Separador do output da query2
 * @return char*    Resultado
 */
char *query2_filter_result(Stat_2 stat, unsigned int topN, char *country, char *sep);


////////////////Query3/////////////////////////

typedef struct genre_likes Genre_Likes;
typedef struct stat_3 *Stat_3;

/**
 * @brief Função responsável por inicializar a estatisticas para a query3
 * 
 * @param capacity 
 * @return Stat_3 
 */
Stat_3 init_stat_3(unsigned int capacity);

/**
 * @brief Função que liberta a memória alocada para a stat_3
 * 
 * @param stat 
 */
void destroy_stat_3(Stat_3 stat);

/**
 * @brief Função que imprime a stat_3
 * 
 * @param stat 
 */
void print_stat_3(Stat_3 stat);

/**
 * @brief Função que organiza os gêneros
 * 
 * @param stat 
 */
void sort_genres(Stat_3 stat);

/**
 * @brief Função responsável por iterar a tabela de users e songs para obter os dados para a query3
 * 
 * @param stat      Estatisticas query3
 * @param songs     Catálogo de songs
 * @param users     Catálogo de users
 * @param min_age   Idade mínima
 * @param max_age   Idade máxima
 */
void exec_stat_3(Stat_3 stat, Songs songs, Users users, int min_age, int max_age);

/**
 * @brief Função responsável por restaurar a stat_3
 * 
 * @param stat 
 */
void reset_stat_3(Stat_3 stat);

/**
 * @brief Função que gera o resultado para a query3
 * 
 * @param stat          Estatisticas query3
 * @param separador_q   Separador do output da query3
 * @return char*        Resultado
 */
char *query3_result(Stat_3 stat, char *separador_q);

/**
 * @brief Função que adiciona ou atualiza o total de likes para um gênero específico
 * 
 * @param genre Gênero de música
 * @param likes Likes a serem adicionados ao gênero
 * @param stat  Estatisticas para a query3
 */
void add_likes_to_genre(char *genre, int likes, Stat_3 stat);


/////////////////Query4///////////////////////////

typedef struct cell *Cell;
typedef struct stat_4 *Stat_4;


/**
 * @brief Função responsável por inicializar as estatisticas para a query4
 * 
 * @return Stat_4 
 */
Stat_4 init_stat_4();

/**
 * @brief Função que imprime a stat_4
 * 
 * @param stat 
 */
void print_stat_4(Stat_4 stat);

/**
 * @brief Função que liberta a memória alocada para a stat_4
 * 
 * @param stat 
 */
void destroy_stat_4(Stat_4 stat);

/**
 * @brief Função que insere os dados na stat_4
 * 
 * @param songs     Catálogo de songs
 * @param stat      Estatisticas para a query4
 * @param song_id   Id da song
 * @param date      Data
 * @param duration  Duração
 */
void insert_stat_4(Songs songs, Stat_4 stat, unsigned int song_id, char *date, char *duration);

/**
 * @brief Função responsável por obter os dados para a query4
 * 
 * @param stat 
 */
void exec_stat_4(Stat_4 stat);

/**
 * @brief Devolve o id caso a query4 não tenha filtro
 * 
 * @param stat 
 * @return int 
 * 
 * Devolve o id do artista que esteve mais vezes no top10
 * 
 */
int get_res_no_filter_id(Stat_4 stat);

/**
 * @brief Devolve a quantidade de vezes que esteve no top10 caso a query4 não tenha filtro
 * 
 * @param stat 
 * @return int 
 * 
 * Devolve a quantidade de vezes que o artista com mais presenças no top10 fez parte do 
 * mesmo
 * 
 */
int get_res_no_filter_count(Stat_4 stat);

/**
 * @brief Devolve o id caso a query4 tenha filtro
 * 
 * @param stat 
 * @param date1 
 * @param date2 
 * @return int 
 * 
 * Devolve o id do artista que esteve mais vezes no top10 durante um período de tempo
 * 
 */
int get_res_filter_id(Stat_4 stat, char *date1, char *date2);

/**
 * @brief Devolve a quantidade de vezes que esteve no top10 caso a query4 tenha filtro
 * 
 * @param stat 
 * @param date1 
 * @param date2 
 * @return int 
 * 
 * Devolve a quantidade de vezes que o artista com mais presenças no top10 fez parte 
 * mesmo durante um perído de tempo
 * 
 */
int get_res_filter_count(Stat_4 stat, char *date1, char *date2);


///////////////////////////Query5////////////////////7

typedef struct stat_5 *Stat_5;

/**
 * @brief Função que inicializa as estatisticas para a query5
 * 
 * @return Stat_5 
 */
Stat_5 init_stat_5();

/**
 * @brief Função que liberta a memória alocada para a stat_5
 * 
 * @param stat 
 */
void destroy_stat_5(Stat_5 stat);

/**
 * @brief Função que imprime a stat_5
 * 
 * @param stat 
 * @param max_lines 
 */
void print_stat_5(Stat_5 stat, int max_lines);

/**
 * @brief Função que atuliza os dados da stat_5
 * 
 * @param stat     Estatisticas para a query5
 * @param user_id  Id do user a atualizar
 * @param genre    Genero a atulizar
 */
void update_stat_5(Stat_5 stat, int user_id, const char *genre);

/**
 * @brief Devolve os utilizadores da stat_5 (linhas da matriz)
 * 
 * @param stat 
 * @return char** 
 */
char **stat_5_get_users(Stat_5 stat);

/**
 * @brief Devolve os gêneros da stat_5 (colunas da matriz)
 * 
 * @param stat 
 * @return char** 
 */
char **stat_5_get_genres(Stat_5 stat);

/**
 * @brief Devolve a matriz da stat_5
 * 
 * @param stat 
 * @return int** 
 */
int **stat_5_get_matriz(Stat_5 stat);

/**
 * @brief Função que liberta a memória alocada para os gêneros da stat_5
 * 
 * @param genres 
 */
void free_stat_5_genres(char **genres);

/**
 * @brief Função que liberta a memória alocada para os users da stat_5
 * 
 * @param users 
 */
void free_stat_5_users(char **users);

/**
 * @brief Função que liberta a memória alocada para a matriz da stat_5
 * 
 * @param matriz 
 */
void free_stat_5_matriz(int **matriz);


///////////////////////Query 6//////////////////

typedef struct genre_cell *Genre_Cell;
typedef struct artist_cell *Artist_Cell;
typedef struct user_cell *User_Cell;
typedef struct stat_6 *Stat_6;
typedef struct histories *Histories;

/**
 * @brief Função responsável por inicializar as estatisticas para a query6
 * 
 * @return Stat_6 
 */
Stat_6 init_stat_6();

/**
 * @brief Função responsável por reinicializar as estatisticas para a query6
 * 
 * @param stat 
 */
void reset_stat_6(Stat_6 stat);

/**
 * @brief Função responsável por libertar a memória alocada para a stat_6
 * 
 * @param stat 
 */
void destroy_stat_6(Stat_6 stat);

/**
 * @brief Função que imprime a stat_6
 * 
 * @param stat 
 * @param n 
 */
void print_stat_6(Stat_6 stat, int n);

/**
 * @brief Devolve o id do utilizador
 * 
 * @param stat 
 * @return int 
 */
int get_stat_6_id(Stat_6 stat);

/**
 * @brief Devolve o ano da stat_6
 * 
 * @param stat 
 * @return int 
 */
int get_stat_6_year(Stat_6 stat);

/**
 * @brief Função responsável por obter os dados necessários para a query6
 * 
 * @param stat      Estatisticas para a query6
 * @param histories Catálogo de histories
 * @param songs     Catálogo de songs
 * @param user_id   Id do user
 * @param year      
 * @param n 
 */
void exec_stat_6(Stat_6 stat, Histories histories, Songs songs, unsigned int user_id, unsigned int year, int n);

/**
 * @brief Função responsável por armanezar os dados relativos à query6
 * 
 * @param stat      Estatisticas para a query6
 * @param songs     Catálogo de songs
 * @param song_id   Id da song
 * @param date      Data
 * @param time      Tempo
 * @param duration  Duração
 */
void add_data_stat_6(Stat_6 stat, Songs songs, unsigned int song_id, Date date, Time time, Time duration);

/**
 * @brief Função que devolve o resultado para a query6 sem filtro
 * 
 * @param stat      Estatisticas para a query6
 * @param sep_q     Separador do output da query6
 * @return char*    Resultado
 */
char *query_6_res(Stat_6 stat, char *sep_q);

/**
 * @brief Função que devolve o resultado para a query6 com filtro
 * 
 * @param stat      Estatisticas para a query6
 * @param sep_p     Separador do output da query6
 * @return char*    Resultado
 */
char *query_6_res_filter(Stat_6 stat, char *sep_p);

/**
 * @brief Função de comparação para qsort
 * 
 * @param a     Artista a
 * @param b     Artista b
 * @return int 
 */
int compareArtists(const void *a, const void *b);

#endif