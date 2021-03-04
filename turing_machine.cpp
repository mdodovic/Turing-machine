#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

//NOVA STRUKTURA KOJA PREDSTAVLJA CELIJU TRAKE
typedef struct elem
{
	char podatak;
	struct elem *sledeci, *prethodni;
};

//NOVA STRUKTURA KOJA PREDSTAVLJA JEDNU FUNKCIJU
typedef struct function
{
	int trenutno_stanje, novo_stanje;
	char trenutni_znak, novi_znak, pomeranje_glave, moguce_stanje;
	struct function *sledeca;
};

//FUNKCIJA KOJA PRETVARA char BROJA U int
int vrati_broj(char c)
{
	return c - '0';
}

//FUNKCIJA ZA UCITAVANJE TRAKE IZ TEKSTUALNE DATOTEKE traka.txt
elem* ucitaj_traku(FILE *traka)
{
	elem *prvi = NULL, *novi, *posl = NULL;
	char c = fgetc(traka);

	while (c != EOF)
	{
		novi = (struct elem *) malloc(sizeof(elem));
		if (!novi) exit(1);
		novi->podatak = c;
		novi->sledeci = NULL;
		novi->prethodni = posl;
		if (prvi == NULL) prvi = novi;
		else posl->sledeci = novi;
		posl = novi;

		c = getc(traka);
	}
	return prvi;
}

//FUNKCIJA ZA UCITAVANJE FUNKCIJA TM IZ TEKSTUALNE DATOTEKE funkcije.txt
function* ucitaj_funkcije(FILE *funkcije)
{
	function *prva = NULL, *nova, *posl = NULL;
	char c;
	int broj = 0;

	while ((c = getc(funkcije)) != EOF)
	{
		nova = (struct function*) malloc(sizeof(function));
		if (!nova) exit(2);

		getc(funkcije);
		getc(funkcije);
		while ((c = getc(funkcije)) != ',')
		{
			broj = broj * 10 + vrati_broj(c);
		}
		nova->trenutno_stanje = broj;
		broj = 0;
		nova->trenutni_znak = getc(funkcije);
		getc(funkcije);
		getc(funkcije);
		getc(funkcije);
		getc(funkcije);
		while ((c = getc(funkcije)) != ',')
		{
			if (c == '+' || c == '-')
			{
				nova->moguce_stanje = c;
				break;
			}
			broj = broj * 10 + vrati_broj(c);
		}
		if (c == '+' || c == '-') getc(funkcije);
		nova->novo_stanje = broj;
		broj = 0;
		nova->novi_znak = getc(funkcije);
		getc(funkcije);
		nova->pomeranje_glave = getc(funkcije);
		getc(funkcije);
		getc(funkcije);
		getc(funkcije);
		nova->sledeca = NULL;
		if (!prva) prva = nova;
		else posl->sledeca = nova;
		posl = nova;
	}

	return prva;
}

//FUNKCIJA ZA ISPISIVANJE TRAKE
void ispisi_traku(elem *prvi)
{

	elem *trenutni = prvi;
	while (trenutni != NULL)
	{
		printf("%c", trenutni->podatak);
		trenutni = trenutni->sledeci;
	}
}

//FUNKCIJA ZA DODAVANJE ELEMENTA b NA POCETAK TRAKE
void dodaj_na_traku_pocetak(elem **elementi)
{
	elem* novi = new elem;
	novi->sledeci = *elementi;
	novi->prethodni = NULL;
	novi->podatak = 'b';
	*elementi = novi;
}

//FUNKCIJA ZA DODAVANJE ELEMENTA b NA KRAJ TRAKE
void dodaj_na_traku_kraj(elem *elementi)
{
	elem* novi = new elem;
	elementi->sledeci = novi;
	novi->sledeci = NULL;
	novi->prethodni = elementi;
	novi->podatak = 'b';
}


//FUNKCIJA ZA IZVRSAVANJE PROGRAMA TM
void izvrsi_program(elem *elementi, function *funkcije)
{
	int stanje = 0;
	function *trenutna_funkcija = funkcije;
	elem *glava = elementi->sledeci;
	bool kraj = false;

	while (!kraj)
	{
		if (trenutna_funkcija->trenutno_stanje == stanje
			&& trenutna_funkcija->trenutni_znak == glava->podatak)
		{
			if (trenutna_funkcija->moguce_stanje == '+' || trenutna_funkcija->moguce_stanje == '-')
			{
				printf("Funkcija koja se sledeca izvrsava je: f(q%d,%c)=(q%c,%c,%c1)\n", trenutna_funkcija->trenutno_stanje, trenutna_funkcija->trenutni_znak, trenutna_funkcija->moguce_stanje, trenutna_funkcija->novi_znak, trenutna_funkcija->pomeranje_glave);
			}
			else
			{
				printf("Funkcija koja se sledeca izvrsava je: f(q%d,%c)=(q%d,%c,%c1)\n", trenutna_funkcija->trenutno_stanje, trenutna_funkcija->trenutni_znak, trenutna_funkcija->novo_stanje, trenutna_funkcija->novi_znak, trenutna_funkcija->pomeranje_glave);
			}

			stanje = trenutna_funkcija->novo_stanje;
			glava->podatak = trenutna_funkcija->novi_znak;
			if (trenutna_funkcija->pomeranje_glave == '+') {
				if (glava->sledeci == NULL) dodaj_na_traku_kraj(glava);
				glava = glava->sledeci;
			}
			else {
				if (glava->prethodni == NULL) {
					ispisi_traku(elementi); cout << endl;
					dodaj_na_traku_pocetak(&glava);
					glava = glava->sledeci;
				}
				glava = glava->prethodni;
			}
			ispisi_traku(elementi);
			putchar('\n');
			if (trenutna_funkcija->moguce_stanje == '+') {
				break;
			}
			else if (trenutna_funkcija->moguce_stanje == '-')
			{
				printf("Greska na traci!");
				break;
			}

			trenutna_funkcija = funkcije;
		}
		else trenutna_funkcija = trenutna_funkcija->sledeca;
	}

}


int main()
{

	FILE *program, *traka;
	program = fopen("program.txt", "r");
	traka = fopen("line.txt", "r");


	elem *prva_celija;
	function *prva_funkcija;

	prva_celija = ucitaj_traku(traka); // ucitavanje trake
	ispisi_traku(prva_celija); // ispis pocetne trake
	cout << endl;
	prva_funkcija = ucitaj_funkcije(program); // ucitavanje programa TM

	izvrsi_program(prva_celija, prva_funkcija); // izvrsavanje programa TM


	fclose(program);
	fclose(traka);
	getchar();
	return 0;

}