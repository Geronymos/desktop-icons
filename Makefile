
all:
	gcc `pkg-config --cflags gtk4` -o dicons dicons.c `pkg-config --libs gtk4`
clangd:
	bear -- gcc `pkg-config --cflags gtk4` -o dicons dicons.c `pkg-config --libs gtk4`

