#include <stdio.h>
#include <stdlib.h>
#include "librarySystemFunctions.h"

static void print_menu(void){
    printf("------------------------------------------------------------------------------\n"
           "MENU: wybierz akcję, wpisując przypisany znak:\n"
           "1) Dodaj nową książkę\n"
           "2) Usuń książkę\n"
           "3) Edytuj dane książki\n"
           "4) Zmień status książki (wypożyczona/oddana)\n"
           "5) Wyszukaj książkę\n"
           "6) Wyświetl bazę po tytułach\n"
           "7) Wyświetl książki wypożyczone\n"
           "8) Zapisz do pliku\n"
           "9) Odczytaj z pliku\n"
           "X) Zakończ program\n"
           "------------------------------------------------------------------------------\n");
}

int main(void){
    char choice[16];
    while(1){
        print_menu();
        printf("Wybór: ");
        if(!fgets(choice, sizeof(choice), stdin)) break;
        switch(choice[0]){
            case '1': addNewBook(); break;
            case '2': delete(); break;
            case '3': editData(); break;
            case '4': changeStatus(); break;
            case '5': search(); break;
            case '6': printAllByTitles(); break;
            case '7': printBorrowed(); break;
            case '8': saveToFile(); break;
            case '9': importFromFile(); break;
            case 'x': case 'X':
                freeAll();
            printf("Do zobaczenia!\n");
            return 0;
            default:
                printf("Błąd! Upewnij się, że wpisałeś poprawny znak!\n");
        }
    }
    freeAll();
    return 0;
}