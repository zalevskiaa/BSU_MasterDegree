# В рамках учебных дисциплин по алгоритмам обычно предлагаются вымышленные задачи. А на курсе «Эксплуатация и администрирование UNIX-систем» решаются реальные задачи из жизни.
# Денис занимается администрированием тестирующей системы https://acm.bsu.by/. Помогите Денису по логам подсчитать число GET-запросов к серверу за каждый день.
# Формат входных данных
# На стандартный ввод поступает лог-файл веб-сервера Microsoft IIS в формате W3C.
# Каждая строка лога описывает один запрос и состоит из 15 полей, разделённых пробелом. Список полей таков (текст взят из документации):
# Date. The date on which the activity occurred.
# Time. The time, in coordinated universal time (UTC), at which the activity occurred.
# Server IP Address. The IP address of the server on which the log file entry was generated.
# Method. The requested verb, for example, a GET method.
# URI Stem. The target of the verb, for example, Default.htm.
# URI Query. The query, if any, that the client was trying to perform. A Universal Resource Identifier (URI) query is necessary only for dynamic pages.
# Server Port. The server port number that is configured for the service.
# User Name. The name of the authenticated user that accessed the server. Anonymous users are indicated by a hyphen.
# Client IP Address. The IP address of the client that made the request.
# User Agent. The browser type that the client used.
# Referrer. The site that the user last visited. This site provided a link to the current site.
# HTTP Status. The HTTP status code.
# Protocol Substatus. The substatus error code.
# Win32 Status. The Windows status code.
# Time Taken. The length of time that the action took, in milliseconds.
# Строки, начинающиеся с решётки, обрабатывать не нужно (это комментарии).
# Формат выходных данных
# Для каждого дня, представленного в лог-файле, выведите число обработанных сервером запросов с методом GET. Статистика выводится в хронологическом порядке: дата (в формате YYYY-MM-DD) и число запросов за этот день, разделителем является знак табуляции.
# Пример
# Входные данные:
# #Software: Microsoft Internet Information Services 8.5
# #Version: 1.0
# #Fields: date time s-ip cs-method cs-uri-stem cs-uri-query s-port cs-username c-ip cs(User-Agent) cs(Referer) sc-status sc-substatus sc-win32-status time-taken
# 2017-03-15 00:03:31 217.21.43.21 GET /contests/7/problems/3551/ - 443 - 93.158.152.28 Mozilla/5.0+(compatible;+YandexBot/3.0;++http://yandex.com/bots) - 200 0 0 205
# 2017-03-15 01:10:52 217.21.43.21 GET / - 443 - 66.249.66.230 Mozilla/5.0+(compatible;+Googlebot/2.1;++http://www.google.com/bot.html) - 301 0 0 171
# 2017-03-14 08:16:10 217.21.43.21 GET /login/ - 443 - 10.150.6.51 Mozilla/5.0+(Windows+NT+6.1)+AppleWebKit/537.36+(KHTML,+like+Gecko)+Chrome/56.0.2924.87+Safari/537.36 https://acm.bsu.by/ 200 0 0 18
# 2017-03-14 08:16:10 217.21.43.21 GET /courses/27/submit/ - 443 - 46.216.216.92 Mozilla/5.0+(X11;+Ubuntu;+Linux+x86_64;+rv:52.0)+Gecko/20100101+Firefox/52.0 https://acm.bsu.by/courses/27/problems/2962/ 200 0 0 232
# 2017-03-14 08:16:15 217.21.43.21 POST /login/ - 443 - 10.150.6.51 Mozilla/5.0+(Windows+NT+6.1)+AppleWebKit/537.36+(KHTML,+like+Gecko)+Chrome/56.0.2924.87+Safari/537.36 https://acm.bsu.by/login/ 302 0 0 409
# 2017-03-14 08:16:15 217.21.43.21 GET / - 443 - 10.150.6.51 Mozilla/5.0+(Windows+NT+6.1)+AppleWebKit/537.36+(KHTML,+like+Gecko)+Chrome/56.0.2924.87+Safari/537.36 https://acm.bsu.by/login/ 200 0 0 68
# 2017-03-14 08:16:17 217.21.43.21 GET /courses/27/my/attempts/ problem=2962 443 - 46.216.216.92 Mozilla/5.0+(X11;+Ubuntu;+Linux+x86_64;+rv:52.0)+Gecko/20100101+Firefox/52.0 https://acm.bsu.by/courses/27/submit/ 200 0 0 52
# 2017-03-14 08:16:19 217.21.43.21 GET /courses/23/ - 443 - 10.150.6.51 Mozilla/5.0+(Windows+NT+6.1)+AppleWebKit/537.36+(KHTML,+like+Gecko)+Chrome/56.0.2924.87+Safari/537.36 https://acm.bsu.by/ 200 0 0 249
# 2017-03-14 08:16:21 217.21.43.21 POST /courses/27/submit/ - 443 - 46.216.216.92 Mozilla/5.0+(X11;+Ubuntu;+Linux+x86_64;+rv:52.0)+Gecko/20100101+Firefox/52.0 https://acm.bsu.by/courses/27/submit/ 302 0 0 134
# 2017-03-14 08:16:21 217.21.43.21 GET /courses/27/submission/172032/ - 443 - 46.216.216.92 Mozilla/5.0+(X11;+Ubuntu;+Linux+x86_64;+rv:52.0)+Gecko/20100101+Firefox/52.0 https://acm.bsu.by/courses/27/submit/ 200 0 0 94
# 2017-03-14 08:16:24 217.21.43.21 GET /solutions/172032/status/json/ - 443 - 46.216.216.92 Mozilla/5.0+(X11;+Ubuntu;+Linux+x86_64;+rv:52.0)+Gecko/20100101+Firefox/52.0 https://acm.bsu.by/courses/27/submission/172032/ 200 0 0 60
# 2017-03-14 08:17:22 217.21.43.21 GET /.well-known/apple-app-site-association - 443 - 66.249.66.130 Mozilla/5.0+(compatible;+Googlebot/2.1;++http://www.google.com/bot.html) - 404 0 2 131
# Выходные данные:
# 2017-03-14  8
# 2017-03-15  2

awk '{
  if (substr($1,1,1) != "#" && $4 == "GET") {
        ++mp[$1]
  }
}
END {
  n = asorti(mp, ind)
  for (i = 1; i <= n; ++i) {
    print ind[i]"\t"mp[ind[i]]
  }
}'