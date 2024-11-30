from cs50 import get_float

change = get_float("Change: ")
while (change < 0):
    change = get_float("Change: ")

change = change * 100
count = 0

for i in [25, 10, 5, 1]:
    removed_coins = change//i
    change -= removed_coins * i
    count += removed_coins

print(int(count))
