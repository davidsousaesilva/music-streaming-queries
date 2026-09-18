/**
 * @file duration.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */

#ifndef TEMPO_H
#define TEMPO_H

#include <stdio.h>
#include <stdlib.h>

typedef struct time *Time;

/**
 * @brief Função que criar um tempo
 * 
 * @param hours     Horas
 * @param minutes   Minutes
 * @param seconds   Segundos
 * @return Time     Tempo criado
 */
Time create_time(unsigned char hours, unsigned char minutes, unsigned char seconds);

/**
 * @brief Função que duplica um tempo
 * 
 * @param ori_time 
 * @return Time     Tempo duplicada
 * 
 * Em algumas funções poderá ser necessária trabalhar sobre tempos portanto é mais seguro
 * trabalhar sobre uma cópia
 */
Time dup_time(Time ori_time);

/**
 * @brief Função que devolve a hora de um tempo
 * 
 * @param t             Tempo
 * @return unsigned int Hora
 */
unsigned int get_hour(Time t);

/**
 * @brief Função que liberta a memória alocada para um tempo
 * 
 * @param t 
 */
void free_time(struct time *t);

/**
 * @brief Função que imprime um tempo
 * 
 * @param t 
 */
void print_time(struct time *t);

/**
 * @brief Calcula o tempo em segundos
 * 
 * @param t             Tempo
 * @return unsigned int Tempo em segundos
 */
unsigned int calculate_duration(Time t);

#endif