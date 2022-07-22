# spanzuratoarea-arduino


In codul de Arduino ne folosim in partea de pregatire
a proiectului (setup()):
- ne folosim de o valoare de la un pin neconectat
pentru a genera indexul unui cuvant de citit.
- initializam pinii specializati pentru tastatura(liniile si coloane)
- ne vom folosi de o variabila prevPressedButton pentru a retine
ce buton a fost retinut anterior
- pentru a afisa un text la ecranul LCD
- tastatura va fi similara celei de la un telefon Nokia mai vechi;
cand apesi o tasta aceasta poate selecta pana la 3 litere(cu exceptia
tastei 7 care poate cicla intre literele VWXYZ si ENTER).
- De asemenea am creat un vector de cuvinte

Functia displayMessage() iti afiseaza fractionat un mesaj mai lung la LCD,
iar intre bucatile de mesaj se impune un delay de cateva milisecunde
pentru a avea timp utilizatorul sa citeasca


In loop():
- Pentru a incepe jocul trebuie sa apesi ENTER, iar starea jocului
(start_game trece in 1). De-asemenea, cand se termina jocul se trece in
starea 2, unde se asteapta din nou apasarea lui ENTER(si ulterior trece in 3).

Dupa ce s-a inceput jocul, pentru a utiliza tastatura, se ia fiecare linie
de pe tastatura si se trece pe LOW, iar celelalte randuri ale tastaturii raman 
pe HIGH. Daca o tasta este apasata, o anume coloana trece pe LOW si asa se stie ca 
s-a apasat un buton. Cu acest input dat programului, avem urmatoarele conditii:
 - se apasa un buton diferit ENTER(8) si jocul e pornit:
	atunci se afiseaza pe ecranul LCD caracterul selectat, functia selectLetter()
 - se apasa butonul ENTER si jocul nu e pornit:
	atunci se afiseaza doar underscore la lcd
 - daca jocul e pornit si se apasa ENTER:
	- se verifica daca litera face parte din cuvant
	- daca ai ghicit, se arata literele din cuvant, eventual daca sunt ghicite toate
	se afiseaza un mesaj cum ca jocul a fost castigat
	- mai mult, daca nu mai ai vieti se termina jocul.
