#include <iostream>
#include <conio.h>
#include <windows.h>        //česká diakritika
using namespace std;


    int pozice          = 0;
    bool konec_hry      = false;
    string klavesy[5]   = {"q", "w", "e", "r", "t"};

    struct Hrdina          //pouziti struktur, chatGBT, https://www.w3schools.com/cpp/cpp_structs.asp, dokumentace c++
    {
        string jmeno;
        int id;
        string popis;
        int zivoty;
        int mana;
        int penize;
        int level;
        int zkusenosti;
        int utok;
        int schopnosti[10];
    };

    struct Mostrum
    {
        string jmeno;
        int id;
        string popis;
        int zivoty;
        int utok;
        int kouzla[10];
    };

    struct Misto
    {
        string nazev;
        int id;
        string popis;
        int penize;
        string typ;
        bool navstiveno;
        int pohyb[5];
        int monstra[3];
    };

    struct Level
    {
        int max_zivoty;
        int max_mana;
        int schopnost;
        int odmena;
    };

    struct Schopnost
    {
        string nazev;
        float uder;         //Znasobi silu uderu
        bool multi_uder;    //True=Uder na vsechny monstra, mana-10
        int oziveni;        //Prida pocet zivotu
    };

    struct Kouzlo
    {
        string nazev;
        float uder;         //Znasobi silu uderu
    };

    // DATA HRY

    Level levels[] =
    {
        {100, 100, NULL, 0},
        {110, 100, 0, 0},
        {115, 110, 1, 10}
    };

    Schopnost schopnosti[] =
    {
        {"Silnější úder", 1.5, false, 0},
        {"Útok světlem", 1, true, 0},
        {"Oživení", 1, false, 10}
    };
    Kouzlo kouzla[] =
    {
        {"Blesk", 1.5},
        {"Hrom", 1.3}
    };

    Hrdina hrdinove[] =
    {
        {"Paladin", 0, "popis Paladina", 100, 100, 10, 0, 0, 60},
        {"Lovec", 1, "popis Lovce", 80, 100, 10, 0, 0, 80},
        {"Mag", 2, "popis Maga", 90, 120, 10, 0, 0, 50},
        {"Warlock", 3, "popis Warlocka", 120, 80, 10, 0, 0, 50}
    };

    Mostrum monstra[] =
    {
        {"Monstrum", 1, "popis Monstra", 100, 10, {}},
        {"Monstrum2", 2, "popis Monstra2", 80, 20, {0, 1}}
    };

    Misto mista[] =
    {
        {"Start", 0, "popis Startu", 0, "zacatek", false, {2}},
        {"Les", 1, "popis Bojiste", 5, "bojiste", false, {2, 3, 4},{1}},
        {"Grad", 2, "vesnice", 0, "vesnice", false, {1, 3}},
        {"Reka", 3, "reka", 0, "bojiste", false, {2, 1, 4},{2}},
        {"Konec", 4, "konec", 0, "vesnice", false, {}}
    };

    int pocet_mist      = sizeof(mista)/sizeof(mista[0]);
    int posledni_misto  = pocet_mist - 1;

