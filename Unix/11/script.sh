# Напишите скрипт, который выводит в stderr значение переменной окружения PATH в такой форме (обратите внимание на кавычки):
# *** Value of '$PATH' is "/usr/local/bin:/usr/bin:/bin" ***

>&2 echo "*** Value of '\$PATH' is \"$PATH\" ***"