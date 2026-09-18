/**
 * @file hashtable.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "artist.h"
#include "song.h"
#include "user.h"
#include "album.h"
#include "history.h"

typedef struct hash_node *HashNode;
typedef struct hashtable *Hashtable;


/**
 * @brief Função que irá iniciar a hashtable
 * 
 * @param num_buckets Numero buckets da hashtable
 * @return Hashtable 
 */
Hashtable hashtable_init(int num_buckets);

/**
 * @brief Função que insere um elemento na hashtable
 * 
 * @param table   
 * @param key     
 * @param value 
 * @return true  Inserido com sucesso
 * @return false Ocorreu um erro
 */
bool hashtable_insert(Hashtable table, unsigned int key, void *value);

/**
 * @brief Função responsável por libertar a memória alocada para a hashtable
 * 
 * @param table 
 * @param free_func Função para libertar a memória do seu conteudo
 */
void hashtable_destroy(Hashtable table, void (*free_func)(void *));

/**
 * @brief Função para procurar e devolver um elemento na hashtable
 * 
 * @param table 
 * @param key 
 * @return void* 
 */
void *hashtable_lookup(Hashtable table, unsigned int key);

/**
 * @brief Função para iterar a tabela de hash com apenas um context
 * 
 * @param table 
 * @param func 
 * @param context 
 */
void hashtable_iterate(Hashtable table, void (*func)(void *value, void *context), void *context);

/**
 * @brief Função alternativa para iterar a tabela de hash com dois context
 * 
 * @param table 
 * @param func 
 * @param context1 
 * @param context2 
 */
void hashtable_iterate2(Hashtable table, void (*func)(void *value, void *context1, void *context2), void *context1, void *context2);

/**
 * @brief Função alternativa para iterar a tabela de hash com 3 context
 * 
 * @param table 
 * @param func 
 * @param context1 
 * @param context2 
 * @param context3 
 */
void hashtable_iterate3(Hashtable table, void (*func)(void *value, void *context1, void *context2, void *context3), void *context1, void *context2, void *context3);

/**
 * @brief Função para imprimir a tabela de hash
 * 
 * @param table 
 * @param print_func 
 */
void hashtable_print(Hashtable table, void (*print_func)(void *));

#endif