/*


TO-DO:
v implementacja wykrywania kol AABB
x implementacja CollisionDetectora
v extrude AABB
	temp: szukanie coll distance: 
		triangle test lini o -> simplex closest face daj¹cy V
		seek in dir V new point
- przemyslenie i implementacja sposobu na zwracanie danych do resolve collision
- dokonczenie GJK tak zeby byla mozliwa parametryzacja i wlasne klasy do wykrywania kolizji 
z mysla o kolejnych algorytmach
- implementacja wykrywania punktu kolizji (triangle ray)
przeinaczenie GJK nie do Poznania i £odzi
- stowrzenie "ladnego" renderowania obiektow



schemat g³ównej pêtli:
Iteracja symulacji sceny i obiektow:
	wykrycie kolizji
	przesuniecie obiektow
Update klatki animacji


schemat iteracji:
rozci¹gnij OBB dla szybkich obiektów
(optymalizacja sweep&prune czy czyms)
sprawdz kolizje OBB
dla koliduj¹cej pary
	scenariusz 1(mój):
		oblicz punkt kolizji dla OBBów
		punkt kolizji rzutuj na odcinek drogi i oblicz czas kolizji dla pierwszego obiektu
		przesun obiekty o dany czas 
		sprawdz kolizje w danym czasie
			jesli nie ma kolizji przesuñ w czasie do przodu
		jesli jest kolizja
			obsluz odpowiedz na kolizje
	scenariusz 2:
		rozci¹gnij obiekty i traktuj jak convex hulle
		sprawdz kolizje GJK na hullach
			kolizja musi nast¹piæ
			przesuñ obiekty w odpowiednie mijesca dla kolizji wzgledem punktu kolizji
		obsluz odpowiedz na kolizje


































*/