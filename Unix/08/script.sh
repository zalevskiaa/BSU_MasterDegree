# В файл file.txt запишите строку
# The quick brown fox jumps over the lazy dog.
# (строка оканчивается символом перевода строки).
# Создайте символическую ссылку link1.txt, указывающую на файл file.txt.
# Затем создайте жёсткую ссылку link2.txt, которая будет указывать на символическую ссылку link1.txt.

echo "The quick brown fox jumps over the lazy dog.\n" > file.txt
ln -s -f -n file.txt link1.txt
ln -P -f link1.txt link2.txt