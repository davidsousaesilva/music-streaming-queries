/**
 * @file date.h
 * @author David Silva, João Aleixo, João Costa 
 *  
 */


#ifndef DATE_H
#define DATE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct date *Date;

/**
 * @brief Função que cria um nova data
 * 
 * @param day   Dia
 * @param month Mês
 * @param year  Ano
 * @return Date Data criada
 */
Date create_date(unsigned char day, unsigned char month, unsigned short year);

/**
 * @brief Função que duplica uma data
 * 
 * @param ori_date Data original
 * @return Date    Data copiada
 * 
 * Em algumas funções poderá ser necessária trabalhar sobre datas portanto é mais seguro
 * trabalhar sobre uma cópia
 */
Date dup_date(Date ori_date);

/**
 * @brief Função responsável por libertar a memória alocada para uma data
 * 
 * @param data Data a ser libertada
 */
void free_date(Date data);

/**
 * @brief Função que imprime uma data
 * 
 * @param d Data a imprimir
 */
void print_date(Date d);

/**
 * @brief Função que devolve o dia de uma data
 * 
 * @param date 
 * @return unsigned char Dia devolvido
 */
unsigned char get_day(Date date);

/**
 * @brief Função que devolve o mês de uma data
 * 
 * @param date 
 * @return unsigned char Mẽs devolvido
 */
unsigned char get_month(Date date);

/**
 * @brief Função que devolve o ano de uma data
 * 
 * @param date 
 * @return unsigned short Ano devolvido
 */
unsigned short get_year(Date date);

#endif