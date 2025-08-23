# library-system
Projekt zaliczeniowy przedmiotu Podstawy Programowania na kierunku Bioinformatyka w roku 2020/21.


## Treść projektu:

Należy napisać program, który pozwala na wprowadzenie i przechowywanie danych o książkach. Dane powinny być przechowywane na liście dwukierunkowej, a lista ta zawiera obiekty struktury _Ksiazka_.
Struktura _Ksiazka_ zawiera pola przechowujące poniższe dane:
1. ID (unikalna liczba całkowita - przypisywane automatycznie przez program, najlepiej od 1)
2. Tytuł książki
3. Imię autora
4. Nazwisko autora
5. Kategoria
6. Data wypożyczenia (jako wewnętrzna struktura Date zwierająca rok/miesiąc/dzień)
7. Imię tego, kto wypożyczył
8. Nazwisko tego, kto wypożyczył
9. Inne dane
10. 2 wskaźniki na inne struktury typu Ksiazka (na poprzedni i następny obiekt na liście dwukierunkowej)

Program pamięta wskaźnik na pierwszy element listy, ale także osobny wskaźnik wskazujący zawsze na ostatni dodany element listy (gdzie element, to oczywiście obiekt struktury _Ksiazka_).

Program wyświetla następujące menu:
1. Dodaj nową książkę
2. Usuń książkę
3. Edytuj dane książki
4. Zmień status książki
5. Wyszukaj książkę
6. Wyświetl bazę po tytułach
7. Wyświetl książki wypożyczone
8. Zapisz do pliku
9. Odczytaj z pliku
10. X) Zakończ program

### Opis poszczególnych części:

1. Dodaj nową książkę: funkcja tworzy nowy obiekt książki, wcześniej pytając o tytuł, kategorię i dane autora. Program automatycznie przypisuje pierwszy wolny ID do nowego obiektu, który następnie dodawany jest na koniec listy książek (lub jako pierwszy element, gdy lista jest jeszcze pusta). Po dodaniu nowej książki program pyta, czy ma wrócić do głównego menu, czy dodać nową książkę.
2. Usuń książkę: funkcja najpierw program pyta czy usuwanie ma być po ID czy po tytule. Następnie prosi o odpowiednie dane (ID lub tytuł) i usuwa książkę z listy. Gdy książki o podanym ID/tytule nie ma w ogóle na liście - wyświetla odpowiedni komunikat i wraca do głównego menu.
3. Edytuj dane książki: podobnie jak w przypadku usuwania, funkcja najpierw program pyta o ID książki lub jej tytuł. Następnie prosi o wprowadzenie tych samych danych co przy dodawaniu. UWAGA: Jeśli np. program właśnie pyta o którąś informację, a użytkownik po prostu wciśnie Enter bez wprowadzania czegokolwiek nowego, stary ciąg danych pozostaje. Np. program pyta o nowe Nazwisko autora, wciskamy Enter - w danych książki pozostaje stare nazwisko.
4. Zmień status książki: funkcja prosi o wprowadzenie ID lub tytuł książki. Jeśli książka nie jest wypożyczona, program od razu pyta o dane wypożyczającego i datę wypożyczenia. Jeśli książka jest już przez kogoś wypożyczona, program najpierw wypisuje dane książki i tego, kto pożyczył, a następnie pyta, czy ma zmienić status książki na oddaną - oznacza to usunięcie daty wypożyczenia oraz danych wypożyczającego z obiektu danych o książce.
5. Wyszukaj książkę: funkcja umożliwia wyszukiwanie po ID lub tytule lub nazwisku wypożyczającego - wyświetla pełne dane książki/książek (w przypadku, gdy jedna osoba wypożyczyła więcej niż 1 książkę).
6. Wyświetl bazę po tytułach: funkcja wyświetla bazę po tytułach z zachowaniem informacji o danych autora i kategorii książki.
7. Wyświetl książki wypożyczone: funkcja wyświetla książki wypożyczone z informacją kto wypożyczył daną książkę.
8. Zapisz do pliku: funkcja prosi o podanie nazwy pliku, jeśli plik już istnieje to jego stara zawartość jest zastępowana nową. Zapisywane są aktualne dane z listy.
9. Odczytaj z pliku: funkcja prosi o podanie nazwy pliku, jeśli taki plik istnieje to usuwa całą aktualną zawartość pamięci (strukturę) oraz tworzy nową, na bazie tego, co odczytał z pliku.
