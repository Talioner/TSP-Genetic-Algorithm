# TSP-Genetic-Algorithm
Macierz kosztów jest przechowywana jako dwuwymiarowy vector typu int. Do przechowywania listy
tabu i drogi wykorzystałem tablice dynamiczne. Zaimplementowany przeze mnie algorytm działa na
zasadzie sprawdzania najbliższego sąsiedztwa. Na samym początku generowana jest losowa ścieżka,
dla której liczony jest koszt przejścia przez wszystkie wierzchołki i powrotu do wierzchołka startowego.
W tym czasie jest to nasze najlepsze rozwiązanie. Kolejnym krokiem jest przeszukiwanie lokalne, które
odbywa się przy pomocy funkcji swap zamieniającej wierzchołki i z j. Zapisywany jest koszt tej zamiany
i jeśli jest ona lepsza niż poprzednia zamiana to jest to nowa najlepsza zmiana sąsiedztwa. W danym
momencie gdy znajdziemy już najlepszą zmianę sąsiedztwa, ruch ten zapisywany jest w liście ruchów
zakazanych(tabu list). Jako warunek tabu przyjąłem, że jeśli zamieniamy wierzchołek „i” z
wierzchołkiem „j” (i < j), to po zamianie wierzchołek o indeksie „j” nie może zamienić się z
wierzchołkiem o mniejszym indeksie. Przechodząc kilka razy ten proces możemy natknąć się na
sytuacje gdy dany ruch dałby lepszy koszt zamiany, a zarazem zoptymalizowałby wynik, lecz ruch ten
znajduje się na liście tabu. W tym momencie wykorzystujemy kryterium aspiracji, czyli warunek
pozwalający zignorować listę tabu, jeśli ruch ten daje najlepszy dotychczasowy koszt drogi. Za każdym
razem sprawdzany jest nowy wynik, gdy jest on lepszy od aktualnego najlepszego, staje się on nowym
najlepszym wynikiem. Warto dodać, że ruchy dodawane do listy tabu są tam przez określony czas
(podaną kadencję). Kadencja jest to wartość, która w kolejnych poszukiwaniach najlepszej zamiany
zostaje dekrementowana, aż do osiągnięcia 0, w tym czasie dany ruch wraca do obiegu. W algorytmie
wykorzystałem operator modulo (%tabuSize), aby przy pomocy kolejnych iteracji głównej pętli
odwoływać się do elementów listy tabu, tym samym uniknąłem osobnego liczenia kadencji (elementy
listy tabu są nadpisywane po liczbie obrotów pętli odpowiadającej rozmiarowi listy). Kolejną ważną
sprawą jest skorzystanie z kryterium dywersyfikacji. Kryterium to jest stosowane po to aby spróbować 
poprawić nasz aktualny wynik. Gdy wynik przez pewien czas nie poprawia się, nasze parametry są
resetowane, oczywiście oprócz aktualnego minimum. Algorytm kończy działanie po wykonaniu zadanej
ilości iteracji. Złożoność tego algorytmu to O(n^3), ponieważ wykonując zamiany wierzchołków każdy
z każdym stworzyłem podwójną pętlę od 0 do n. A do tego sprawdzany jest koszt każdej zamiany na
podstawie macierzy kosztów, która również ma rozmiar n.
