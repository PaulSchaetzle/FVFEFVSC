# FVFEFVSC
A simple Gtk Text Editor that just works :tm:
> An idiot admires complexity. If you make something so clusterfucked he can't understand it, he's gonna think you're a god.

## Disclaimer
<p>
Being fed up with Vim, Emacs and Visual Studio Code, I decided to create my own text editor, with coke and hookers.
In all seriousness, this probably won't go anywhere, so don't get your hopes up.
</p>

## Overview
<p>
FVFEFVCS is a simple-to-use text editor that does not need you to learn obscure keybindings, a lisp dialect or download stuff from some marketplace.
It will offer treesitter and lsp-support right out of the box like [Helix](https://helix-editor.com/) but with an intuitive and modern GTK4/LibAdw User
Interface similar to the default [GNOME Text Editor](https://apps.gnome.org/app/org.gnome.TextEditor/).
</p>

## Building
```
mkdir build
meson build
cd build
meson compile
./src/fvfefvsc
```
