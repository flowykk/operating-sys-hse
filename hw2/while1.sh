#!/bin/bash

count=1

# Цикл будет выполняться, пока count <= 5
while [ $count -le 5 ]; do # Условие окончания цикла цикла
    echo "count = $count"
    ((count++))  # Увеличиваем счётчик на 1
done

# Цикл закончился
echo "Цикл закончился"

