import csv
import sys


def main():

    # TODO: Check for command-line usage
    args = sys.argv
    if len(args) < 3:
        print("Incorrect command line usage")
        sys.exit(1)

    csv_filename = args[1]
    txt_filename = args[2]

    # TODO: Read database file into a variable
    rows = []
    with open(csv_filename) as file:
        reader = csv.DictReader(file)
        field_names = reader.fieldnames
        for row in reader:
            rows.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(txt_filename) as file:
        dna_sequence = file.read()

    # TODO: Find longest match of each STR in DNA sequence

    suspect = {}

    # for subsequence in field_names:
    #     match_number = longest_match(dna_sequence, subsequence)
    #     suspect[subsequence] = match_number

    for i in range(1, len(field_names)):
        match_number = longest_match(dna_sequence, field_names[i])
        suspect[field_names[i]] = match_number

    # TODO: Check database for matching profile
    for row in rows:
        found = True
        for i in range(1, len(field_names)):
            if (str(row[field_names[i]]) != str(suspect[field_names[i]])):
                found = False
                break
        if found is True:
            print(row[field_names[0]])
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
