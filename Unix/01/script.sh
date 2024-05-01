# Создайте иерархию каталогов одним вызовом утилиты.
# $ tree
# .
# ├── another
# │   └── long
# │       └── path
# ├── very
# │   └── very
# │       └── very
# │           └── long
# │               └── path
# └── yet
#     └── another
#         └── path

mkdir -p another/long/path very/very/very/long/path yet/another/path