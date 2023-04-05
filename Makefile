
all:
	gcc `pkg-config --cflags gtk+-3.0 gtk-layer-shell-0` -o dicons dicons.c `pkg-config --libs gtk+-3.0 gtk-layer-shell-0`
clangd:
	bear -- gcc `pkg-config --cflags gtk+-3.0 gtk-layer-shell-0` -o dicons dicons.c `pkg-config --libs gtk+-3.0 gtk-layer-shell-0`

