#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "pizza.h"

void showMenu();

int main()
{
    int action;
    Datoteka aktivna;
    aktivna.f = NULL;

    do
    {
        do
        {
            showMenu();
            scanf("%d",&action);
            switch(action)
            {
            case 1:
                form_ras_dat(&aktivna);
                break;
            case 2:
                izbor_aktivne(&aktivna);
                break;
            case 3:
                prikaz_naziva_aktivne_dat(&aktivna);
                break;
            case 4:
                dodaj_slog(&aktivna);
                break;
            case 5:
                nadji_slog(&aktivna);
                break;
            case 6:
                prikazi_sve_slogove(&aktivna);
                break;
            case 7:
                logicko_brisanje(&aktivna);
                break;
            case 8:
                izmena_adrese(&aktivna);
                break;
            case 9:
                zatvaranje_datoteke(&aktivna);
                break;
            default:
                printf("Pogresan unos. Probajte ponovo\n");
            }
        }while(action < 1 || action > 9);
    }while(action != 9);
    return 0;
}

void showMenu()
{
    printf("\n---------MENI---------\n");
    printf("1. Formirajte praznu datoteku.\n");
    printf("2. Izaberite aktivnu datoteku.\n");
    printf("3. Prikazite naziv aktivne datoteke.\n");
    printf("4. Dodajte slog.\n");
    printf("5. Pretrazite slog.\n");
    printf("6. Prikazite sve slogove.\n");
    printf("7. Logicko brisanje.\n");
    printf("8. Izmena adrese.\n");
    printf("9. Izlaz.\n");
    printf("---------------------\n");
}
