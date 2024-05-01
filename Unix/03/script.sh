# Дано два каталога — dirA и dirB. Одним вызовом cp скопируйте всё содержимое из первого каталога во второй. Файлы, ранее лежавшие в каталоге dirB, должны быть сохранены. Гарантируется, что имена файлов различны.
# Пример
# Создайте такую структуру:
# sobols@sobols-w10:~/unix/fs/task4$ tree -a
# .
# ├── dirA
# │   ├── .gitignore
# │   ├── lorem.txt
# │   └── subdir
# │       └── dolor.txt
# └── dirB
#     └── ipsum.txt

# 3 directories, 4 files
# В результате копирования файлы lorem.txt и ipsum.txt оба окажутся в dirB. В итоге должно получиться так:
# sobols@sobols-w10:~/unix/fs/task4$ tree -a
# .
# ├── dirA
# │   ├── .gitignore
# │   ├── lorem.txt
# │   └── subdir
# │       └── dolor.txt
# └── dirB
#     ├── .gitignore
#     ├── ipsum.txt
#     ├── lorem.txt
#     └── subdir
#         └── dolor.txt

# 4 directories, 7 files

cp -R dirA/. dirB