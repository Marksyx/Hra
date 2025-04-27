#include <iostream>
using namespace std;


    int pozice = 0;
    bool konec_hry = false;

    struct Postava          //pouziti struktur, chatGBT, https://www.w3schools.com/cpp/cpp_structs.asp, dokumentace c++
    {
        string jmeno;
        int id;
        string popis;
        int zivoty;
        int max_zivoty;
        int mana;
        int max_mana;
        int penize;
        int level;
        int zkusenosti;
        int utok;
        int schopnosti;
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
    };

    Postava postavy[] =
    {
        {"Paladin", 0, "popis Paladina", 100, 100, 100, 100, 10, 1, 0, 60, 20},
        {"Monstrum", 1, "popis Monstra", 100, 100, 100, 100, 10, 1, 0, 60, 20}
    };

    Misto mista[] =
    {
        {"Start", 0, "popis Startu", 0, "zacatek", false, {2}},
        {"Les", 1, "popis Bojiste", 5, "bojiste", false, {2, 3, 4}},
        {"Grad", 2, "vesnice", 0, "vesnice", false, {1, 3}},
        {"Reka", 3, "reka", 0, "bojiste", false, {2, 1, 4}},
        {"Konec", 4, "konec", 0, "vesnice", false, {}}
    };

    int pocet_mist = sizeof(mista)/sizeof(mista[0]);
    int posledni_misto = pocet_mist - 1;

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
    cout << mista[pozice].nazev << endl;
    cout << "Kam muzes jít?" << endl;
    for (int j = 0; j < sizeof(mista[pozice].pohyb)/sizeof(mista[pozice].pohyb[0]); j++)
    {
                if (mista[pozice].pohyb[j] > 0)
                    {
                     int id_budouci = mista[pozice].pohyb[j];
                     cout << mista[id_budouci].nazev << ": " << id_budouci << endl;
                    }
    }

    cout << "Kam jdes?\n";
    cin >> pozice;
}
int main()
{
    do
    {
        vypis();

        if (pozice == posledni_misto)
            {
                konec_hry = true;
            }
       // konec_hry = true;
    } while (!konec_hry);

    return 0;
}
