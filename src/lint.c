#include "lint.h"

struct lint
{
    unsigned int value;
    struct lint *prox;
};

LInt lint_create_node(unsigned int id)
{
    LInt new_node = (LInt)malloc(sizeof(struct lint));
    if (new_node)
    {
        new_node->value = id;
        new_node->prox = NULL;
    }
    return new_node;
}

void lint_insert_head(LInt *head, unsigned int id)
{
    LInt new_node = lint_create_node(id);
    if (new_node)
    {
        new_node->prox = *head;
        *head = new_node;
    }
}

LInt lint_dup_list(LInt head)
{
    LInt current = head;
    if (head == NULL)
    {
        return NULL; // Lista vazia, retorna NULL
    }

    LInt new_head = NULL;
    LInt *new_tail = &new_head;

    while (current != NULL)
    {
        LInt new_node = lint_create_node(current->value);
        if (new_node == NULL)
        {
            // Em caso de falha na alocação de memória
            while (new_head != NULL)
            {
                LInt temp = new_head;
                new_head = new_head->prox;
                free(temp);
            }
            return NULL;
        }

        *new_tail = new_node; // Adiciona o novo nó ao final da nova lista
        new_tail = &((*new_tail)->prox);

        current = current->prox; // Passa para o próximo nó da lista original
    }

    return new_head;
}

void free_lint(LInt list)
{
    while (list)
    {
        LInt temp = list;
        list = list->prox;
        free(temp);
    }
}

unsigned int lint_get_value(LInt node)
{
    if (node != NULL)
    {
        return node->value;
    }
    return 0;
}

LInt lint_get_next(LInt node)
{
    if (node != NULL)
    {
        return node->prox;
    }
    return NULL;
}

void print_lint(LInt node)
{
    bool first = true;  // Variável para verificar se é o primeiro nó
    while (node != NULL)
    {
        if (!first)
        {
            printf(",");  // Imprime a vírgula antes de cada número, exceto no primeiro
        }
        printf("%u", node->value);  // Imprime o valor do nó
        node = node->prox;  // Avança para o próximo nó
        first = false;  // Depois de imprimir o primeiro, marque como false
    }
    printf("\n");  // Imprime uma nova linha após a lista
}