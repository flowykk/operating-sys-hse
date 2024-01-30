#!/bin/bash

word=""

while [ "$word" != "0" ]; do
    read -p "Введите слово: " word

    if [ "$name" != "0" ]; then
        echo "Введенное слово: $word!"
    fi
done
