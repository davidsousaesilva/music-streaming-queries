/**
 * @file utils.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "artist_manager.h"
#include "song_manager.h"
#include "song.h"
#include "date.h"
#include "duration.h"
#include "artist.h"
#include "user_manager.h"

/**
 * @brief Função responsável por fazer o parse de uma lista de ids
 * 
 * @param string Informação recebida
 * @return LInt 
 */
LInt parse_id_list(char *string);

/**
 * @brief Função para validar se uma duração(de uma song) é válida
 * 
 * @param duration 
 * @return true  Duração válida
 * @return false Duração inválida
 */
bool is_valid_duration(const char *duration);

/**
 * @brief Função para validar se um email(de um user) é válido
 * 
 * @param email 
 * @return true     Email válido
 * @return false    Email inválido
 */
bool is_valid_email(const char *email);

/**
 * @brief Função para validar se uma subscrição é válida
 * 
 * @param type 
 * @return true     Subscrição válida
 * @return false    Subscrição inválida
 */
bool is_valid_subscription(const char *type);

/**
 * @brief Função para validar se uma data é válida
 * 
 * @param date 
 * @return true     Data válida
 * @return false    Data inválida
 */
bool is_valid_date(Date date);

/**
 * @brief Função que imprime um número em segundos no formato hora:minutos:segundos
 * 
 * @param total_seconds 
 */
void print_sec_to_time_format(unsigned int total_seconds);

/**
 * @brief Função que recebe um tempo em segundos e retorna uma string no formato 
 * hora:minutos:segundos
 * 
 * @param total_seconds 
 * @return char* 
 */
char *get_sec_to_time_format(unsigned int total_seconds);

/**
 * @brief Função que remove as aspas de uma string
 * 
 * @param str 
 * @return char* 
 */
char *remove_quotes(char *str);

/**
 * @brief Função responsável por fazer parse da duração de uma música
 * 
 * @param duration_str 
 * @return Time 
 */
Time parse_time(const char *duration_str);

/**
 * @brief Função responsável por fazer parse de uma data
 * 
 * @param birthdate_str 
 * @return Date 
 */
Date parse_date(const char *birthdate_str);

/**
 * @brief Função responsável por fazer parse da data de nascimento de um user
 * 
 * @param birth_date 
 * @return Date 
 */
Date parse_user_date(const char *birth_date);

/**
 * @brief Função responsável por limpar a string duration
 * 
 * @param str 
 */
void clean_string(char *str);

/**
 * @brief Função que converte uma string no formato hora:minutos:segundos em segundos
 * 
 * @param time_str 
 * @return int 
 */
int time_to_seconds(const char *time_str);

/**
 * @brief Função para calcular qual a semana a partir da data
 * 
 * @param date 
 * @return int 
 */
int calculate_week(const char *date);

/**
 * @brief Função para calcular o dia do ano
 * 
 * @param year 
 * @param month 
 * @param day 
 * @return int 
 */
int day_of_year(int year, int month, int day);

/**
 * @brief Função que recebe um total de segundos e converte numa string hora:minutos:segundos
 * 
 * @param totalSeconds 
 * @return char* 
 */
char *sec_to_string(int totalSeconds);

/**
 * @brief Função para calcular a data a partir do ano e número de dias
 * 
 * @param year 
 * @param days_passed 
 * @return char* 
 */
char *calculate_date(int year, int days_passed);

#endif
