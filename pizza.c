#include "pizza.h"
#include "types.h"
#include <stdio.h>
#include <math.h>
#include <string.h>


#define SLOBODAN -1
#define ZAUZET 1
#define LOGICKI_OBRISAN 2
#define U_PRIM_ZONI 0
#define U_ZONI_PREK 1
#define NEUSPESNO_TRAZENJE 2
#define PREKORACIO 1
#define UPISAN_U_MAT 0
#define UPISAN_U_PREK 2


#define POC_ZONE_PREKOR 8
#define KRAJ_ZONE_PREKOR 15

void form_ras_dat(Datoteka *d)
{
    printf("Unesite naziv datoteke: \n");
    char extension[5] = ".bin";
    fflush(stdin);
    gets(d->naziv);

    strcat(d->naziv,extension);


    d->f = fopen(d->naziv,"wb+");
    if(d->f == NULL)
    {
        printf("Greska prilikom kreiranja datoteke %s \n", d->naziv);

    }
    else
    {
        form_praz_prim_zone(d);
        form_praz_zone_prekor(d);
        printf("Datoteka '%s' je uspesno kreirana!\n",d->naziv);
    }
    //fclose(d->f);
}

void form_praz_prim_zone(Datoteka *d)
{
    printf("Formiranje prazne primarne zone ... \n");
    Baket bak;
    int i,j;

    for(i = 1; i <= BR_BAKETA; i++)
    {
        for(j = 0; j < FAKTOR_BAKETIRANJA; j++)
        {
            bak.slogovi[j].status = SLOBODAN;
        }
        //fseek(d->f,(i-1)*sizeof(Baket),SEEK_SET);
        //fwrite(&bak,sizeof(Baket),1,d->f);
        //ide od 1 do 7
        upisi_u_adresu(d,i,&bak);
    }
}

//formiranje zone prekoracenja sa 7 baketa faktora baketiranja 2
void form_praz_zone_prekor(Datoteka *d)
{
    printf("Formiranje prazne zone prekoracenja ... \n");
    Baket bak;
    int i,j;

    for(i = POC_ZONE_PREKOR ; i <= KRAJ_ZONE_PREKOR; i++)
    {
        for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
        {
            bak.slogovi[j].status = SLOBODAN;
        }
        upisi_u_adresu(d,i,&bak);
    }
}

void izbor_aktivne(Datoteka *d)
{
    if(d->f != NULL)
    {
        //postoji vec otvorena datoteka
        fclose(d->f);
    }

    printf("Unesite naziv datoteke (sa ekstenzijom) koju zelite da otvorite.\n");
    scanf("%s",d->naziv);
    d->f = fopen(d->naziv,"rb+");
    if(d->f == NULL)
    {
        printf("Greska pri otvoranju datoteke, ili ne postoji trazena datoteka '%s'.\n",d->naziv);
    }
    else
        printf("Uspesno otvorena datoteka '%s'! \n",d->naziv);
}

void prikaz_naziva_aktivne_dat(Datoteka *d)
{
    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka.\n");
    }
    else
    {
        printf("Naziv aktivne datoteke je: '%s' \n",d->naziv);
    }
}

void zatvaranje_datoteke(Datoteka *d)
{
    if(d->f != NULL)
    {
        fclose(d->f);
    }
}

