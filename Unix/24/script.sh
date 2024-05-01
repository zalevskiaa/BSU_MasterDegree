# Количество строк кода (англ. Source Lines of Code — SLOC) — это метрика программного обеспечения, используемая для измерения его объёма с помощью подсчёта количества строк в тексте исходного кода.
# Посчитайте общее число строк в с- и h-файлах, расположенных в текущем каталоге (с учётом подкаталогов). Ответ нужно напечатать на стандартный вывод.
# Пример
# vasya@pupkin:~/unix/hw/sloc$ tree
# .
# ├── demo
# │   └── main.c
# └── foo.h

# 1 directory, 2 files
# vasya@pupkin:~/unix/hw/sloc$ ./sloc.sh
# 9
# Файл foo.h:
# #pragma once
# Файл demo/main.c:
# #include <stdio.h>

# // Demo Application

# int main(int argc, char* argv[]) {
#     printf("Hello!\n");
#     return 0;
# }

total=0
find ./ -type f -regex ".*\.[hc]" | while read FILE; do
     # You see, use 'grep' instead of 'wc'! for properly counting
     count=$(grep -c ^ < "$FILE")
     echo "$FILE has $count lines"
     let total=total+count #in bash, you can convert this for another shell
     echo total $total
done
echo TOTAL LINES COUNTED:  $total

echo "lol"
find ./ -type f -regex ".*\.[hc]" | while read FILE
do
    count=$(wc -l < $FILE)
    echo "$FILE has $count lines"
done


echo "lol"
echo $(find ./ -type f -regex ".*\.[hc]" -exec wc -l {} \; | awk '{total += (($1))} END{print total}')
