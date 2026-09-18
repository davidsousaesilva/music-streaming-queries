#include "hashtable.h"

struct hash_node
{
    unsigned int key;
    void *value;
    struct hash_node *next;
};

struct hashtable
{
    HashNode *buckets;
    int size;
};

Hashtable hashtable_init(int num_buckets)
{
    Hashtable table = malloc(sizeof(struct hashtable));
    if (!table)
    {
        return NULL; // Falhou
    }

    table->size = num_buckets;
    table->buckets = malloc(num_buckets * sizeof(HashNode));
    if (!table->buckets)
    {
        free(table); // Liberta a memória alocada para a tabela
        return NULL; // Falhou
    }

    // Iniciar todos os buckets como NULL
    for (int i = 0; i < num_buckets; i++)
    {
        table->buckets[i] = NULL;
    }

    return table; // Retorna o ponteiro para a hashtable criada
}

bool hashtable_insert(Hashtable table, unsigned int key, void *value)
{
    if (!table || !value)
    {
        return false; // Falhou
    }

    // Calcular o índice do bucket
    unsigned int index = key % table->size;

    // Alocar memória para o novo nó
    HashNode new_node = malloc(sizeof(struct hash_node));
    if (!new_node)
    {
        return false; // Falhou
    }

    // Configurar os campos do novo nó
    new_node->key = key;
    new_node->value = value;
    new_node->next = table->buckets[index]; // Inserir no início da lista
    table->buckets[index] = new_node;       // Atualiza o bucket

    return true; // Sucesso
}

void hashtable_destroy(Hashtable table, void (*free_func)(void *))
{
    if (!table)
        return;

    // Percorre todos os buckets da tabela hash
    for (int i = 0; i < table->size; i++)
    {
        HashNode current = table->buckets[i];

        // Percorre a lista encadeada de nós em cada bucket
        while (current)
        {
            HashNode next = current->next; // Guarda o próximo nó

            // Chama a função de callback para libertar o valor armazenado no nó
            if (free_func)
            {
                free_func(current->value);
            }

            // Liberta o nó atual
            free(current);

            current = next; // Vai para o próximo nó
        }
    }

    // Liberta os buckets da tabela hash
    free(table->buckets);

    // Liberta a própria estrutura da tabela hash
    free(table);
}

// Aplica uma funcao argumento a todos os elementos e guarda o resultado numa variavel de controlo
void hashtable_iterate(Hashtable table, void (*func)(void *value, void *context), void *context)
{
    if (!table || !func)
        return; // Verifica se a tabela e a função de callback são válidas

    for (int i = 0; i < table->size; i++)
    {
        HashNode current = table->buckets[i]; // Guarda o primeiro nó da lista ligada no bucket
        while (current)
        {
            func(current->value, context); // Chama a função de callback para o valor do nó atual
            current = current->next;       // Move para o próximo nó na lista
        }
    }
}

void hashtable_iterate2(Hashtable table, void (*func)(void *value, void *context1, void *context2), void *context1, void *context2)
{
    if (!table || !func)
        return; // Verifica se a tabela e a função de callback são válidas

    for (int i = 0; i < table->size; i++)
    {
        HashNode current = table->buckets[i]; // Pega o primeiro nó da lista encadeada no bucket
        while (current)
        {
            func(current->value, context1, context2); // Chama a função de callback com as duas variáveis de contexto
            current = current->next;                  // Move para o próximo nó na lista
        }
    }
}

void hashtable_iterate3(Hashtable table, void (*func)(void *value, void *context1, void *context2, void *context3), void *context1, void *context2, void *context3)
{
    if (!table || !func)
        return; // Verifica se a tabela e a função de callback são válidas

    for (int i = 0; i < table->size; i++)
    {
        HashNode current = table->buckets[i]; // Pega o primeiro nó da lista encadeada no bucket
        while (current)
        {
            func(current->value, context1, context2, context3); // Chama a função de callback com as três variáveis de contexto
            current = current->next;                            // Move para o próximo nó na lista
        }
    }
}

// Usado para debug
void hashtable_print(Hashtable table, void (*print_func)(void *)) // basicamente +- = hashtable_iterate
{
    for (int i = 0; i < table->size; i++)
    {
        HashNode current = table->buckets[i];
        printf("\nBucket %d: ", i);

        if (!current)
        {
            printf("Empty\n");
        }
        else
        {
            while (current)
            {
                print_func(current->value); // Chama a função de impressão
                current = current->next;
            }
            printf(" Empty\n");
        }
    }
}

// Delvolve um elemento se ele existir na tabela
void *hashtable_lookup(Hashtable table, unsigned int key)
{
    if (!table)
        return NULL; // Verifica se a tabela é válida

    // Calcular o índice do bucket onde a chave pode estar
    unsigned int index = key % table->size;

    // Percorrer a lista ligada do bucket
    HashNode current = table->buckets[index];
    while (current)
    {
        if (current->key == key)
        {
            return current->value; // Retorna o valor se a chave for encontrada
        }
        current = current->next; // Move para o próximo nó
    }

    return NULL; // Retorna NULL se a chave não for encontrada
}