void dodaj_slog(Datoteka *d)
{
    int i,j,ind1;
    Slog s;
    Baket bak;

    int flag = PREKORACIO;

    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka.\n");
        return;
    }

    printf("---DODAVANJE SLOGA---\n\n");

    do
    {
        printf("Evidencioni broj: \n");
        scanf("%lu",&s.evidencioni_broj);
    }while(s.evidencioni_broj > 999999 ||  s.evidencioni_broj < 100000);


    ind1 = trazenje_u_ras_dat(d,s.evidencioni_broj);

    if(ind1 == U_PRIM_ZONI || ind1 == U_ZONI_PREK)
    {
        printf("Slog sa evidencionim brojem %lu vec postoji u aktivnoj datoteci '%s'.",s.evidencioni_broj,d->naziv);
        return;
    }
    else
    {
        printf("Vrsta pizze: \n");
        fflush(stdin);
        gets(s.vrsta_pizze);

        printf("Adresa: \n");
        fflush(stdin);
        gets(s.adresa);

        do
        {
            printf("Kolicina: (najvise 10) \n");
            scanf("%d",&s.kolicina);
        }while(s.kolicina > 10 || s.kolicina <= 0);

        s.status = ZAUZET;

        //transformisem evid_br u adresu baketa
        int kljuc = transformacija_sred_cif_kvad(s.evidencioni_broj);

        //ucitavam baket
        ucitaj_sa_adrese(d,kljuc,&bak);

        //upisujem slog u baket
        for(i = 0; i < FAKTOR_BAKETIRANJA; i++)
        {
            if(bak.slogovi[i].status == SLOBODAN)
            {
                printf("Upisujem slog u primarnu zonu!\n");
                memcpy(&bak.slogovi[i],&s,sizeof(Slog));
                flag = UPISAN_U_MAT; //upisan u maticni baket
                upisi_u_adresu(d,kljuc,&bak);
                return;
            }
        }


        //int flag2 = 0;
        if(flag == PREKORACIO)
        {
            printf("Prekoracenje u baketu %d\n",kljuc);

            Baket bak_prek;
            for(i = POC_ZONE_PREKOR; i <= KRAJ_ZONE_PREKOR; i++)
            {
                ucitaj_sa_adrese(d,i,&bak_prek);
                for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
                {
                    printf("bak_prek status: %d\n",bak_prek.slogovi[j].status);
                    if(bak_prek.slogovi[j].status == SLOBODAN)
                    {
                        printf("Upisujem slog u zonu prekoracenja!\n");
                        memcpy(&bak_prek.slogovi[j],&s,sizeof(Slog));
                        flag = UPISAN_U_PREK;
                        break;
                    }
                }

                if(flag == UPISAN_U_PREK)
                {
                    upisi_u_adresu(d,i,&bak_prek);
                    return;
                }

            }

        }

        if(flag != UPISAN_U_PREK)
        {
            printf("Nedostatak prostora u zoni prekoracenja!\n");
            return;
        }

    }

}

//algoritam za hesiranje
int transformacija_sred_cif_kvad(unsigned long evid_br)
{
    int p,v,ret;
    double k,n,t,r;
    p = 6; // p je maks br cifara kljuca,tj evid broja = 6
    v = 10; // v predstavlja broj dozvoljenih vr, tj br cifara od 0-9 = 10.

    n = ceil(log10(BR_BAKETA)); //n = log za osnovu v od Q
    t = p - n/2;
    k = pow(evid_br,2);
    r = (k/pow(v,t)); //r je rel adresa mat baketa
    r = (long long)r % (long long)pow(v,n);
    r = r*BR_BAKETA/(pow(v,n)) + 1;

    ret = (int)floor(r);
    return ret;
}


void ucitaj_sa_adrese(Datoteka *d, int adresa, Baket *baket)
{
    //printf("Ucitavanje\n");
    int seek_flag,read_flag;

    seek_flag = fseek(d->f,(adresa-1)*sizeof(Baket),SEEK_SET);

    if(seek_flag != 0)
    {
        printf("Greska pri pozicioniranju datoteke %s\n", d->naziv);
    }

    read_flag = fread(baket,sizeof(Baket),1,d->f);
    if(read_flag != 1)
    {
        printf("Greska pri citanju datoteke '%s'\n", d->naziv);
    }
}

void upisi_u_adresu(Datoteka *d, int adresa, Baket *baket)
{
    //printf("Upisivanje\n");
    int seek_flag,write_flag;

    seek_flag = fseek(d->f,(adresa-1)*sizeof(Baket),SEEK_SET);

    if(seek_flag != 0)
    {
        printf("Greska pri pozicioniranju datoteke %s\n", d->naziv);
    }

    write_flag = fwrite(baket,sizeof(Baket),1,d->f);
    //treba da vrati br upisa, tj 1
    if(write_flag != 1)
    {
        printf("Greska pri upisivanju u datoteku '%s'\n", d->naziv);
    }
}

