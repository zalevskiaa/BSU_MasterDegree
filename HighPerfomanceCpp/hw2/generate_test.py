import random
from tqdm.auto import tqdm, trange

def main():
    random.seed(20231113)

    # values number, queries number
    n = q = 10 ** 7

    # values bounds
    a, b = 1, 10 ** 9

    values = [
        random.randint(a, b)
        for _ in range(n)
    ]
    queries = []

    for _ in trange(q):
        match random.randint(1, 4):
            case 1:
                p = random.randint(1, n)
                v = random.randint(a, b)
                queries.append([1, p, v])

            case 2:
                l = random.randint(1, n)
                r = random.randint(1, n)
                l, r = sorted((l, r))
                queries.append([2, l, r])

            case 3:
                l = random.randint(1, n)
                r = random.randint(1, n)
                l, r = sorted((l, r))
                queries.append([3, l, r])

            case 4:
                l = random.randint(1, n)
                r = random.randint(1, n)
                l, r = sorted((l, r))
                queries.append([4, l, r])

            case _:
                raise RuntimeError

    with open('tests/input.txt', 'w') as f:
        f.write(f'{n} {q}' + '\n')
        f.write(' '.join(str(elem) for elem in values) + '\n')

        for query in tqdm(queries):
            f.write(' '.join(str(elem) for elem in query) + '\n')


if __name__ == '__main__':
    main()