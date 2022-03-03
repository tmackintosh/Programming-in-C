#include <stdio.h>
#include <stdbool.h>

typedef struct {
    int year;
    int month;
    int day;
    int weekday;
    int daysFromStart;
} Date;

// @param month int month number, January = 1, Decemeber = 12
// @param year int year number
// @returns int number of days in the month on the year given.
int getDaysInMonth (int month, int year) {
    switch (month) {
        case 1:
            return 31;
            break;

        case 2:
            // Leap year logic
            if (year % 4 == 0 && (year % 400 == 0 || !(year % 100 == 0))) {
                return 29;
            }
            else {
                return 28;
            }

            break;

        case 3:
            return 31;
            break;

        case 4:
            return 30;
            break;

        case 5:
            return 31;
            break;

        case 6:
            return 30;
            break;

        case 7:
            return 31;
            break;

        case 8:
            return 31;
            break;

        case 9:
            return 30;
            break;

        case 10:
            return 31;
            break;

        case 11:
            return 30;
            break;

        case 12:
            return 31;
            break;
    }

    return -1;
}

// NB: This function will only work for years in the range 1801 - 2099 given the leap year constraints and tight coupling to the question
// @param currentDate Date the complex data type of the date to count how many days from start
// @returns int number of days since the 1st of January 1801
int calculateDaysFromStart (Date currentDate) {
    int daysFromStart = 0;

    int deltaYear = currentDate.year - 1801;
    daysFromStart += (365 * deltaYear);

    int leapYears = (deltaYear) / 4;
    daysFromStart += leapYears;

    // Given the context of this question, if the year is greater
    // than 1900 then a specific leap year on 1900 didn't take place.
    // TODO:
    // This should be adjusted to decouple this function at a later date.
    if (currentDate.year > 1900) {
        daysFromStart -= 1;
    }

    while (currentDate.month > 1) {
        daysFromStart += getDaysInMonth(currentDate.month - 1, currentDate.year);
        currentDate.month -= 1;
    }

    daysFromStart += currentDate.day - 1;

    return daysFromStart;
}

// @param currentDate Date complex data type to mutate into the next month/
// @returns a copied but mutated complex data type, does not reference currentDate parameter
Date advanceMonth (Date currentDate) {
    currentDate.day += getDaysInMonth(currentDate.month, currentDate.year);

    // Recall getDaysInMonth function to recalculate days in the current month
    // to maintain consistency.
    while (currentDate.day > getDaysInMonth(currentDate.month, currentDate.year)) {
        currentDate.day -= getDaysInMonth(currentDate.month, currentDate.year);
        currentDate.month += 1;

        while (currentDate.month > 12) {
            currentDate.month -= 12;
            currentDate.year += 1;
        }
    }

    currentDate.daysFromStart = calculateDaysFromStart(currentDate);
    return currentDate;
}

// @param currentDate Date complex data type to get the weekday from
// @returns int the weekday of the date, 0 = Monday, 6 = Sunday
int getWeekday(Date currentDate) {
    // As 1/1/1801 is a Sunday, we remove 1 to adjust
    // for the weekday.
    return (currentDate.daysFromStart - 1) % 7;
}

// @param currentDate complex data type to display onto the terminal
// @returns void
void displayCurrentDate (Date currentDate) {
    char weekdays[7][10] = {
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"
    };

    char months[12][10] = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    printf("%s ", weekdays[getWeekday(currentDate)]);
    printf("%d ", currentDate.day);
    printf("%s ", months[currentDate.month - 1]);
    printf("%d\n", currentDate.year);
}

// Returns how many Fridays fell on the fifth of the month during the nineteenth and twentieth centuries (1 Jan 1801 to 31 Dec 2000) given the dates in the question text
int howManyDays() {
    int fridays = 0;

    // Initialize starting day.
    // Very tightly coupled to the question.
    Date currentDate;
    currentDate.year = 1801;
    currentDate.month = 1;
    currentDate.day = 5;
    currentDate.daysFromStart = calculateDaysFromStart(currentDate);

    // Assess months until the year 2000 as specified in the question
    while(currentDate.year < 2001) {
        // Weekday number 4 is a Friday
        if (getWeekday(currentDate) == 4) {
            displayCurrentDate(currentDate);
            fridays += 1;
        }
        currentDate = advanceMonth(currentDate);
    }
    
    printf("%d", fridays);
    return fridays;
}

int main() {
    howManyDays();
    return 0;
}