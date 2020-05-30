#ifndef PIZZA_H
#define PIZZA_H
#include <stdio.h>
#include "types.h"

void form_ras_dat(Datoteka *d);
void form_praz_prim_zone(Datoteka *d);
void form_praz_zone_prekor(Datoteka *d);
void izbor_aktivne(Datoteka *d);
void zatvaranje_datoteke(Datoteka *d);
void prikaz_naziva_aktivne_dat(Datoteka *d);

void dodaj_slog(Datoteka *d);
int trazenje_u_ras_dat(Datoteka *d, unsigned long evid_br);
int transformacija_sred_cif_kvad(unsigned long evid_br);
void ucitaj_sa_adrese(Datoteka *d, int adresa, Baket *baket);
void upisi_u_adresu(Datoteka *d, int adresa, Baket *baket);

void nadji_slog(Datoteka *d);
void prikazi_sve_slogove(Datoteka *d);
void logicko_brisanje(Datoteka *d);
void izmena_adrese(Datoteka *d);


#endif // PIZZA_H
