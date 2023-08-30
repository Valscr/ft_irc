#!/bin/bash

# Commande irssi avec les options spécifiées
irssi_cmd="irssi -c localhost -p 8080 -n valentin -w pass"
irssi_cmd1="irssi -c localhost -p 8080 -n sami -w pass"
irssi_cmd2="irssi -c localhost -p 8080 -n sophia -w pass"

# Fonction pour ouvrir un terminal et exécuter une commande sur macOS
open_terminal_macos() {
    terminal_cmd=$1
    osascript -e "tell application \"Terminal\" to do script \"$terminal_cmd\""
}

# Fonction pour ouvrir un terminal et exécuter une commande sur Linux
open_terminal_linux() {
    terminal_cmd=$1
    geometry=$2
    gnome-terminal $geometry -- bash -c "$terminal_cmd; exec bash"
    # Remplacez 'gnome-terminal' par le nom de votre terminal et ajustez la géométrie selon vos besoins
}

# Détecter le système d'exploitation
if [[ "$OSTYPE" == "darwin"* ]]; then
    open_terminal_macos "$irssi_cmd"
    open_terminal_macos "$irssi_cmd1"
    open_terminal_macos "$irssi_cmd2"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    open_terminal_linux "$irssi_cmd"  --geometry=80x24+200+0
    open_terminal_linux "$irssi_cmd1" --geometry=80x24+1200+0
    open_terminal_linux "$irssi_cmd2" --geometry=80x24+1200+700
fi
