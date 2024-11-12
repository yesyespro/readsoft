MAIN=bin/main.o
INTERFACE=bin/interface.o
BIN=bin/readsoft
COM=gcc

all:
	./gerar_obj.sh
	@$(COM) $(MAIN) $(INTERFACE) -o $(BIN) -lncurses
	@rm -rf $(MAIN) $(INTERFACE)

	@echo "Sucesso..."
debug:
	./gerar_obj.sh
	@$(COM) -g $(MAIN) $(INTERFACE) -o $(BIN) -lncurses
	@rm  -rf $(MAIN) $(INTERFACE)[
	@echo "Sucesso..."
