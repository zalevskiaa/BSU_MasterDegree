# Напишите скрипт stdswap.sh, который выполняет указанную команду (со всеми аргументами), меняя при этом местами потоки stdout и stderr (весь стандартный вывод перенаправляется в поток вывода ошибок, и наоборот).
# Пример
# Рассмотрим пример. Пусть есть код main.py на Python:
# #!/usr/bin/env python3
# import sys
# print('Arguments:', sys.argv, file=sys.stdout)
# print('Executable path:', sys.executable, file=sys.stderr)

# Скрипт должен работать следующим образом:
# $ ./main.py "hello, world!" --help 1 -f 2 >/dev/null
# Executable path: /usr/bin/python3
# $ ./stdswap.sh ./main.py "hello, world!" --help 1 -f 2 >/dev/null
# Arguments: ['./main.py', 'hello, world!', '--help', '1', '-f', '2']

"$@" 3>&2 2>&1 1>&3