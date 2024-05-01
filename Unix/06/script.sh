# Задан каталог dirA, в котором находятся файлы и подкаталоги. Скопируйте каталог dirA в каталог dirB как можно более точно: структура каталогов должна быть воспроизведена, для каждого файла у оригинала и у копии должно совпадать содержимое, пользователь-владелец и группа-владелец, права доступа, время модификации.
# Замечание
# Разрешается использовать sudo. Система на сервере настроена так, что пароль при этом не запрашивается.
# Пример
# $ tree -a -p -u --timefmt %Y-%m-%d dirA
# dirA
# |-- [-r-----r-- student   2019-12-03]  backup2.dat
# |-- [-r--r----- student   2019-12-03]  backup3.dat
# |-- [-r--r--r-- student   2019-12-03]  backup.dat
# `-- [drwxrwxr-x student   2019-12-03]  subdir
#     |-- [drwxrwxr-x student   2019-12-03]  .hidden
#     |   |-- [---------- root     2010-01-02]  .more_secret
#     |   `-- [-r--r----- root     2010-01-01]  secret.txt
#     `-- [drwxrwxr-x student   2019-12-03]  public
#         |-- [-rw-rw-rw- student   2007-12-30]  consectetur.txt
#         |-- [-rw-r--r-- student   2007-12-31]  dolor_sit_amet.txt
#         |-- [-rw-r--r-- student   2007-12-29]  lorem_ipsum.txt
#         `-- [-rwxr-xr-x student   2019-12-03]  run.sh

# 3 directories, 9 files

sudo cp -R -p dirA dirB