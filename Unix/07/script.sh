# В текущем каталоге находится файл под названием file.
# Выведите права доступа к этому файлу сначала в числовом виде (три восьмеричные цифры), затем через пробел — в буквенном виде (десять символов, каждый из которых — r, w, x и -).
# Пример
# 640 -rw-r-----

stat --printf "%a %A\n" file