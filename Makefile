all: build/client build/team build/tchat

build/client: obj/main.o obj/clientbrc.o obj/clientTCP.o obj/game.o obj/poketudiant.o obj/error.o obj/color.o
	@mkdir -p build
	gcc -g -o build/client obj/main.o obj/clientbrc.o obj/clientTCP.o obj/game.o obj/poketudiant.o obj/error.o obj/color.o -pthread

build/team: obj/team.o obj/color.o obj/poketudiant.o
	@mkdir -p build
	gcc -g -o build/team obj/team.o obj/color.o obj/poketudiant.o -pthread

build/tchat: obj/tchat.o
	@mkdir -p build
	gcc -g -o build/tchat obj/tchat.o -pthread

obj/main.o: src/main.c
	@mkdir -p obj
	gcc -c src/main.c -o obj/main.o

obj/clientbrc.o: src/clientbrc.c
	@mkdir -p obj
	gcc -c src/clientbrc.c -o obj/clientbrc.o

obj/clientTCP.o: src/clientTCP.c
	@mkdir -p obj
	gcc -c src/clientTCP.c -o obj/clientTCP.o

obj/game.o: src/game.c
	@mkdir -p obj
	gcc -c src/game.c -o obj/game.o

obj/error.o: src/error.c
	@mkdir -p obj
	gcc -c src/error.c -o obj/error.o

obj/color.o: src/color.c
	@mkdir -p obj
	gcc -c src/color.c -o obj/color.o

obj/team.o: src/team.c
	@mkdir -p obj
	gcc -c src/team.c -o obj/team.o

obj/tchat.o: src/tchat.c
	@mkdir -p obj
	gcc -c src/tchat.c -o obj/tchat.o

obj/poketudiant.o: src/poketudiant.c
	@mkdir -p obj
	gcc -c src/poketudiant.c -o obj/poketudiant.o

clean:
	@rm -rf build
	@rm -rf obj