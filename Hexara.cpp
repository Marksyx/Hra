#include <iostream>
using namespace std;

    struct Postava          //pouziti struktur, chatGBT, https://www.w3schools.com/cpp/cpp_structs.asp, dokumentace c++
    {
        string jmeno;
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
        string popis;
        int penize;
        string typ;
        bool navstiveno;
        string pohyb[5];
    };

    Postava postavy[] =
    {
        {"Paladin", "popis Paladina", 100, 100, 100, 100, 10, 1, 0, 60, 20},
        {"Monstrum", "popis Monstra", 100, 100, 100, 100, 10, 1, 0, 60, 20}
    };

    Misto mista[] =
    {
        {"Start", "popis Startu", 0, "zacatek", false, {"start", "Les"}},
        {"Les", "popis Bojiste", 5, "bojiste", false, {"start", "Les"}},
        {"Grad", "vesnice", 0, "vesnice", false, {"start", "Les"}}
    };


void vypis()
{
    for (int i = 0; i < sizeof(postavy)/sizeof(postavy[0]); i++)        //pouziti sizeof, dokumentace c++
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

    for (int i = 0; i < sizeof(mista)/sizeof(mista[0]); i++)
    {
        cout << "\nNazev: " << mista[i].nazev;
        cout << ", Typ: " << mista[i].typ;
        cout << ", Penize: " << mista[i].penize;
        cout << ", Navstiveno: " << mista[i].navstiveno << endl;
    }

}
int main()
{
    vypis();
    return 0;
}
