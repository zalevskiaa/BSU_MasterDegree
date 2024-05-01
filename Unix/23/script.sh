# Распечатайте имена всех пользователей системы из файла /etc/passwd через запятую. Вывод должен завершаться переводом строки.
# Пример
# $ ./usernames
# root,daemon,bin,sys,sync,games,student

awk -F: '{printf "%s%s",sep,$1; sep=","} END{print ""}' /etc/passwd

# awk -F: '{print $1} END{print ","}' /etc/passwd
# awk '{printf "%s%s",sep,$1; sep=","} END{print ""}' file

# while IFS= read -r line; do
#     echo $line
#     echo awk -F: '{print $1}' $line
# done < /etc/passwd