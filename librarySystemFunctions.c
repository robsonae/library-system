#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "librarySystemFunctions.h"

lista *head = NULL;
lista *tail = NULL;

static int next_id = 1;

// --- Pomocnicze funkcje I/O ---
static int ci_strcmp(const char* a, const char* b){
    unsigned char ca, cb;
    while(*a && *b){
        ca = (unsigned char)tolower((unsigned char)*a);
        cb = (unsigned char)tolower((unsigned char)*b);
        if(ca!=cb) return (int)ca-(int)cb;
        a++; b++;
    }
    return (unsigned char)tolower((unsigned char)*a) - (unsigned char)tolower((unsigned char)*b);
}

static void trim_newline(char *s){
    if(!s) return;
    size_t n = strlen(s);
    while(n>0 && (s[n-1]=='\n' || s[n-1]=='\r')) { s[--n] = 0; }
}
static void read_line(const char *prompt, char *buf, size_t buflen){
    if(prompt) printf("%s", prompt);
    if(fgets(buf, (int)buflen, stdin)==NULL){
        // EOF -> treat as empty
        buf[0] = 0;
        clearerr(stdin);
        return;
    }
    trim_newline(buf);
}
static int read_int(const char *prompt){
    char line[64];
    while(1){
        read_line(prompt, line, sizeof(line));
        if(line[0]==0){ // empty -> ask again
            printf("Wpisz liczbę.\n");
            continue;
        }
        char *end=NULL;
        long v = strtol(line, &end, 10);
        if(end && *end==0) return (int)v;
        printf("Niepoprawna liczba, spróbuj ponownie.\n");
    }
}
static int yesno(const char *prompt){
    char line[32];
    while(1){
        read_line(prompt, line, sizeof(line));
        if(line[0]==0) continue;
        char c = (char)tolower((unsigned char)line[0]);
        if(c=='t' || c=='y') return 1; // tak/yes
        if(c=='n') return 0;
        if(c=='1') return 1;
        if(c=='0') return 0;
        printf("Wpisz T/N.\n");
    }
}

static int is_borrowed(const lista *b){
    return (b->borrowed.year!=0);
}

int assignID(void){
    // Funkcja zwraca kolejny wolny ID.
    // Wersja bezpieczna: w razie rozjazdu obliczamy max(head)+1
    int maxid = 0;
    for(lista *p=head; p; p=p->next) if(p->ID>maxid) maxid = p->ID;
    if(next_id <= maxid) next_id = maxid+1;
    return next_id++;
}

// --- Dodawanie nowej książki ---
void addNewBook(void){
    while(1){
        lista *e = (lista*)calloc(1, sizeof(lista));
        if(!e){ printf("Brak pamięci.\n"); return; }
        e->ID = assignID();

        read_line("Podaj tytuł książki: ", e->title, sizeof(e->title));
        read_line("Podaj imię autora: ", e->authorsName, sizeof(e->authorsName));
        read_line("Podaj nazwisko autora: ", e->authorsSurname, sizeof(e->authorsSurname));
        read_line("Podaj kategorię: ", e->category, sizeof(e->category));
        read_line("Inne dane (opcjonalnie): ", e->notes, sizeof(e->notes));

        // pola wypożyczenia puste
        e->borrowed.year = 0; e->borrowed.month = 0; e->borrowed.day = 0;
        e->borrowerName[0] = 0; e->borrowerSurname[0] = 0;

        // dodanie na koniec listy (ostatnio dodany element)
        e->prev = tail;
        e->next = NULL;
        if(tail) tail->next = e;
        tail = e;
        if(!head) head = e;

        printf("Dodano książkę o ID %d.\n", e->ID);

        if(!yesno("Dodać kolejną? (T/N): ")) break;
    }
}

// --- Kasowanie elementów (wewnętrzne) ---
void deleteFirstElement(lista *element){
    if(!element) return;
    head = element->next;
    if(head) head->prev = NULL;
    else tail = NULL;
    free(element);
}
void deleteLastElement(lista *element){
    if(!element) return;
    tail = element->prev;
    if(tail) tail->next = NULL;
    else head = NULL;
    free(element);
}
void deleteInside(lista *element){
    if(!element) return;
    if(element==head){ deleteFirstElement(element); return; }
    if(element==tail){ deleteLastElement(element); return; }
    element->prev->next = element->next;
    element->next->prev = element->prev;
    free(element);
}

