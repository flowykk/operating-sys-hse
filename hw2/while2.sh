#!/bin/bash

name=""

while [ "$name" != "0" ]; do
    read -p "Введите слово: " name

    if [ "$name" != "0" ]; then
        echo "Введенное имя: $name!"
    fi
done
