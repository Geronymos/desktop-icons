# desktop-icons

> Show Files from a Directory on the Desktop

## Features

- [X] Show content from a folder as Icons on the Desktop
- [X] Use xdg_user_dir to use the default desktop directory
- [X] Start default application for the active file
- [X] Update the Icons on changes to the directory (added/removed files)
- [X] Drag and Drop Files from/to the Desktop
- [ ] Multi-Monitor Support
- [ ] Thumbnails for Images/Documents
- [ ] Sort Rows of List Store based on Name/Type/Date
- [ ] Context-Menu for creating files

## Supported Desktops

> Quoted from [gtk-layer-shell](https://github.com/wmww/gtk-layer-shell)

This application only works on Wayland, and only on Wayland compositors that
support the Layer Shell protocol. Layer shell **is supported** on:
- wlroots based compositors (such as **Sway**)
- Mir-based compositors (some may not enable the protocol by default and require
  `--add-wayland-extension zwlr_layer_shell_v1`)

Layer shell **is not supported** on:
- Gnome-on-Wayland
- Any X11 desktop

## Getting Started

### Dependencies

- gtk+-3.0
- gtk-layer-shell

Arch
```sh
sudo pacman -S gtk3 gtk-layer-shell
```

### Installation

```sh
git clone https://github.com/Geronymos/desktop-icons
cd desktop-icons
make
sudo make install
```

Uninstall
```sh
sudo make uninstall
```

### Development

Dependencies
- bear

To have warnings and autocompletion with clangd in Vim you can generate the
`compile-commands.json` with
```sh
make clangd
```

## License

This project is licensed under the GPL-3 License - see the `LICENSE` file for details