// --- Usuwanie po ID lub tytule ---
void delete(void){
    if(!head){ printf("Lista jest pusta.\n"); return; }
    printf("Usuwanie: wybierz tryb:\n1) po ID\n2) po tytule\n");
    char line[8]; read_line("Twój wybór: ", line, sizeof(line));
    if(line[0]=='1'){
        int id = read_int("Podaj ID: ");
        lista *p=head;
        while(p && p->ID!=id) p=p->next;
        if(!p){ printf("Nie znaleziono książki o ID %d.\n", id); return; }
        if(p==head) deleteFirstElement(p);
        else if(p==tail) deleteLastElement(p);
        else deleteInside(p);
        printf("Usunięto książkę o ID %d.\n", id);
    }else if(line[0]=='2'){
        char title[MAX]; read_line("Podaj tytuł: ", title, sizeof(title));
        int found = 0;
        lista *p=head, *next;
        while(p){
            next = p->next;
            if(strcmp(p->title, title)==0){
                if(p==head) deleteFirstElement(p);
                else if(p==tail) deleteLastElement(p);
                else deleteInside(p);
                found++;
            }
            p = next;
        }
        if(found==0) printf("Nie znaleziono książki o podanym tytule.\n");
        else printf("Usunięto %d wystąp.\n", found);
    }else{
        printf("Nieznana opcja.\n");
    }
}

// --- Edycja danych (po ID lub tytule) ---
static void edit_one(lista *b){
    char buf[MAX];
    printf("Edycja książki (ID %d). Pozostaw puste, aby zachować starą wartość.\n", b->ID);
    read_line("Nowy tytuł: ", buf, sizeof(buf)); if(buf[0]) strncpy(b->title, buf, sizeof(b->title));
    read_line("Nowe imię autora: ", buf, sizeof(buf)); if(buf[0]) strncpy(b->authorsName, buf, sizeof(b->authorsName));
    read_line("Nowe nazwisko autora: ", buf, sizeof(buf)); if(buf[0]) strncpy(b->authorsSurname, buf, sizeof(b->authorsSurname));
    read_line("Nowa kategoria: ", buf, sizeof(buf)); if(buf[0]) strncpy(b->category, buf, sizeof(b->category));
    read_line("Inne dane: ", buf, sizeof(buf)); if(buf[0]) strncpy(b->notes, buf, sizeof(b->notes));
}
void editData(void){
    if(!head){ printf("Lista jest pusta.\n"); return; }
    printf("Edycja: wybierz tryb:\n1) po ID\n2) po tytule (wszystkie dopasowania)\n");
    char line[8]; read_line("Twój wybór: ", line, sizeof(line));
    if(line[0]=='1'){
        int id = read_int("Podaj ID: ");
        for(lista *p=head; p; p=p->next) if(p->ID==id){ edit_one(p); return; }
        printf("Nie znaleziono książki o ID %d.\n", id);
    }else if(line[0]=='2'){
        char title[MAX]; read_line("Podaj tytuł: ", title, sizeof(title));
        int count=0;
        for(lista *p=head; p; p=p->next) if(strcmp(p->title, title)==0){ edit_one(p); count++; }
        if(count==0) printf("Nie znaleziono tytułu.\n");
    }else{
        printf("Nieznana opcja.\n");
    }
}

// --- Zmiana statusu wypożyczenia ---
void changeStatus(void){
    if(!head){ printf("Lista jest pusta.\n"); return; }
    printf("Zmień status: wybierz tryb:\n1) po ID\n2) po tytule\n");
    char line[8]; read_line("Twój wybór: ", line, sizeof(line));
    lista *p = NULL;
    if(line[0]=='1'){
        int id = read_int("Podaj ID: ");
        for(p=head; p; p=p->next) if(p->ID==id) break;
    }else if(line[0]=='2'){
        char title[MAX]; read_line("Podaj tytuł: ", title, sizeof(title));
        for(p=head; p; p=p->next) if(strcmp(p->title, title)==0) break;
    }
    if(!p){ printf("Nie znaleziono książki.\n"); return; }

    if(!is_borrowed(p)){
        // ustaw jako wypożyczona
        read_line("Imię wypożyczającego: ", p->borrowerName, sizeof(p->borrowerName));
        read_line("Nazwisko wypożyczającego: ", p->borrowerSurname, sizeof(p->borrowerSurname));
        p->borrowed.year  = read_int("Rok wypożyczenia (YYYY): ");
        p->borrowed.month = read_int("Miesiąc (1-12): ");
        p->borrowed.day   = read_int("Dzień (1-31): ");
        printf("Ustawiono status: wypożyczona.\n");
    }else{
        printf("Książka jest wypożyczona przez %s %s od %04d-%02d-%02d.\n",
               p->borrowerName, p->borrowerSurname,
               p->borrowed.year, p->borrowed.month, p->borrowed.day);
        if(yesno("Oznaczyć jako oddaną? (T/N): ")){
            p->borrowerName[0]=0; p->borrowerSurname[0]=0;
            p->borrowed.year=0; p->borrowed.month=0; p->borrowed.day=0;
            printf("Status zmieniony na: dostępna.\n");
        }
    }
}

