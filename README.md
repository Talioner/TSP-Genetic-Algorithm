# TSP-Genetic-Algorithm
Algorytm polega na stworzeniu populacji rozwiązań problemu. Najlepsze rozwiązania tej
populacji zostaną ze sobą połączone w celu stworzenia nowej populacji i uzyskania lepszych
rozwiązań.
W swoim algorytmie przyjąłem stały rozmiar populacji - P, a ich członków stworzyłem losowo
(to znaczy, że każdy członek jest trasą z losowo ułożonych wierzchołków). Następnie z tej
populacji wybierani są rodzice, którzy stworzą następną generację. Rodzice są wybierani
metodą turniejową. Turniej polega na walce dwóch członków populacji, gdzie wygranym jest
ten, o mniejszym koszcie trasy. Jest P turniejów, przy czym jeden członek może wylosowany
do turnieju kilka razy, ale jeśli zwycięży i już wcześniej był rodzicem, to nie dodajemy go kolejny
raz do tablicy rodziców. Tym sposobem ilość rodziców może być zmienna. Kolejnym krokiem
jest dobranie rodziców w pary i stworzenie dziecka. Dziecko jest tworzone metodą Order
Corssover (OX). Polega ona na wylosowaniu punktów przecięć, takich samych dla obu
rodziców, następnie przepisanie do dziecka ścieżki między punktami przecięć z pierwszego
rodzica i uzupełnienie reszty kolejnymi wierzchołkami z drugiego rodzica zaczynając od
drugiego punktu przecięcia w prawo, tak aby wierzchołki się nie powtarzały. W moim
algorytmie tworzę P-1 dzieci, które tworzą nową generację, a P-tym członkiem jest najlepszy
członek generacji poprzedniej (elitaryzm). Po stworzeniu pojedynczego dziecka jest 0,1% szans
na jego mutację, która polega na zamianie miejscami jego dwóch losowych wierzchołków. W
każdej nowej generacji jest sprawdzane najlepsze rozwiązanie. Następnie algorytm jest
wykonywany kolejny raz od wybrania rodziców do utworzenia nowej generacji. Złożoność
obliczeniowa tego algorytmu zależna jest w od rozmiaru instancji N, rozmiaru populacji P i
ilości iteracji. W najgorszym przypadku jest to P * N, ale P ze względu na to, że P jest w moim
algorytmie takie same niezależnie od wielkości instancji, złożoność to O(N).
Macierz kosztów jest przechowywana jako dwuwymiarowy vector typu int. Wszyscy
członkowie populacji są przechowywani jako tablice dynamiczne jednowymiarowe. Zbiory
członków (populacja, rodzice, dzieci) są przechowywane tablice dynamiczne dwuwymiarowe.
