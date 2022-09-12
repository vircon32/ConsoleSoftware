# before building make sure /usr/local/DevTools/ is in your system PATH
CC	= compile
AS	= assemble
PCK	= packrom
PNG	= png2vircon
TIL	= tiled2vircon
WAV	= wav2vircon

# build directory
BUILD_DIR = build

# obj directory
OBJ_DIR	 = obj

# cflags
CFLAGS 	+= -Wall
CFLAGS 	+= -v

# include VirconDefinitions library (not necessary for now)
LIBS	+= /home/chandler/Documentos/ComputerSoftware/VirconDefinitions/build/

.PHONY: all clean

all: vircon32-tic-tac-toe.v32

vircon32-tic-tac-toe.v32:vircon32-tic-tac-toe.xml vircon32-tic-tac-toe.vbin TextureTicTacToe.vtex $(BUILD_DIR)
	$(PCK) $< -o $(BUILD_DIR)/$@

vircon32-tic-tac-toe.vbin:main.asm $(OBJ_DIR)
	$(AS) $(OBJ_DIR)/$< -o $(OBJ_DIR)/$@

main.asm:main.c $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$@

TextureTicTacToe.vtex: $(OBJ_DIR)
	png2vircon TextureTicTacToe.png -o $(OBJ_DIR)/TextureTicTacToe.vtex

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)