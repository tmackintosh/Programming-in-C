#include <stdio.h>
#include <stdbool.h>

// Returns how many Fridays fell on the fifth of the month during the nineteenth and twentieth centuries (1 Jan 1801 to 31 Dec 2000) given the dates in the question text
typedef struct {
    int year;
    int month;
    int day;
    int weekday;
    int daysFromStart;
} Date;

int getDaysInMonth (int month, int year) {
    switch (month) {
        case 1:
            return 31;
            break;

        case 2:
            if (year % 4 == 0 && (!year % 100 == 0 || year % 400 == 0)) {
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

int calculateDaysFromStart (Date currentDate) {
    int daysFromStart = 0;

    int deltaYear = currentDate.year - 1801;
    daysFromStart += (365 * deltaYear);

    int leapYears = (deltaYear) / 4;
    daysFromStart += leapYears;

    if (currentDate.year >= 2000) {
        daysFromStart -= 1;
    }

    while (currentDate.month > 1) {
        daysFromStart += getDaysInMonth(currentDate.month - 1, currentDate.year);
        currentDate.month -= 1;
    }

    daysFromStart += currentDate.day - 1;

    return daysFromStart;
}

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

int getWeekday(Date currentDate) {
    // As 1/1/1801 is a Sunday, we remove 1 to adjust
    // for the weekday.
    return (currentDate.daysFromStart - 1) % 7;
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

int howManyDays() {
    Date currentDate;
    currentDate.year = 1801;
    currentDate.month = 1;
    currentDate.day = 5;
    currentDate.daysFromStart = calculateDaysFromStart(currentDate);

    while(currentDate.year < 2001) {
        displayCurrentDate(currentDate);
        currentDate = advanceMonth(currentDate);
    }
    
    return 0;
}

int main() {
    howManyDays();
    return 0;
}