/**
 * @file parser_data.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */
#ifndef PARSER_DATA_H
#define PARSER_DATA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>

#include "artist_manager.h"
#include "artist.h"
#include "song_manager.h"
#include "song.h"
#include "user.h"
#include "user_manager.h"
#include "album.h"
#include "album_manager.h"
#include "history.h"
#include "history_manager.h"
#include "duration.h"
#include "lint.h"
#include "utils.h"

/**
 * @brief Função responsável por dividir o parse dos diferentes ficheiros
 * 
 * @param path      Ficheiro a ler
 * @param artists   Catálogo artistas
 * @param songs     Catálogo musicas
 * @param users     Catálogo users
 * @param albums    Catálogo albums
 * @param histories Catálogo histories
 * @param stat_4    Estatisticas query4
 * @param sep       Separador de informação para este tipo de ficheiro
 * @return int      
 */
int parse_data(char *path, Artists artists, Songs songs, Users users, Albums albums, Histories histories, Stat_4 stat_4, Stat_5 stat_5, char *sep);

#endif