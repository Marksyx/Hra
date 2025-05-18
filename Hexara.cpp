#include <iostream>
#include <conio.h>
#include <windows.h>        //česká diakritika
using namespace std;


    int pozice          = 0;
    int muj_hrdina;
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
        // int id;
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
        {"Mag", 2, "popis Maga", 1, 120, 10, 0, 0, 50},
        {"Warlock", 3, "popis Warlocka", 120, 80, 10, 0, 0, 50}
    };

    Mostrum monstra[] =
    {
        {"NoMonster", "", 0, 0, {}},
        {"Monstrum", "popis Monstra", 100, 10, {}},
        {"Monstrum2", "popis Monstra2", 80, 20, {0, 1}}
    };

    Misto mista[] =
    {
        {"Start", 0, "popis Startu", 0, "zacatek", false, {2}},
        {"Les", 1, "popis Bojiste", 5, "bojiste", false, {2, 3, 4},{1}},
        {"Grad", 2, "vesnice", 0, "vesnice", false, {1, 3}},
        {"Řeka", 3, "reka", 0, "bojiste", false, {2, 1, 4},{2, 1}},
        {"Konec", 4, "konec", 0, "vesnice", false, {}}
    };

    int pocet_mist      = sizeof(mista)/sizeof(mista[0]);
    int posledni_misto  = pocet_mist - 1;

void konec(bool vyhra)
{
    if(vyhra)
    {
        cout << "Gratuluju, hru jsi úspěšně dohrál" << endl;
    }
    else
    {
        cout << "Bohužel jsi prohrál. Zkus to příště." << endl;
    }
    cout << "Jeslti se ti hra líbila, nebo k ní máš nějaké připomínky nebo nápady, napiš mi na marksyxx@gmail.com.";
    exit(0);
}

void boj()
{
    // cout << "Pocet monster v miste: " << sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]) << endl;

    int tah = 1;
    int i;
    cout << hrdinove[muj_hrdina].jmeno << endl;
    for (i = 0; i < sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]); i++)
    {
        if(mista[pozice].monstra[i] > 0)
        {
            cout << monstra[mista[pozice].monstra[i]].jmeno;
            if(monstra[mista[pozice].monstra[i]].zivoty <= 0)
            {
                cout <<    " DEAD";
            }
            cout << endl;
         }
    }
    system("pause");

    for (i = 0; i < sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]); i++)
    {
        bool vyhra_souboj = false;
        tah = 1;
        if(mista[pozice].monstra[i] > 0 && monstra[mista[pozice].monstra[i]].zivoty > 0)
        {
            do
            {
                cout << "Hrdina (" << hrdinove[muj_hrdina].jmeno << "): " << hrdinove[muj_hrdina].zivoty << endl;
                cout << "Monstrum (" << monstra[mista[pozice].monstra[i]].jmeno << "): " << monstra[mista[pozice].monstra[i]].zivoty << endl;

                cout << "Tah " << tah++ << ".: " << endl;

                monstra[mista[pozice].monstra[i]].zivoty = monstra[mista[pozice].monstra[i]].zivoty - hrdinove[muj_hrdina].utok;
                if(monstra[mista[pozice].monstra[i]].zivoty < 0)
                {
                    cout << "Hrdina vyhrál souboj" << endl;
                    vyhra_souboj = true;
                }
                else
                {
                    hrdinove[muj_hrdina].zivoty = hrdinove[muj_hrdina].zivoty - monstra[mista[pozice].monstra[i]].utok;
                    if(hrdinove[muj_hrdina].zivoty < 0)
                    {
                        cout << "Hrdina prohrál souboj" << endl;
                        konec(false);
                    }
                }
                system("pause");
            }while(!vyhra_souboj);
        }
    }
}

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
    int k = 0;      //pomocna ke klavesam
    int m = 0;      //pomocna k monstrum
    string klavesa;
    int kod_klavesy;
    int xMisto[5];
    string xKlavesa[5];
    bool spravna_klavesa = false;

    cout << endl << mista[pozice].nazev << endl;

    int pocet_monster = sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]);

    for (int j = 0; j < pocet_monster; j++)
    {
        if (mista[pozice].monstra[j] > 0)
        {
            cout << "Na tomto místě je toto monstrum: " << monstra[mista[pozice].monstra[j]].jmeno << endl;
            m++;
        }
    }
    if(m)
    {
        cout << "\nBoj začíná" << endl;
        boj();
    }

    cout << endl << "Kam můžeš jít?" << endl;

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

    char opravdu;
    int pocet_hrdinu = sizeof(hrdinove)/sizeof(hrdinove[0]);

    do
    {
        do
        {
            system("cls");

            cout << "Vyber si hrdinu: " << endl;
            for (int i = 0; i < pocet_hrdinu; i++)
            {
                cout << i << " " << hrdinove[i].jmeno << " - " << hrdinove[i].popis << endl;
            }

            cout << "\nZadej číslo hrdiny: ";
            cin >> muj_hrdina;
        }while (muj_hrdina < 0 || muj_hrdina >= pocet_hrdinu);

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


    }while (opravdu != 'a' && opravdu != 'A');


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
