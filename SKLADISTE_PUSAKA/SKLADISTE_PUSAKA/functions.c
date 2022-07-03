#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataType.h"
#include "functions.h"

static int brojClanova = 0;

void kreiranjeDatoteke(const char* const ime) {

	FILE* fp = fopen(ime, "wb");

	if (fp == NULL) {
		perror("Kreiranje datoteke puske.bin");
		exit(EXIT_FAILURE);
	}

	fwrite(&brojClanova, sizeof(int), 1, fp);

	printf("Kreirana datoteka puske.bin\n\n");
	fclose(fp);

}

void dodajKorisnika(const char* const ime) {

	FILE* fp = fopen(ime, "rb+");

	if (fp == NULL) {
		perror("Dodavanje puske u datoteku clanovi.bin");
		exit(EXIT_FAILURE);
	}

	//citamo prvi red datoteke i zapisujemo broj clanova u varijablu brojClanova
	fread(&brojClanova, sizeof(int), 1, fp);
	printf("Broj pusaka: %d\n\n", brojClanova);

	CLAN temp = { 0 };
	temp.id = brojClanova;
	getchar();

	printf("Unesite ime puske: ");
	scanf("%24[^\n]", temp.imePuske);
	getchar();

	printf("Unesite kalibar puske: ");
	scanf("%24[^\n]", temp.kalibar);
	getchar();

	printf("Unesite domet: ");
	scanf("%49[^\n]", temp.domet);
	getchar();

	printf("Unesite brzinu metka: ");
	scanf("%10[^\n]", temp.brzinaMetka);

	//pomicemo se na kraj datoteke i zapisujemo novog clana tamo
	fseek(fp, sizeof(CLAN) * brojClanova, SEEK_CUR);
	fwrite(&temp, sizeof(CLAN), 1, fp);
	printf("Nova puska dodana.\n\n");

	// povratak na pocetak datoteke
	rewind(fp);
	brojClanova++;

	// zapis novog broja clanova
	fwrite(&brojClanova, sizeof(int), 1, fp);

	fclose(fp);
}

void* ucitajKorisnike(const char* const ime) {

	FILE* fp = fopen(ime, "rb");

	if (fp == NULL) {
		perror("Ucitavanje korisnika iz clanovi.bin");
		return NULL;
		exit(EXIT_FAILURE);
	}

	fread(&brojClanova, sizeof(int), 1, fp);
	printf("Broj clanova: %d\n", brojClanova);

	CLAN* poljeClanova = (CLAN*)calloc(brojClanova, sizeof(CLAN));

	if (poljeClanova == NULL) {
		perror("Zauzimanje memorije za studente");
		return NULL;
		exit(EXIT_FAILURE);
	}

	fread(poljeClanova, sizeof(CLAN), brojClanova, fp);

	printf("Svi clanovi uspjesno ucitani.\n\n");

	return poljeClanova;
}

void ispisiSveKorisnike(const CLAN* const polje) {

	if (polje == NULL) {
		printf("Polje clanova prazno\n");
		return;
	}

	int i;

	for (i = 0; i < brojClanova; i++) {
		printf("Puska broj %d\nID: %d\nIme puske: %s\nKalibar: %s mm\nDomet: %s m\nBrzina metka %s m/s \n\n",
			i + 1,
			(polje + i)->id,
			(polje + i)->imePuske,
			(polje + i)->kalibar,
			(polje + i)->domet,
			(polje + i)->brzinaMetka);
	}
}

void* pretraziClanove(CLAN* const polje) {

	if (polje == NULL) {
		printf("Polje clanova prazno\n");
		return;
	}

	int i;

	int trazeniId;

	printf("Unesite ID clana kojeg trazite: \n");
	scanf("%d", &trazeniId);

	// ako nademo clana vracamo cijeli element polja s trazenim id-em
	for (i = 0; i < brojClanova; i++) {

		if (trazeniId == (polje + i)->id) {
			printf("Clan pronaden.\n");

			return (polje + i);
		}
	}

	// vracamo NULL u slucaju da ne nademo nijednog clana s tim id
	return NULL;
}

int izlazIzPrograma(CLAN* polje) {

	free(polje);

	return 0;
}