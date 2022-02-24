#include <stdio.h>

// Returns how many Fridays fell on the fifth of the month during the nineteenth and twentieth centuries (1 Jan 1801 to 31 Dec 2000) given the dates in the question text
typedef struct {
    int year;
    int month;
    int day;
    int weekday;
    int daysFromStart;
} Date;

Date advanceMonth (Date currentDate) {
    int increments[12] = {
        31,
        28,
        31,
        30,
        31,
        30,
        31,
        31,
        30,
        31,
        30,
        31
    };

    currentDate.daysFromStart += increments[currentDate.month - 1];
    
    // Check for leap-year anomalies
    if (currentDate.month == 2) {
        if (currentDate.year % 4 == 0 && currentDate.year % 400 != 0) {
            currentDate.daysFromStart += 1;
        }
    }

    currentDate.month += 1;

    // In case the year has incremented.
    if (currentDate.month > 12) {
        currentDate.month = (currentDate.month % 12);

        // Edge case that currentDate.month may have been 24 or
        // any other multiple of 12 other than 12.
        if (currentDate.month == 0) {
            currentDate.year += currentDate.month / 12;
            currentDate.month = 12;
        }
        else {
            currentDate.year += 1;
        }
    }

    return currentDate;
}

int getWeekday(Date currentDate) {
    return (currentDate.daysFromStart - 2) % 7;
}

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

int calculateDaysFromStart (Date currentDate) {
    int daysFromStart = 0;

    int deltaYear = currentDate.year - 1801;
    daysFromStart += (365 * deltaYear);

    int leapYears = (deltaYear + 2) / 4;
    daysFromStart += leapYears;

    if (currentDate.year >= 2000) {
        daysFromStart -= 1;
    }

    return daysFromStart;
}

int howManyDays() {
    Date currentDate;
    currentDate.year = 1801;
    currentDate.month = 3;
    currentDate.day = 24;
    currentDate.daysFromStart = 30 + 28 + 24;

    displayCurrentDate(currentDate);

    while (currentDate.year != 2004) {
        currentDate = advanceMonth(currentDate);
        displayCurrentDate(currentDate);
    }

    return 0;
}

int main() {
    howManyDays();
    return 0;
}