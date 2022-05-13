import random

lines = []

for i in range(1000000):
    lines.append(" ".join([str(random.randint(-100, 100)), str(random.randint(-100, 100))]) + "\n")

with open("test4.txt", "wt") as file:
    file.writelines(lines)