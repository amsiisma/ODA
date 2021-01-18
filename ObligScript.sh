#!/bin/bash

#sjekker om det er noen miljøvariabler, disse prioriteres
# I står for hvilken plass på linjen brukernavnet kommer på
if [ -n "$FORNAVN" ]; then
	I=3
elif [ -n "$ETTERNAVN" ]; then
	I=3
elif [ -n "$BRUKERNAVN" ]; then
	I=1
elif [ ! -z "$1" ]; then #sjekker om det er argumenter
        ARG=$1
        case $ARG in
                b)
                I=1
                ;;
                e)
                I=3
                ;;
                f)
                I=3
                ;;
                *)
        esac
else
	I=3 #default
fi

PROGRAM=$(./obligtest $@ | sort ) 

while read -r line; do
	USERNAME="$(cut -d',' -f$I <<<"$line" | tr -d ' ')" #henter ut brukernavn og trimmer bort et mellomrom fra hver bruker

	if [ -e "/home/$USERNAME/public_html/pub_key.asc" ]; then
		echo " $line has key"
	else
		echo $line
	fi
done <<< "$PROGRAM"




