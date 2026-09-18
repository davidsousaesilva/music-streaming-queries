/**
 * @file queries.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef QUERIES_H
#define QUERIES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "user_manager.h"
#include "user.h"
#include "user_manager.h"
#include "utils.h"
#include "statistics.h"
#include "../recomendador/recomendador.h"


/**
 * @brief Função que irá resolver a query1
 * 
 * @param queryArgs    Input recebido
 * @param users        Catálogo users
 * @param artists      Catálogo artistas
 * @param separador_q  Separador do output
 * @param n            Número da query
 * @return char*       Resultado
 * 
 * Uma vez que todos os dados necessários para a query1 são obtidos durante o parse,
 * a função irá apenas verificar se o input recebido é de um artista ou um utilizador.
 * Conforme o tipo recebido irá buscar a informação necessária à respetiva estrutura utilizando
 * funções auxiliares e devolve o resultado.
 * 
 */
char *query1(char *queryArgs, Users users, Artists artists, char *separador_q, unsigned int n);

/**
 * @brief Função que irá resolver a query2
 * 
 * @param queryArgs    Input recebido
 * @param artists      Catálogo artistas
 * @param songs        Catálogo musicas
 * @param stat_2       Estatisticas query2
 * @param separador_q  Separador do output
 * @param n            Número da query
 * @return char*       Resultado
 * 
 * A função irá verificar qual o tipo de query2 que está a resolver. Caso seja
 * um input com filtro irá invocar a função query2_filter_res.
 * Caso contrário irá invocar a função query2_nofilter_result.
 * 
 */
char *query2(char *queryArgs, Artists artists, Songs songs, Stat_2 stat_2, char *separador_q, unsigned int n);

/**
 * @brief Função que irá resolver a query3
 * 
 * @param queryArgs    Input recebido
 * @param users        Catálogo users
 * @param songs        Catálogo songs
 * @param stat_3       Estatisticas query3
 * @param separador_q  Separador do output
 * @param n            Número da query
 * @return char*       Resultado
 *
 * A função irá inicialmente invocar a função reset_all_song_likes para evitar corrupção de dados.
 * Posteriormente invoca exec_stat_3 que irá preencher a stat_3 com as estatisticas necessárias.
 * Finalmente invoca query3_result que irá devolver o resultado. 
 * 
 */
char *query3(char *queryArgs, Users users, Songs songs, Stat_3 stat_3, char *separador_q, unsigned int n);

/**
 * @brief Função que irá resolver a query4
 * 
 * @param queryArgs    Input recebido
 * @param artists      Catálogo artistas
 * @param songs        Catálogo songs
 * @param histories    Catálogo histories
 * @param stat_4       Estatisticas query4
 * @param separador_q  Separador do output
 * @param n            Número da query
 * @return char*       Resultado
 * 
 * A função irá verificar se se trate de uma query4 com filtro ou sem filtro-
 * Caso seja um input com filtro irá invocar as funções get_res_filter_id e
 * get_res_filter_count e escreve o resultado.
 * Caso seja um input sem filtro irá invocar as funções get_res_no_filter_id
 * e get_res_no_filter_count e escreve o resultado.
 * No final retorna o resultado
 * 
 */
char *query4(char *queryArgs, Artists artists, Songs songs, Histories histories, Stat_4 stat_4, char *separador_q, unsigned int n);

/**
 * @brief Função que irá resolver a query5
 * 
 * @param queryArgs    Input recebido
 * @param users        Catálogo de users
 * @param stat_5       Estatisticas query5
 * @param sep_q        Separador do output
 * @param n            Número da query
 * @return char*       Resultado
 * 
 * Uma vez que a matriz já foi construída aquando do parse das histories, 
 * neste momento é apenas necessário invocar a função recomendaUtilizadores, 
 * escrever o resultado e no final devolvê-lo
 * 
 */
char *query5(char *queryArgs, Users users, Stat_5 stat_5, char *sep_q, int n);   

/**
 * @brief Função que irá resolver a query6
 * 
 * @param queryArgs    Input recebido
 * @param stat_6       Estatisticas query6
 * @param histories    Catálogo histories
 * @param songs        Catálogo songs
 * @param sep_q        Separador do output
 * @param n            Número da query
 * @return char*       Resultado
 * 
 * Invoca a funçãoo exec_stat_6 para realizar o processamento dos dados recebidos
 * Invoca as funções query6_res e query6_res_filter para obter os resultados
 * Invoca a função reset_stat_6 para recompor as estatisticas para a query6
 * Retorna o resultado.
 * 
 */
char *query6(char *queryArgs, Stat_6 stat_6, Histories histories, Songs songs, char *sep_q, int n);

#endif