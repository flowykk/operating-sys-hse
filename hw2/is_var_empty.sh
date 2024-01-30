#!/bin/bash

variable1="value"
variable2=""

# Проверка variable1 на пустоту
if [ -z "$variable1" ]; then
    echo "variable1 пуста."
else
    echo "variable1 не пуста, variable1 = $variable1."
fi

# Проверка variable2 на пустоту
if [ -z "$variable2" ]; then
    echo "variable2 пуста."
else
    echo "variable2 не пуста, variable2 = $variable2."
fi
