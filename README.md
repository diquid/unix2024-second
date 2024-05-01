# unix2024-second
## Задача 2 Тема: совместная работа с файлами, понятие блокировок.
<br>

Выполнила Фатыхова Диляра ФТ-403 МЕН-400809  
<br>

Чтобы запустить выполните следующие команды в своем терминале:  
`chmod +x runme.sh`  
`./runme.sh`  
(Первая строка дает скрипту право на исполнение, вторая запускает скрипт)  
<br>

Скрипт скомпилирует и запустит `main`.  
<br>

После исполнения программы в директории появятся `.lck` файл и файл `results.txt` со статистикой после выполнения.  
После исполнения статистика также выведется в консоль.  
<br>

Если запустить программу и во время ее исполнения вручную удалить из директории `.lck`, то программа продолжит работать.  
<br>

В случае, если во время исполнения программы возникнут какие-либо ошибки (в том числе после удаления `.lck` файла во время исполнения), то в директории появится файл `errors.log`, в который они будут логироваться.  
Если ошибок во время исполнения не будет, то `errors.log` не появится.  
<br>

Для наглядности в директории есть файлы `errors_example.log` и `results_example.txt`, в них результаты моего запуска.  
<br>
