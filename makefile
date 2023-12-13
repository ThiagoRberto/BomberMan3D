####################### Como usar #######################
# No local do arquivo makefile abra o terminal
# se for windows, abra o powershell ou cmd e digite os comandos:
#		mingw32-make.exe -f makefile nome_da_regra
#
# se for linux, abra o terminal e digite o comando:
# 		make nome_da_regra
#
# (Troque nome_da_regra por help por exemplo)
####################### Macros #######################
FLAGSW= -lopengl32 -lglu32 -lfreeglut -lwinmm -lgdi32
FLAGSL= -lglut -lGL -lGLU -lm

D= $(if $(filter $(OS),Windows_NT),\,/)
RM= $(if $(filter $(OS),Windows_NT),del /Q,rm -rf)
FLAGS= $(if $(filter $(OS),Windows_NT),$(FLAGSW), $(FLAGSL))

N= .$(D)build$(D)
SOILC= $(N)SOIL.c  $(N)image_helper.c $(N)image_DXT.c $(N)stb_image_aug.c

DIRHOME= .$(D)src$(D)main$(D)
PATHCharacter=  $(DIRHOME)characters$(D)mainCharacter.c
PATHCamera= $(DIRHOME)settings$(D)modelCamera.c
CC= gcc
#To tests
DIR_TEST= $(DIRHOME)teste$(D)
####################### Objetos #######################
all: testeGame

executeArq: exe.o
	.$(D)exe.o

game: gameArq executeArq

gameArq: $(DIRHOME)MainGame.c $(SOILC)
	$(CC) $^ $(FLAGS) -o exe.o
		@ echo ' '

testeGame: testeGameArq executeArq

testeGameArq: $(DIR_TEST)MainGame.c $(SOILC)
	$(CC) $^ $(FLAGS) -o exe.o
		@ echo ' '

character: characterArq executeArq

characterArq:
	$(CC) $(PATHCharacter) $(FLAGS) -o exe.o	
	@ echo ' '

teste: testeArq executeArq 

testeArq: $(DIR_TEST)mainPerson.c  $(DIR_TEST)modelInteration.c  $(DIR_TEST)modelPerson.c $(PATHCamera)
	$(CC) $^ $(FLAGS) -o exe.o
	@ echo ' '

camera: cameraArq executeArq

cameraArq: $(DIR_TEST)mainCamera.c  $(DIRHOME)modelInteration.c  $(DIR_TEST)modelAmbientSimple.c $(PATHCamera)
	$(CC) $^ $(FLAGS) -o exe.o
	@ echo ' '

two: twoArq executeArq

twoArq: $(DIR_TEST)mainPerson.c  $(DIRHOME)modelInteration.c  $(DIR_TEST)modelPerson.c $(PATHCamera)
	$(CC) $^ $(FLAGS) -o exe.o
	@ echo ' '

clean:
	$(RM) .$(D)*.o
	@ echo "Limpeza realizada"

help:
	@ echo "Se for windows troque 'make' por 'mingw32-make.exe -f makefile'"
	@ echo " "
	@ echo "Funcoes para visualizacao disponiveis:"
	@ echo " "
	@ echo "make 	 		-> O mesmo que make all e make game" 
	@ echo "make game 		-> Ver a integracao do game" 	
	@ echo "make character 		-> Personagem" 
	@ echo "make teste		-> Teste arquivos separados" 
	@ echo "make two		-> Camera e personagem" 
	@ echo "make camera		-> Teste camera "
	@ echo " "
	@ echo "make clean 		-> Limpeza de executaveis" 
	@ echo ' '
