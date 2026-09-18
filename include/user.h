/**
 * @file user.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lint.h"
#include "date.h"

typedef struct user *User;
typedef struct users *Users;
typedef struct songs *Songs;

/**
 * @brief Função responsável por criar e atribuir cada parametro do user
 * 
 * @param username      Id do user
 * @param email         Email do user
 * @param first_name    Primeiro nome do user
 * @param last_name     Ultimo nome do user
 * @param birth_date    Dia de nascimento do user
 * @param country       Pais do user
 * @param premium_acc   Estado da conta(premium ou normal)
 * @param liked_m_id    Lista de músicas de que gosta
 * @return User 
 */
User create_user(unsigned int username, const char *email, const char *first_name,
                 const char *last_name, Date birth_date, const char *country,
                 bool premium_acc, LInt liked_m_id);

/**
 * @brief Função que duplica um user
 * 
 * @param ori_user 
 * @return User 
 * 
 * Para permitir segurança dos dados, quando é necessário trabalhar sobre um user
 * é mais seguro utilizar uma cópia deste, daí a necessidade de criar esta função
 * 
 */
User dup_user(User ori_user);

/**
 * @brief Função responsável por libertar a memória alocada para o user
 * 
 * @param user_ptr 
 */
void free_user(void *user_ptr);

/**
 * @brief Função que imprime um user
 * 
 * @param user_ptr 
 */
void print_user(void *user_ptr);

/**
 * @brief Função que devolve o email do user
 * 
 * @param user 
 * @return char* 
 */
char *get_email(User user);

/**
 * @brief Função que devolve o primeiro nome do user
 * 
 * @param user 
 * @return char* 
 */
char *get_fst_name(User user);

/**
 * @brief Função que devolve o ultimo nome do user
 * 
 * @param user 
 * @return char* 
 */
char *get_last_name(User user);

/**
 * @brief Função que devolve o país do user
 * 
 * @param user 
 * @return char* 
 */
char *get_country(User user);

/**
 * @brief Função que devolve a data de nascimento do user
 * 
 * @param user 
 * @return Date 
 */
Date get_birth_date(User user);

/**
 * @brief Função que calcula a idade de um user
 * 
 * @param birth_date Data de nascimento do user
 * @return int 
 */
int calculate_age(Date birth_date);

/**
 * @brief Função que devolve a idade de um user
 * 
 * @param user 
 * @return int 
 */
int get_age(User user);

/**
 * @brief Função que devolve a lista de músicas que um utilizador gosta
 * 
 * @param user 
 * @return LInt 
 */
LInt get_liked_songs(User user);

/**
 * @brief Função que atuliza a idade de um user
 * 
 * @param user_ptr Apontador do user
 * @param context  NULL
 */
void update_age_for_user(void *user_ptr, void *context);

/**
 * @brief Função que recebe a informação de um user e faz parse do mesmo
 * 
 * @param buffer        Informação recebida
 * @param users         Catalogo de users
 * @param catalog       Catálogo de songs
 * @param error_file    Ficheiro caso entrada seja inválida
 * @param sep           Separador de informação para este ficheiro
 */
void parse_user(char *buffer, Users users, Songs catalog, FILE *error_file, char *sep);

#endif