/*


TO-DO:
v implementacja wykrywania kol AABB
x implementacja CollisionDetectora
v extrude AABB
	temp: szukanie coll distance: 
		triangle test lini o -> simplex closest face daj�cy V
		seek in dir V new point
- przemyslenie i implementacja sposobu na zwracanie danych do resolve collision
- dokonczenie GJK tak zeby byla mozliwa parametryzacja i wlasne klasy do wykrywania kolizji 
z mysla o kolejnych algorytmach
- implementacja wykrywania punktu kolizji (triangle ray)
przeinaczenie GJK nie do Poznania i �odzi
- stowrzenie "ladnego" renderowania obiektow



schemat g��wnej p�tli:
Iteracja symulacji sceny i obiektow:
	wykrycie kolizji
	przesuniecie obiektow
Update klatki animacji


schemat iteracji:
rozci�gnij OBB dla szybkich obiekt�w
(optymalizacja sweep&prune czy czyms)
sprawdz kolizje OBB
dla koliduj�cej pary
	scenariusz 1(m�j):
		oblicz punkt kolizji dla OBB�w
		punkt kolizji rzutuj na odcinek drogi i oblicz czas kolizji dla pierwszego obiektu
		przesun obiekty o dany czas 
		sprawdz kolizje w danym czasie
			jesli nie ma kolizji przesu� w czasie do przodu
		jesli jest kolizja
			obsluz odpowiedz na kolizje
	scenariusz 2:
		rozci�gnij obiekty i traktuj jak convex hulle
		sprawdz kolizje GJK na hullach
			kolizja musi nast�pi�
			przesu� obiekty w odpowiednie mijesca dla kolizji wzgledem punktu kolizji
		obsluz odpowiedz na kolizje


































*/