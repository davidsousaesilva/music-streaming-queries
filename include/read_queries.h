/**
 * @file read_queries.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */


#ifndef PARSERq_H
#define PARSERq_H

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
#include "handle_query.h"
#include "statistics.h"

/**
 * @brief Função responsável por ler o ficheiro de inputs
 * 
 * @param path_input   Ficheiro de inputs
 * @param artists      Catálogo artistas
 * @param songs        Catálogo songs
 * @param users        Catálogo users
 * @param albums       Catálogo albums
 * @param histories    Catálogo histories
 * @param stat_2       Estatisticas query2
 * @param stat_3       Estatisticas query3
 * @param stat_4       Estatisticas query4
 * @param stat_5       Estatisticas query5
 * @param stat_6       Estatisticas query6
 * @param SEPARATOR    Separador do ficheiro
 */
void read_queries(char *path_input, Artists artists, Songs songs, Users users, Albums albums, Histories histories, Stat_2 stat_2, Stat_3 stat_3, Stat_4 stat_4, Stat_5 stat_5, Stat_6 stat_6, char *SEPARATOR);

#endif