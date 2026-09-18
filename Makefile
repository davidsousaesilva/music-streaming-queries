CC=gcc
CFLAGS=-O2 -Wall -Iinclude 

SRC_DIR = src
BIN_PRINCIPAL = programa-principal
BIN_TESTES = programa-testes
BIN_INTERATIVO = programa-interativo


SOURCES = $(filter-out $(SRC_DIR)/main_program.c $(SRC_DIR)/test_program.c $(SRC_DIR)/interactive_program.c, $(wildcard $(SRC_DIR)/*.c))

RECOMENDADOR_OBJ = recomendador/recomendador-arm64.o
RECOMENDADOR_HEADER = recomendador/recomendador.h

# Alvo principal que compila ambos os executáveis
all: $(BIN_PRINCIPAL) $(BIN_TESTES) $(BIN_INTERATIVO)

# Compilação do programa principal
$(BIN_PRINCIPAL): $(SOURCES) $(SRC_DIR)/main_program.c $(RECOMENDADOR_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Compilação do programa de testes 
$(BIN_TESTES): $(SOURCES) $(SRC_DIR)/test_program.c $(RECOMENDADOR_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_INTERATIVO): $(SOURCES) $(SRC_DIR)/interactive_program.c $(RECOMENDADOR_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Limpeza dos binários e outros arquivos
clean:
	rm -f $(BIN_PRINCIPAL) $(BIN_TESTES) $(BIN_INTERATIVO) resultados/*
