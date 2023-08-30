#!/bin/bash

# Commande irssi avec les options spécifiées
irssi_cmd="irssi -c localhost -p 8080 -n valentin -w pass"
irssi_cmd1="irssi -c localhost -p 8080 -n sami -w pass"
irssi_cmd2="irssi -c localhost -p 8080 -n sophia -w pass"
# Fonction pour ouvrir un terminal et exécuter une commande
open_terminal() {
    terminal_cmd=$1
    osascript -e "tell application \"Terminal\" to do script \"$terminal_cmd\""
}

# Ouvrir trois terminaux avec la commande irssi
open_terminal "$irssi_cmd"
open_terminal "$irssi_cmd1"
open_terminal "$irssi_cmd2"
