/**
 * @file user_manager.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef USERm_H
#define USERm_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "user.h"
#include "hashtable.h"
#include "song_manager.h"

typedef struct users *Users;

/**
 * @brief Função responsável por inicializar o catálogo de users
 * 
 * @param num_buckets Número de buckets com que a hashtable é criada
 * @return Users 
 */
Users init_user_catalog(int num_buckets);

/**
 * @brief Função responsável por inserir um user no catálogo
 * 
 * @param catalog   Atual catálogo de users
 * @param new_user  User que irá ser inserido
 * @param username  Id do user a ser inserido
 * @return true     O user foi inserido com sucesso
 * @return false    Não foi possível inserir o user
 */
bool insert_user(Users catalog, User new_user, unsigned int username);

/**
 * @brief Função que verifica se um user existe no catálogo
 * 
 * @param catalog Atual catálogo de users
 * @param id      Id de procura
 * @return true   O user existe no catálogo
 * @return false  O user não existe no catálogo
 */
bool check_if_user_exists(Users catalog, unsigned int id);

/**
 * @brief Função que devolve um user do catálogo caso este exista
 * 
 * @param catalog   Atual catálogo de users
 * @param username  Id de procura
 * @return User     
 */
User get_user(Users catalog, unsigned int username);

/**
 * @brief Função que devolve o número de users no catálogo
 * 
 * @param catalog   Atual catálogo de users
 * @return int 
 */
int get_nr_users(Users catalog);

/**
 * @brief Função que liberta a memória alocada para o catálogo de users
 * 
 * @param catalog 
 */
void destroy_user_catalog(Users catalog);

/**
 * @brief Função que imprime o catálogo de users
 * 
 * @param catalog 
 */
void print_user_catalog(Users catalog);

/**
 * @brief Função que calcula a idade dos users do catálogo
 * 
 * @param users 
 */
void calculate_users_age(Users users);

/**
 * @brief Função responsável por iterar a tabela de users e acumular os likes dos users
 * dentro do intervalo de idades
 * 
 * @param users     Catálogo de users
 * @param songs     Catálogo de songs
 * @param min_age   Idade mínima
 * @param max_age   Idade máxima
 */
void iterate_users_stat3(Users users, Songs songs, int min_age, int max_age);

#endif