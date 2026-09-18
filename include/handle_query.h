/**
 * @file handle_query.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */


#ifndef HANDLEq_H
#define HANDLEq_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "artist_manager.h"
#include "artist.h"
#include "song_manager.h"
#include "song.h"
#include "user_manager.h"
#include "user.h"
#include "queries.h"

/**
 * @brief A função irá atribuir uma query à sua respetiva função
 * 
 * @param buffer        Query recebida
 * @param artists       Catálogo de artistas
 * @param songs         Catálogo de músicas
 * @param users         Catálogo de utilizadores
 * @param albums        Catálogo de albums
 * @param histories     Catálogo de histórico
 * @param stat_2        Estatisticas relativas à query2
 * @param stat_3        Estatisticas relativas à query3
 * @param stat_4        Estatisticas relativas à query4
 * @param stat_5        Estatisticas relativas à query5
 * @param stat_6        Estatisticas relativas à query6
 * @param n             Contador de queries
 * @param interact_mode Bool relativo a execução de progama-principal ou interativo
 * 
 * A função handle query irá receber apenas uma linha dos inputs, ou seja recebe uma query.
 * A função irá verificar qual o tipo de query recebido e atribuí-la à função correspondente juntamente com 
 * os dados necessários para a resolver.
 * 
 */
void handle_query(char *buffer, Artists artists, Songs songs, Users users, Albums albums, Histories histories, Stat_2 stat_2, Stat_3 stat_3, Stat_4 stat_4, Stat_5 stat_5, Stat_6 stat_6, unsigned int n, bool interact_mode);

#endif