// --- Wyszukiwanie ---
void search(void){
    if(!head){ printf("Lista jest pusta.\n"); return; }
    printf("Wyszukiwanie: wybierz tryb:\n1) po ID\n2) po tytule\n3) po nazwisku wypożyczającego\n");
    char line[8]; read_line("Twój wybór: ", line, sizeof(line));
    if(line[0]=='1'){
        int id = read_int("Podaj ID: ");
        for(lista *p=head; p; p=p->next) if(p->ID==id){
            printf("ID:%d | \"%s\" %s %s | kategoria: %s\n", p->ID, p->title, p->authorsName, p->authorsSurname, p->category);
            if(is_borrowed(p))
                printf("  Wypożyczona przez %s %s dnia %04d-%02d-%02d\n",
                       p->borrowerName, p->borrowerSurname, p->borrowed.year, p->borrowed.month, p->borrowed.day);
            return;
        }
        printf("Nie znaleziono.\n");
    }else if(line[0]=='2'){
        char title[MAX]; read_line("Podaj tytuł: ", title, sizeof(title));
        int cnt=0;
        for(lista *p=head; p; p=p->next) if(strcmp(p->title, title)==0){
            printf("ID:%d | \"%s\" %s %s | kategoria: %s\n", p->ID, p->title, p->authorsName, p->authorsSurname, p->category);
            if(is_borrowed(p))
                printf("  Wypożyczona przez %s %s dnia %04d-%02d-%02d\n",
                       p->borrowerName, p->borrowerSurname, p->borrowed.year, p->borrowed.month, p->borrowed.day);
            cnt++;
        }
        if(cnt==0) printf("Brak wyników.\n");
    }else if(line[0]=='3'){
        char sn[MAX]; read_line("Podaj nazwisko wypożyczającego: ", sn, sizeof(sn));
        int cnt=0;
        for(lista *p=head; p; p=p->next) if(is_borrowed(p) && strcmp(p->borrowerSurname, sn)==0){
            printf("ID:%d | \"%s\" %s %s | wypożyczona przez %s %s (%04d-%02d-%02d)\n",
                   p->ID, p->title, p->authorsName, p->authorsSurname,
                   p->borrowerName, p->borrowerSurname,
                   p->borrowed.year, p->borrowed.month, p->borrowed.day);
            cnt++;
        }
        if(cnt==0) printf("Brak wyników.\n");
    }else{
        printf("Nieznana opcja.\n");
    }
}

// --- Wyświetlanie bazy posortowanej po tytułach ---
typedef struct { lista *ptr; } Row;
static int cmp_title(const void *a, const void *b){
    const Row *ra = (const Row*)a, *rb = (const Row*)b;
    return ci_strcmp(ra->ptr->title, rb->ptr->title);
}
void printAllByTitles(void){
    // policz
    int n=0; for(lista *p=head; p; p=p->next) n++;
    if(n==0){ printf("Lista jest pusta.\n"); return; }
    Row *rows = (Row*)malloc(n*sizeof(Row));
    if(!rows){ printf("Brak pamięci.\n"); return; }
    int i=0; for(lista *p=head; p; p=p->next) rows[i++].ptr=p;
    qsort(rows, n, sizeof(Row), cmp_title);
    printf("== Baza książek (posortowana po tytułach) ==\n");
    for(i=0;i<n;i++){
        lista *p = rows[i].ptr;
        printf("ID:%d | \"%s\" | %s %s | kategoria: %s%s\n",
               p->ID, p->title, p->authorsName, p->authorsSurname, p->category,
               is_borrowed(p) ? " | [WYP]" : "");
    }
    free(rows);
}

