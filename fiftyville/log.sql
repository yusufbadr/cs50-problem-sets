-- Keep a log of any SQL queries you execute as you solve the mystery.

-- checking any connected reports => id:295 - 3 witnesses - each mention bakery - theft @ 10:15 AM 28/7/2023
SELECT * FROM crime_scene_reports
WHERE day = 28 AND month = 7  AND year = 2023 AND street LIKE "%Humphrey Street%";

-- checking interviews
SELECT * FROM interviews
WHERE day = 28 AND month = 7  AND year = 2023
AND transcript LIKE "%bakery%";

-- within 10 min of theft => driveaway
SELECT * FROM bakery_security_logs
WHERE activity = "exit"
AND day = 28 AND month = 7  AND year = 2023
AND hour = 10 AND minute>=15 AND minute<=25;

-- withdrawing money from Leggett Street ATM before theft (before interviewee arrived at bakery)
SELECT * FROM atm_transactions
WHERE transaction_type = "withdraw" AND atm_location LIKE "%Leggett Street%"
AND day = 28 AND month = 7 AND year = 2023;

-- while leaving => called someone for < 1 min
SELECT * FROM phone_calls
WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2023;

-- earliest flight out of fiftyville tmrw (29/07/2023)
SELECT * FROM flights
WHERE day = 29 AND month = 7 AND year = 2023
ORDER BY hour, minute LIMIT 1;

-- thief escaped to: id = 4
SELECT * FROM airports WHERE id =
(
    SELECT destination_airport_id FROM flights
    WHERE day = 29 AND month = 7 AND year = 2023
    ORDER BY hour, minute LIMIT 1
);

-- getting passport number of thief : passport_number = 5773159633
SELECT passport_number FROM passengers
WHERE flight_id =
(
    SELECT id FROM flights
    WHERE day = 29 AND month = 7 AND year = 2023
    ORDER BY hour, minute LIMIT 1
)
AND passport_number IN
(
    SELECT passport_number FROM people
    WHERE license_plate IN
    (
        SELECT license_plate FROM bakery_security_logs
        WHERE activity = 'exit'
        AND day = 28 AND month = 7  AND year = 2023
        AND hour = 10 AND minute>=15 AND minute<=25
    ) AND id IN
    (
        SELECT person_id FROM bank_accounts
        WHERE account_number IN
        (
            SELECT account_number FROM atm_transactions
            WHERE transaction_type = 'withdraw' AND atm_location LIKE '%Leggett Street%'
            AND day = 28 AND month = 7 AND year = 2023
        )
    ) AND phone_number IN
    (
        SELECT caller FROM phone_calls
        WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2023
    )
);

-- getting the name of the thief
SELECT name FROM people
WHERE passport_number = 5773159633;

-- getting thief's phone number: (367) 555-5533
SELECT phone_number FROM people
WHERE passport_number = 5773159633;

-- getting thieif's accomplice phone number: (375) 555-8161
SELECT * FROM phone_calls
WHERE duration < 60 AND day = 28 AND month = 7 AND year = 2023
AND caller = '(367) 555-5533';

-- getting thieif's accomplice name: Robin
SELECT * FROM people
WHERE phone_number = '(375) 555-8161';
