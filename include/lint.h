/**
 * @file lint.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef LINT_H
#define LINT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct lint *LInt;

/**
 * @brief Função responsável por criar um nodo da lista ligada
 * 
 * @param id    Value do nodo
 * @return LInt 
 */
LInt lint_create_node(unsigned int id);

/**
 * @brief Função que duplica a lista
 * 
 * @param head  Inicio da lista
 * @return LInt 
 * 
 * Devido à necessidade de trabalhar com estes dados é mais seguro utilizar uma cópia dos mesmos
 */
LInt lint_dup_list(LInt head);

/**
 * @brief Função responsável por libertar a memória alocada para a lista
 * 
 * @param list 
 */
void free_lint(LInt list);

/**
 * @brief Função que adiciona um novo nodo à lista
 * 
 * @param head Inicio da lista
 * @param id   Value do novo nodo
 */
void lint_insert_head(LInt *head, unsigned int id);

/**
 * @brief Função que retorna o value de um nodo da lista
 * 
 * @param node 
 * @return unsigned int 
 */
unsigned int lint_get_value(LInt node);

/**
 * @brief Função que retorna o próximo nodo da lista
 * 
 * @param node 
 * @return LInt 
 */
LInt lint_get_next(LInt node);

/**
 * @brief Função que imprime a lista
 * 
 * @param node 
 */
void print_lint(LInt node);

#endif