// --- Wyświetlenie tylko wypożyczonych ---
void printBorrowed(void){
    int cnt=0;
    for(lista *p=head; p; p=p->next){
        if(is_borrowed(p)){
            printf("ID:%d | \"%s\" | przez %s %s od %04d-%02d-%02d\n",
                   p->ID, p->title, p->borrowerName, p->borrowerSurname,
                   p->borrowed.year, p->borrowed.month, p->borrowed.day);
            cnt++;
        }
    }
    if(cnt==0) printf("Brak wypożyczonych książek.\n");
}

// --- Zapis/Odczyt pliku ---
static void write_one(FILE *f, const lista *p){
    // prosty format CSV z separatorem ';'
    fprintf(f, "%d;%s;%s;%s;%s;%d;%d;%d;%s;%s;%s\n",
            p->ID, p->title, p->authorsName, p->authorsSurname, p->category,
            p->borrowed.year, p->borrowed.month, p->borrowed.day,
            p->borrowerName, p->borrowerSurname, p->notes);
}
static int read_one(FILE *f, lista **out){
    char line[2048];
    if(!fgets(line, sizeof(line), f)) return 0;
    trim_newline(line);
    lista *p = (lista*)calloc(1, sizeof(lista));
    if(!p) return 0;
    // rozbij po ';'
    // expecting 11 fields
    char *fields[11]; int nf=0;
    char *tok = strtok(line, ";");
    while(tok && nf<11){ fields[nf++] = tok; tok = strtok(NULL, ";"); }
    if(nf!=11){ free(p); return -1; }
    p->ID = atoi(fields[0]);
    strncpy(p->title, fields[1], sizeof(p->title));
    strncpy(p->authorsName, fields[2], sizeof(p->authorsName));
    strncpy(p->authorsSurname, fields[3], sizeof(p->authorsSurname));
    strncpy(p->category, fields[4], sizeof(p->category));
    p->borrowed.year  = atoi(fields[5]);
    p->borrowed.month = atoi(fields[6]);
    p->borrowed.day   = atoi(fields[7]);
    strncpy(p->borrowerName, fields[8], sizeof(p->borrowerName));
    strncpy(p->borrowerSurname, fields[9], sizeof(p->borrowerSurname));
    strncpy(p->notes, fields[10], sizeof(p->notes));
    p->prev=p->next=NULL;
    *out = p;
    return 1;
}

void saveToFile(void){
    if(!head){ printf("Lista jest pusta, nie zapisano.\n"); return; }
    char name[512]; read_line("Podaj nazwę pliku do zapisu: ", name, sizeof(name));
    if(name[0]==0){ printf("Anulowano.\n"); return; }
    FILE *f = fopen(name, "w");
    if(!f){ printf("Nie można otworzyć pliku do zapisu.\n"); return; }
    for(lista *p=head; p; p=p->next) write_one(f, p);
    fclose(f);
    printf("Zapisano dane do pliku \"%s\".\n", name);
}

void importFromFile(void){
    char name[512]; read_line("Podaj nazwę pliku do odczytu: ", name, sizeof(name));
    if(name[0]==0){ printf("Anulowano.\n"); return; }
    FILE *f = fopen(name, "r");
    if(!f){ printf("Nie znaleziono pliku.\n"); return; }
    // wyczyść starą listę
    freeAll();
    head = tail = NULL;
    // wczytaj nowe
    int status=0, count=0, maxid=0;
    while(1){
        lista *p=NULL;
        status = read_one(f, &p);
        if(status<=0) break;
        // dodaj na koniec
        if(p->ID>maxid) maxid=p->ID;
        p->prev = tail; p->next = NULL;
        if(tail) tail->next = p;
        tail = p; if(!head) head = p;
        count++;
    }
    fclose(f);
    next_id = maxid+1;
    if(count==0) printf("Plik był pusty lub miał zły format.\n");
    else printf("Wczytano %d rekordów z pliku \"%s\".\n", count, name);
}

void freeAll(void){
    lista *p=head;
    while(p){
        lista *n=p->next;
        free(p);
        p=n;
    }
    head = tail = NULL;
}