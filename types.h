#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>

#define MAX_EVID_BR 6
#define MAX_NAZIV 30
#define MAX_ADRESA 50
#define MAX_KOL 10

#define FAKTOR_BAKETIRANJA 3
#define FAKTOR_BAKETIRANJA_PREKOR 2
#define BR_BAKETA 7
#define BR_LOKACIJA 21 //Q = bB = 21

typedef struct
{
    char naziv[MAX_NAZIV];
    FILE *f;
} Datoteka;

typedef struct slogovi
{
    unsigned long evidencioni_broj; //maks 6 cifara
    char vrsta_pizze[MAX_NAZIV];
    char adresa[MAX_ADRESA];
    int kolicina;
    int status;
} Slog;

typedef struct baketi
{
    Slog slogovi[FAKTOR_BAKETIRANJA];
} Baket;
#endif // TYPES_H
