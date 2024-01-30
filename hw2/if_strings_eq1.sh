#!/bin/bash

# Проверка равенства двух строк
string1="hello"
string2="world"

if [ "$string1" = "$string2" ]; then
    echo "Строки равны."
else
    echo "Строки не равны."
fi
