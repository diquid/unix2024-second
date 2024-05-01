#!/bin/bash

# Компиляция
gcc -o main main.c

count=10
completedTasks=0

# Запуск 10 процессов
seq $count | xargs -I{} -n 1 -P $count ./main myfile &

# Ожидание исполнения программы в течение 5 минут
sleep 300

# Остановка процессов с помощью SIGINT
pkill -SIGINT main
completedTasks=$(pgrep -c main)

# Вывод результатов в файл result.txt и в консоль
echo "In 5 minutes:"
echo "In 5 minutes:" >> result.txt

cat statistics
cat statistics >> result.txt

rm statistics