int trazenje_u_ras_dat(Datoteka *d, unsigned long evid_br)
{
    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka\n");
        return 0;
    }
    int ind,i,j;
    Baket bak;
    //promena kljuca u adresu
    int adresa = transformacija_sred_cif_kvad(evid_br);
    ucitaj_sa_adrese(d,adresa,&bak);

    //trazenje u maticnom baketu
    for(i = 0; i < FAKTOR_BAKETIRANJA; i++)
    {
        if((bak.slogovi[i].evidencioni_broj == evid_br) && (bak.slogovi[i].status != LOGICKI_OBRISAN) )
        {
            //ind = 0 je flag da je pronadjen u maticnom baketu
            ind = U_PRIM_ZONI;
            return ind;
        }
    }

    //trazenje u zoni prekoracenja
    for(i = POC_ZONE_PREKOR; i <= KRAJ_ZONE_PREKOR; i++)
    {
        ucitaj_sa_adrese(d,i,&bak);
        for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
        {
            if(bak.slogovi[j].evidencioni_broj == evid_br && (bak.slogovi[j].status != LOGICKI_OBRISAN))
            {
                //ind = 2 je flag da je pronadjen u zoni prekor
                ind = U_ZONI_PREK;
                return ind;
            }
        }
    }

    ind = NEUSPESNO_TRAZENJE;
    return ind;
}

void nadji_slog(Datoteka *d)
{
    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka\n");
        return;
    }
    unsigned long evid_br;
    int i,j,zona;
    Baket bak;
    char *naziv_zone;

    do
    {
        printf("Unesite evidencioni broj: \n");
        scanf("%lu",&evid_br);
    }while(evid_br > 999999 || evid_br < 100000);

    //promena kljuca u adresu
    int adresa = transformacija_sred_cif_kvad(evid_br);
    ucitaj_sa_adrese(d,adresa,&bak);

    //dobijem informaciju u kojoj je zoni baket
    zona = trazenje_u_ras_dat(d,evid_br);

    if(zona == U_PRIM_ZONI)
    {
        naziv_zone = "primarna zona";
        for(i = 0; i <= FAKTOR_BAKETIRANJA; i++)
        {
            if(bak.slogovi[i].status == ZAUZET && bak.slogovi[i].evidencioni_broj == evid_br)
            {
                printf("\nNadjen je slog u baketu sa adresom '%d' u zoni '%s' \n",adresa,naziv_zone);
                printf("Evidencioni broj: %lu\n",bak.slogovi[i].evidencioni_broj);
                printf("Vrsta pizze: %s\n", bak.slogovi[i].vrsta_pizze);
                printf("Adresa: %s\n", bak.slogovi[i].adresa);
                printf("Kolicina: %d\n",bak.slogovi[i].kolicina);
                return;
            }
        }
    }
    else if(zona == U_ZONI_PREK)
    {
        naziv_zone = "zona prekoracenja";
        //ako nije nasao u prim zoni, trazi u zoni prekoracenja
        for(i = POC_ZONE_PREKOR; i <= KRAJ_ZONE_PREKOR; i++)
        {
            ucitaj_sa_adrese(d,i,&bak);
            for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
            {
                if(bak.slogovi[j].status == ZAUZET && bak.slogovi[j].evidencioni_broj == evid_br)
                {
                    printf("\nNadjen je slog u baketu sa adresom '%d' u zoni '%s' \n",i - BR_BAKETA,naziv_zone);
                    printf("Evidencioni broj: %lu\n",bak.slogovi[j].evidencioni_broj);
                    printf("Vrsta pizze: %s\n", bak.slogovi[j].vrsta_pizze);
                    printf("Adresa: %s\n", bak.slogovi[j].adresa);
                    printf("Kolicina: %d\n",bak.slogovi[j].kolicina);
                    return;
                }
            }
        }
    }
    else if(zona == NEUSPESNO_TRAZENJE)
    {
        printf("Nije pronadjen slog sa datim evidencionim brojem.\n");
        return;
    }



}

