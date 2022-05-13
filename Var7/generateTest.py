import random

lines = [str(random.randint(-10000, 10000) / random.randrange(10000, 1000000, 10000)) for i in range(100000)]

with open("test3.txt", "wt") as file:
    file.write(" ".join(lines))