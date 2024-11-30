from cs50 import get_int

h = get_int("Height: ")
while (h <= 0 or h >= 9):
    h = get_int("Height: ")

for i in range(h):
    for j in range(h-i-1):
        print(" ", end="")
    for j in range(i+1):
        print("#", end="")

    print()
