/*


TO-DO:
v implementacja wykrywania kol AABB
x implementacja CollisionDetectora
v extrude AABB
	temp: szukanie coll distance: 
		triangle test lini o -> simplex closest face dający V
		seek in dir V new point
- przemyslenie i implementacja sposobu na zwracanie danych do resolve collision
- dokonczenie GJK tak zeby byla mozliwa parametryzacja i wlasne klasy do wykrywania kolizji 
z mysla o kolejnych algorytmach
- implementacja wykrywania punktu kolizji (triangle ray)
przeinaczenie GJK nie do Poznania i Łodzi
- stowrzenie "ladnego" renderowania obiektow



schemat głównej pętli:
Iteracja symulacji sceny i obiektow:
	wykrycie kolizji
	przesuniecie obiektow
Update klatki animacji


schemat iteracji:
rozciągnij OBB dla szybkich obiektów
(optymalizacja sweep&prune czy czyms)
sprawdz kolizje OBB
dla kolidującej pary
	scenariusz 1(mój):
		oblicz punkt kolizji dla OBBów
		punkt kolizji rzutuj na odcinek drogi i oblicz czas kolizji dla pierwszego obiektu
		przesun obiekty o dany czas 
		sprawdz kolizje w danym czasie
			jesli nie ma kolizji przesuń w czasie do przodu
		jesli jest kolizja
			obsluz odpowiedz na kolizje
	scenariusz 2:
		rozciągnij obiekty i traktuj jak convex hulle
		sprawdz kolizje GJK na hullach
			kolizja musi nastąpić
			przesuń obiekty w odpowiednie mijesca dla kolizji wzgledem punktu kolizji
		obsluz odpowiedz na kolizje










		Notatki z instrukcji:

		Sterowanie programem:
		Kamera:
		Strza³ki - obracanie kamery
		A - przybli¿a kamere
		Z - oddala kamere
		Ustawienia:
		C - w³acz/wy³¹cz tryb wykrywania kolizji ci¹g³ej CCD
		N - w³acz/wy³¹cz faze wstepn¹ wykrywania kolizji
		D - w³acz/wy³¹cz wyswietlanie obiektów sceny
		R - nadaje wszystkim obiektom losow¹ predkosc w losowym kierunku

		T - Wy³¹cza aplikacje po 650 iteracjach(uzywane do testow) od momentu wcisniecia.
		M - zatrzymuje symulacje i powozwala klatkowaæ wykonywanie programu
		N - w³¹cza spowrotem symulacje w czasie rzeczywistym, jeœli wczeœniej by³o wykorzystywane klatkowanie.
		Dodawanie obiektów do sceny(u¿ywane przy testach):	S - zwieksza argument dla funkcji dodaj¹cych obiekty(zakres 1-5)
		X - zmniejsza argument dla funkcji dodaj¹cych obiekty
		0 - tworzy ogromny szeœcian zapobiegaj¹cy uciekaniu obiektów
		1 - dodaje 25*argument wieloœcianów niskiej z³o¿onoœci do sceny
		2 - dodaje 50 wieloœcianów o z³o¿onoœci zale¿nej od zmiennej argument
		4 - dodaje 10*argument wieloœcianów niskiej z³o¿onoœci do sceny w losowym miejscu z losow¹ predkosci¹
		5 - to samo co opcja '2' tylko dla argumentu 5(UWAGA bardzo powolne!)
		P - usuwa wszystkie obiekty ze sceny

		Ustawienia najlepiej zmieniac dopiero, gdy na scenie juz znajduja sie obiekty.























*/