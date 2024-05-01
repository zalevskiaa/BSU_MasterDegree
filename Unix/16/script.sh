# Напишите скрипт fdcount.sh. Скрипт принимает один аргумент — путь к каталогу. На выходе скрипт выводит общее число обычных файлов и подкаталогов в указанном каталоге (рекурсивно). При подсчёте подкаталогов сам каталог также учитывается. Переходить по символическим ссылкам не нужно.
# Вывод форматируется, как показано в примере. Нужно подставлять ту или иную форму слова в соответствии с числом найденного (0 files, 1 file, 2 files). Во избежание дублирования кода вынесите логику в шелл-функцию.
# Пример
# sobols@sobols-VirtualBox:~/unix$ ./fdcount.sh .
# 1 file
# 1 directory
# sobols@sobols-VirtualBox:~/unix$ mkdir subdir
# sobols@sobols-VirtualBox:~/unix$ ./fdcount.sh .
# 1 file
# 2 directories
# sobols@sobols-VirtualBox:~/unix$ touch subdir/file
# sobols@sobols-VirtualBox:~/unix$ ./fdcount.sh .
# 2 files
# 2 directories


# path="$*"
# files="$(find "${path}" -type f | wc -l)"
# dirs="$(find "${path}" -type d | wc -l)"

# if [ $files -ne 1 ]
# then
#         echo "$files files"
# else
#         echo "$files file"
# fi
# if [ $dirs -ne 1 ]
# then
#         echo "$dirs directories"
# else
#         echo "$dirs directory"
# fi


format()
{
if [ $1 -eq 1 ]
        then
        if [ $2 -ne 1 ]
        then
                file="files"
        fi
else
        if [ $2 -ne 1 ]
        then
                dir="directories"
        fi
fi
}

path="$*"
file="file"
dir="directory"

files="$(find "${path}" -type f | wc -l)"
dirs="$(find "${path}" -type d | wc -l)"

format 1 $files
format 2 $dirs

echo $files $file
echo $dirs $dir