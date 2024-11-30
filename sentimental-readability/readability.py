from cs50 import get_string


def main():
    text = get_string("Text: ")
    l = count_letters(text) / count_words(text) * 100
    s = count_sentences(text) / count_words(text) * 100
    index = 0.0588 * l - 0.296 * s - 15.8

    grade = round(index)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print("Grade", grade)


def count_words(text):
    count = 1
    for i in text:
        if (i == " "):
            count += 1
    return count


def count_sentences(text):
    count = 0
    for i in text:
        if (i == '.' or i == '!' or i == '?'):
            count += 1

    return count


def count_letters(text):
    count = 0
    for i in text:
        if i.isalpha():
            count += 1
    return count


main()
