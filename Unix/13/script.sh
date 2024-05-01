# Напишите скрипт printargs.sh, работающий так же, как ранее созданная программа на C:
# #include <stdio.h>

# int main(int argc, char* argv[]) {
#     for (int i = 0; i < argc; ++i) {
#         printf("arg #%d: %s\n", i, argv[i]);
#     }
#     return 0;
# }
# Пример
# $ ./printargs.sh lorem ipsum 'dolor sit amet'
# arg #0: ./printargs.sh
# arg #1: lorem
# arg #2: ipsum
# arg #3: dolor sit amet

echo "arg #0: $0"
count=1
for i; do 
   echo "arg #$count: $i"
   count=$(($count + 1))
done