void vypis()
{
/**    for (int i = 0; i < sizeof(postavy)/sizeof(postavy[0]); i++)        //pouziti sizeof, dokumentace c++
    {
        cout << "\nJmeno: " << postavy[i].jmeno ;
        cout << ", Zivoty: " << postavy[i].zivoty;
        cout << ", Max zivoty: " << postavy[i].max_zivoty;
        cout << ", Mana: " << postavy[i].mana;
        cout << ", Max mana: " << postavy[i].max_mana;
        cout << ", Penize: " << postavy[i].penize;
        cout << ", Level: " << postavy[i].level;
        cout << ", Zkusenosti: " << postavy[i].zkusenosti;
        cout << ", Utok: " << postavy[i].utok;
        cout << ", Schopnosti: " << postavy[i].schopnosti << endl;
    }

    for (int i = 0; i < pocet_mist; i++)
    {
        cout << "\nNazev: " << mista[i].nazev;
        cout << ", Typ: " << mista[i].typ;
        cout << ", Penize: " << mista[i].penize;
        cout << ", Navstiveno: " << mista[i].navstiveno << ", Pohyb: ";
        for (int j = 0; j < sizeof(mista[i].pohyb)/sizeof(mista[i].pohyb[0]); j++)
            {
                if (mista[i].pohyb[j] > 0
                    {
                     cout << mista[i].pohyb[j] << ", ";
                    }
            }
    }
**/
    int k = 0;
    string klavesa;
    int kod_klavesy;
    int xMisto[5];
    string xKlavesa[5];
    bool spravna_klavesa = false;

    cout << endl << mista[pozice].nazev << endl;
    cout << "Kam můžeš jít?" << endl;

    // vypis moznych mist pro pohyb ve hre
    for (int j = 0; j < sizeof(mista[pozice].pohyb)/sizeof(mista[pozice].pohyb[0]); j++)
    {
        if (mista[pozice].pohyb[j] > 0)
        {
            int id_budouci = mista[pozice].pohyb[j];
            cout << mista[id_budouci].nazev << "(" << klavesy[k]<< ") "<< endl;
            xMisto[j] = id_budouci;
            xKlavesa[j] = klavesy[k];
            k++;
            // cout << mista[id_budouci].nazev << "(" << xKlavesa[j] << "): " << xMisto[j] << endl;
        }
    }

    cout << "Kam jdeš?\n";

    // cteni klavesy od hrace
    kod_klavesy = getch();      //pouziti getch(), https://www.geeksforgeeks.org/getch-function-in-c-with-examples/, dokumentace c++
    klavesa = (char)kod_klavesy;

    // kontrola klavesy vuci mistum ve hre
    for (int j = 0; j < sizeof(xKlavesa)/sizeof(xKlavesa[0]); j++)
    {
        if (xKlavesa[j] == klavesa)
        {
            pozice = xMisto[j];
            spravna_klavesa = true;
        }
    }

    // osetreni chybne klavesy hrace
    if (!spravna_klavesa)
    {
        cout << "\nChyba!\nPoužil jsi špatnou klávesu" << endl;
        cout << "Press any key to continue!";
        kod_klavesy = getch();
    }

}
int main()
{
    SetConsoleOutputCP(CP_UTF8);

    int muj_hrdina;
    char opravdu;
    int pocet_hrdinu = sizeof(hrdinove)/sizeof(hrdinove[0]);

    do {
        system("cls");

        cout << "Vyber si hrdinu: " << endl;
        for (int i = 0; i < pocet_hrdinu; i++)
        {
            cout << i << " " << hrdinove[i].jmeno << " - " << hrdinove[i].popis << endl;
        }

        cout << "\nZadej číslo hrdiny: ";
        cin >> muj_hrdina;

        cout << "Jméno: " << hrdinove[muj_hrdina].jmeno << endl;
        cout << "Popis: " << hrdinove[muj_hrdina].popis << endl;
        cout << "Životy: " << hrdinove[muj_hrdina].zivoty << endl;
        cout << "Mana: " << hrdinove[muj_hrdina].mana << endl;
        cout << "Peníze: " << hrdinove[muj_hrdina].penize << endl;
        cout << "Level: " << hrdinove[muj_hrdina].level << endl;
        cout << "Zkušenosti: " << hrdinove[muj_hrdina].zkusenosti << endl;
        cout << "Útok: " << hrdinove[muj_hrdina].utok << endl << endl;

        cout << "Chceš tuto postavu (a/n)?";
        cin >> opravdu;


    } while (opravdu != 'a' && opravdu != 'A');


    do
    {
        system("cls");      //pouziti clear screen, https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/, dokumentace c++
        vypis();

        if (pozice == posledni_misto)
        {
            konec_hry = true;
        }
       // konec_hry = true;
    } while (!konec_hry);

    cout << "\nGAME OVER!!" << endl;
    return 0;
}