void prikazi_sve_slogove(Datoteka *d)
{
    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka\n");
        return;
    }

    Baket bak;
    int i,j;


    for(i = 1; i <= BR_BAKETA; i++)
    {
        ucitaj_sa_adrese(d,i,&bak);
        for(j = 0; j< FAKTOR_BAKETIRANJA; j++)
        {
            if(bak.slogovi[j].status == ZAUZET)
            {
                printf("\n**********************************************\n");
                printf("Nadjen je slog u baketu sa adresom '%d' u PRIMARNOJ ZONI i on %d. slog \n",i,j+1); //i je adresa baketa
                printf("Evidencioni broj: %lu\n",bak.slogovi[j].evidencioni_broj);
                printf("Vrsta pizze: %s\n", bak.slogovi[j].vrsta_pizze);
                printf("Adresa: %s\n", bak.slogovi[j].adresa);
                printf("Kolicina: %d\n",bak.slogovi[j].kolicina);
                printf("**********************************************\n");
            }
        }
    }

    for(i = POC_ZONE_PREKOR; i <= KRAJ_ZONE_PREKOR; i++)
    {
        ucitaj_sa_adrese(d,i,&bak);
        for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
        {
            if(bak.slogovi[j].status == ZAUZET)
            {
                printf("\n**********************************************\n");
                printf("Nadjen je slog u baketu sa adresom '%d' u ZONI PREKORACENJA i on %d. slog \n",i - BR_BAKETA,j+1); //i je adresa baketa
                printf("Evidencioni broj: %lu\n",bak.slogovi[j].evidencioni_broj);
                printf("Vrsta pizze: %s\n", bak.slogovi[j].vrsta_pizze);
                printf("Adresa: %s\n", bak.slogovi[j].adresa);
                printf("Kolicina: %d\n",bak.slogovi[j].kolicina);
                printf("**********************************************\n");
            }
        }
    }
}

void logicko_brisanje(Datoteka *d)
{
    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka\n");
        return;
    }

    int i,j,adresa;
    unsigned long evid_br;
    Baket bak;

    do
    {
        printf("Unesite evidencioni broj sloga kojeg hocete da obrisete: \n");
        scanf("%lu",&evid_br);
    }while(evid_br > 999999 || evid_br < 100000);

    adresa = transformacija_sred_cif_kvad(evid_br);
    ucitaj_sa_adrese(d,adresa,&bak);

    for(i = 0; i < FAKTOR_BAKETIRANJA; i++)
    {
        if(bak.slogovi[i].evidencioni_broj == evid_br)
        {
            bak.slogovi[i].status = LOGICKI_OBRISAN;
            upisi_u_adresu(d,adresa,&bak);
            printf("Uspesno izvrseno logicko brisanje!\n");
            return;
        }
    }

    //Ako ga nije nasao u maticnom baketu, trazim ga u zoni prekoracenja, prolazim kroz celu zonu
    for(i = POC_ZONE_PREKOR; i <= KRAJ_ZONE_PREKOR; i++)
    {
        ucitaj_sa_adrese(d,i,&bak);
        for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
        {
            if(bak.slogovi[j].evidencioni_broj == evid_br)
            {
                bak.slogovi[j].status = LOGICKI_OBRISAN;
                upisi_u_adresu(d,i,&bak);
                printf("Uspesno izvrseno logicko brisanje!\n");
                return;
            }
        }
    }

    printf("Logicko brisanje nije uspelo.\n");

}

void izmena_adrese(Datoteka *d)
{
    if(d->f == NULL)
    {
        printf("Ne postoji aktivna datoteka\n");
        return;
    }

    unsigned long evid_br;
    Baket bak;
    int i,j,adresa;
    char nova_adresa[MAX_ADRESA];

    do
    {
        printf("Unesite evidencioni broj sloga kojem hocete da menjate adresu: \n");
        scanf("%lu",&evid_br);
    }while(evid_br > 999999 || evid_br < 100000);

    adresa = transformacija_sred_cif_kvad(evid_br);
    ucitaj_sa_adrese(d,adresa,&bak);

    printf("Unesite novu adresu: \n");
    fflush(stdin);
    gets(nova_adresa);

    for(i = 0; i < FAKTOR_BAKETIRANJA; i++)
    {
        if(bak.slogovi[i].evidencioni_broj == evid_br)
        {
            strcpy(bak.slogovi[i].adresa,nova_adresa);
            upisi_u_adresu(d,adresa,&bak);
            printf("Uspesno izvrseno azuriranje adrese!\n");
            return;
        }
    }

    //Ako ga nije nasao u maticnom baketu, trazim ga u zoni prekoracenja, prolazim kroz celu zonu
    for(i = POC_ZONE_PREKOR; i <= KRAJ_ZONE_PREKOR; i++)
    {
        ucitaj_sa_adrese(d,i,&bak);
        for(j = 0; j < FAKTOR_BAKETIRANJA_PREKOR; j++)
        {
            if(bak.slogovi[j].evidencioni_broj == evid_br)
            {
                strcpy(bak.slogovi[j].adresa,nova_adresa);
                upisi_u_adresu(d,i,&bak);
                printf("Uspesno izvrseno azuriranje adrese!\n");
                return;
            }
        }
    }
    printf("Azuriranje adrese nije uspelo.\n");
}




