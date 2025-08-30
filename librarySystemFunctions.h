#ifndef LIBRARYSYSTEMFUNCTIONS_H
#define LIBRARYSYSTEMFUNCTIONS_H

#include <stdbool.h>

#define MAX 100

typedef struct date {
    int year;
    int month;
    int day;
} date;

// Dwukierunkowa lista książek
typedef struct lista {
    int ID;                         // unikalny identyfikator
    char title[MAX];                // tytuł
    char authorsName[MAX];          // imię autora
    char authorsSurname[MAX];       // nazwisko autora
    char category[MAX];             // kategoria
    date borrowed;                  // data wypożyczenia (0/0/0 jeśli brak)
    char borrowerName[MAX];         // imię wypożyczającego
    char borrowerSurname[MAX];      // nazwisko wypożyczającego
    char notes[MAX];                // inne dane
    struct lista *prev;             // wskaznik na poprzedni element
    struct lista *next;             // wskaznik na nastepny element
} lista;

// Główne wskaźniki listy
extern lista *head;
extern lista *tail;

// funkcje
int assignID(void);
void addNewBook(void);
void deleteFirstElement(lista *element);
void deleteLastElement(lista *element);
void deleteInside(lista *element);
void delete(void);
void editData(void);
void changeStatus(void);
void search(void);
void printAllByTitles(void);
void printBorrowed(void);
void saveToFile(void);
void importFromFile(void);

//dodatkowa funkcja porządkująca
void freeAll(void);

#endif //LIBRARYSYSTEMFUNCTIONS_H
