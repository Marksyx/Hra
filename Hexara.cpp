// Autor: Marek Sýkora

// TODO:
// sjednotit ovládání voleb
// vylepšit výpisy
// dopsat popisy míst a doplnit příběh
// navštívená místa
// optimalizace kontroly pouzitych kouzel (do while)
// vlastni funkce pause

// Známé chyby
// ...

#include <iostream>
#include <conio.h>
#include <windows.h>        //česká diakritika
#include <cstdlib>
#include <ctime>            //řešení náhodnosti
#include <limits>           //řešení náhodnosti
using namespace std;

    int pozice          = 0;
    int muj_hrdina;
    bool konec_hry      = false;
    bool debug          = true;
    string klavesy[5]   = {"q", "w", "e", "r", "t"};
    bool omraceni       = false;
    int minule_kouzlo   = 0;

    // Parametry hry

    const int CENA_ZA_UPGRADE = 20;
    const int NAVYSENI_MAX = 20;
    const int MANA_ZA_SCHOPNOST = 30;

    struct Hrdina          //pouziti struktur, chatGBT, https://www.w3schools.com/cpp/cpp_structs.asp, dokumentace c++
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
        int schopnost;
    };

    struct Mostrum
    {
        string jmeno;
        string popis;
        bool boss;
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
        int plus_zivoty;
        int plus_mana;
        float odmena;
    };

    struct Schopnost
    {
        string nazev;
        float uder;         //Znasobi silu uderu
        bool multi_uder;    //True=Uder na vsechny monstra, mana-10
        int oziveni;        //Prida pocet zivotu
        bool omraceni;      //Omraci monstrum / vyneha jeden tah monstra
    };

    struct Kouzlo
    {
        string nazev;
        float uder;         //Znasobi silu uderu
        int leceni;         //+zivoty monstra
        bool pouzito;       //Bylo kouzlo pouzito?
    };

    // DATA HRY

    Level levels[] =
    {
        // plus_zivoty; plus_mana; odmena;
        {20, 20, 1},
        {20, 20, 1.1},
        {20, 20, 1.2},
        {20, 20, 1.3},
        {20, 20, 1.4},
        {20, 20, 1.5},
        {20, 20, 1.6},
        {20, 20, 1.7}
    };

    Schopnost schopnosti[] =
    {
        // nazev; uder; multi_uder; oziveni; omraceni;
        {"NoSchopnost", 1, false, 0, false},
        {"Silnější úder", 1.5, false, 0, false},
        {"Útok světlem", 1, true, 0, false},
        {"Oživení", 1, false, 50, false},
        {"Omráčení", 1, false, 0, true}
    };
    Kouzlo kouzla[] =
    {
        // nazev; uder; leceni; pouzito;
        {"NoKouzlo", 1, 0, false},
        {"Fireball", 1.3, 0, false},
        {"Led", 1.3, 0, false},
        {"Stín", 1.3, 0, false},
        {"Vítr", 1.3, 0, false},
        {"Jed", 1.3, 0, false},
        {"Léčivé", 0, 50, false}
    };

    Hrdina hrdinove[] =
    {
        // jmeno; id; popis; zivoty; max_zivoty; mana; max_mana; penize; level; zkusenosti; utok; schopnost;
        {"Paladin", 0, "popis Paladina", 100, 100, 100, 100, 10, 0, 0, 60, 1},
        {"Lovec", 1, "popis Lovce", 80, 80, 100, 100, 10, 0, 0, 50, 4},
        {"Mag", 2, "popis Maga", 100, 100, 120, 120, 10, 0, 0, 50, 2},
        {"Warlock", 3, "popis Warlocka", 80, 80, 80, 80, 10, 0, 0, 50, 3}
    };

    Mostrum monstra[] =
    {
        // jmeno; popis; boss; zivoty; utok; kouzla
        {"NoMonster", "", 0, 0, {}},
        {"Stínokrad", "Tvor z temnoty, který se zjevuje tam, kde se šíří strach. Jeho tělo tvoří živoucí stín.", false, 100, 10, {}},                                                      //1(cave)                   1
        {"Kostěný rváč", "Nemrtvý válečník se zrezivělým mečem a popraskaným štítem. I po smrti touží po souboji.", false, 80, 20, {}},                                                    //7(hřbitov)                2
        {"Jeskynní škrabač", "Malý, ale nebezpečný tvor se silnými drápy. Vydává děsivé zvuky, než zaútočí ze tmy.", false, 80, 20, {}},                                                   //1(cave)                   3
        {"Bažinný slizák", "Mazlavá hrouda hniloby, co se plazí mokřady a pohlcuje vše živé.", false, 80, 20, {}},                                                                         //3(Les plný mokřadů)       4
        {"Plamenný netvor", "Bytost ze sfér ohně, hořící plameny ji obklopují i během boje.", false, 80, 20, {}},                                                                          //4(Zřicenina hradu)        5
        {"Lesní běs", "Zkroucený duch přírody, co chrání zapomenuté lesy. Vzývá stromy a trny.", false, 80, 20, {}},                                                                       //3(Les plný mokřadů)       6
        {"Kamenný kolos", "Obrovitý tvor z kamene, který se pohybuje pomalu, ale devastuje jediným úderem.", false, 80, 20, {}},                                                           //1(cave)                   7
        {"Krvesajný havran", "Zmutovaný pták velikosti psa, který útočí ve skupinách a cílí na oči.", false, 80, 20, {}},                                                                  //5(opuštěná ZOO)           8
        {"Krveplaz", "Rozervaný tvor s trhlinami v těle, z nichž visí kůže a maso. Jeho útoky jsou brutální a neúprosné.", false, 60, 10, {}},                                             //6(opuštělá chalupa)       9
        {"Dýmová příšera", "Nestvůra z černého kouře, která nemá tvar ani duši. Dusí vše, co vdechuje její plyn.", false, 80, 20, {}},                                                     //4(Zřicenina hradu)        10
        {"Chrlič zatracení", "Oživená kamenná socha z chrámu padlých bohů, věčně bdí a zabíjí vše cizí.", false, 80, 20, {}},                                                              //7(hřbitov)                11
        {"Šeptač nocí", "Neviditelný přízrak, který láme mysl šeptáním. Je slyšet, ale málokdo ho viděl.", false, 80, 20, {}},                                                             //2(děsivé místo)           12
        {"Ledová zmije", "Had s krystaly ledu místo šupin. Její dech mrzne krev v žilách.", false, 80, 20, {}},                                                                            //5(opuštěná ZOO)           13
        {"Bezlící Mučitel", "Bezlící mučitel bez hlavy, který bez zastavení loví a mučí duše svých obětí věčnou bolestí.", false, 80, 20, {}},                                             //6(opuštělá chalupa)       14
        {"Pavučinová matka", "Obrovský pavouk s lidskou tváří. Ovládá menší pavouky a své oběti balí zaživa.", false, 80, 20, {}},                                                         //5(opuštěná ZOO)           15
        {"Půlnoční běs", "Noční jezdec beze jména, zjevuje se za úplňku. Jeho oči žhnou a kůň nevrhá stín.", false, 80, 20, {}},                                                           //2(děsivé místo)           16
        {"Mikeš", "Na pohled roztomilá černá kočička, ve skutečnosti zákeřný démon. Přede, uspává nepřátele a útočí ze stínů. Přináší smůlu těm, kdo se ho dotknou.", false, 80, 20, {}},  //8(brloch)                 17
        {"MB", "MB", true, 200, 30, {}},
        {"MB2", "MB2", true, 250, 15, {}},
        {"HEXARA", "HEXARA", true, 300, 50, {1, 2, 3, 4, 5, 6}}
    };

    Misto mista[] =
    {
        //nazev; id; popis; penize; typ; navstiveno; pohyb[5]; monstra[3];
        {"Start", 0, "popis Startu", 0, "zacatek", false, {1}},
        {"Brloch", 1, "popis Brlochu", 0, "bojiste", false, {2, 3, 4},{17}},
        {"Opuštěná chalupa", 2, "popis", 0, "bojiste", false, {1, 4},{9, 13}},
        {"Les plný mokřadů", 3, "popis", 0, "bojiste", false, {1, 4, 5},{4, 6}},
        {"Vesnice", 4, "popis", 0, "vesnice", false, {1, 2, 3, 5, 6}},
        {"Děsivé místo", 5, "popis", 0, "bojiste", false, {3, 4, 6, 7},{12, 16}},
        {"Hřbitov", 6, "popis", 0, "bojiste", false, {4, 5, 7, 8},{2, 11}},
        {"Vesnice2", 7, "popis", 0, "vesnice", false, {5, 6, 8, 9, 10}},
        {"Zřicenina hradu", 8, "popis", 0, "bojiste", false, {6, 7, 10},{5, 10}},
        {"Opuštěná ZOO", 9, "popis", 0, "bojiste", false, {7, 10, 11},{8, 13, 15}},
        {"Jeskyně", 10, "popis", 0, "bojiste", false, {7, 8, 9, 11},{1, 3, 7}},
        {"MB", 11, "popis", 0, "bojiste", false, {9, 10, 12},{18}},
        {"MB2", 12, "popis", 0, "bojiste", false, {11, 13},{19}},
        {"Vesnice3", 13, "popis", 0, "vesnice", false, {12, 14}},
        {"Hexara", 14, "popis", 0, "bojiste", false, {15},{20}},
        {"Konec", 15, "popis", 0, "konec", false, {},{}}
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
    cout << "Jestli se ti hra líbila, nebo k ní máš nějaké připomínky nebo nápady, napiš mi na marksyxx@gmail.com.";
    exit(0);
}

void vypis_hrdiny()
{
    cout << "\nJméno: " << hrdinove[muj_hrdina].jmeno << endl;
    cout << "Popis: " << hrdinove[muj_hrdina].popis << endl;
    cout << "Životy: " << hrdinove[muj_hrdina].zivoty << endl;
    cout << "Mana: " << hrdinove[muj_hrdina].mana << endl;
    cout << "Peníze: " << hrdinove[muj_hrdina].penize << endl;
    cout << "Level: " << hrdinove[muj_hrdina].level << endl;
    cout << "- Max životy: " << hrdinove[muj_hrdina].max_zivoty << endl;
    cout << "- Max mana: " << hrdinove[muj_hrdina].max_mana << endl;
    cout << "Zkušenosti: " << hrdinove[muj_hrdina].zkusenosti << endl;
    cout << "Útok: " << hrdinove[muj_hrdina].utok << endl;
    cout << "Schopnost: " << schopnosti[hrdinove[muj_hrdina].schopnost].nazev << endl << endl;
}

void vesnice() // řeší možnosti vesnice, doplnění vlastností hrdiny
{
    int vyber = 0;
    cout << "Ve vesnici si můžeš doplnit životy, vylepšit max životy, manu a útok." << endl;
    do
    {
        cout << "Tvé staty:" << endl;
        cout << "Jméno: " << hrdinove[muj_hrdina].jmeno << endl;
        cout << "Životy: " << hrdinove[muj_hrdina].zivoty << "/" << hrdinove[muj_hrdina].max_zivoty << endl;
        cout << "Mana: " << hrdinove[muj_hrdina].mana << "/" << hrdinove[muj_hrdina].max_mana << endl;
        cout << "Útok: " << hrdinove[muj_hrdina].utok << endl;
        cout << "Peníze: " << hrdinove[muj_hrdina].penize << endl << endl;

        cout << "1:Doplní životy" << endl;
        cout << "2:Upgrade max-životy" << endl;
        cout << "3:Doplní manu" << endl;
        cout << "4:Upgrade max-mana" << endl;
        cout << "5:Upgrade útok" << endl;
        cout << "6:Opustit vesnici" << endl << endl;
        cout << "Vyber volbu:"<< endl;
        cin >> vyber;

        switch(vyber)
        {
        case 1: // Doplní životy
            if (hrdinove[muj_hrdina].zivoty == hrdinove[muj_hrdina].max_zivoty)
            {
                cout << "Už si vyléčený." << endl;
                break;
            }

            if (hrdinove[muj_hrdina].penize < CENA_ZA_UPGRADE)
            {
                cout << "Nemáš dostatek peněz." << endl;
            }
            else
            {
                hrdinove[muj_hrdina].zivoty = hrdinove[muj_hrdina].max_zivoty;
                hrdinove[muj_hrdina].penize -= CENA_ZA_UPGRADE;
            }
            cout << endl;
            break;

        case 2: // Upgrade max-životy
            if (hrdinove[muj_hrdina].penize < CENA_ZA_UPGRADE)
            {
                cout << "Nemáš dostatek peněz.";
            }
            else
            {
                hrdinove[muj_hrdina].max_zivoty += NAVYSENI_MAX;
                hrdinove[muj_hrdina].penize -= CENA_ZA_UPGRADE;
            }
            cout << endl;
            break;

        case 3: // Doplní manu
            if (hrdinove[muj_hrdina].mana == hrdinove[muj_hrdina].max_mana)
            {
                cout << "Už máš many dost." << endl;
                break;
            }

            if (hrdinove[muj_hrdina].penize < CENA_ZA_UPGRADE)
            {
                cout << "Nemáš dostatek peněz.";
            }
            else
            {
                hrdinove[muj_hrdina].mana = hrdinove[muj_hrdina].max_mana;
                hrdinove[muj_hrdina].penize -= CENA_ZA_UPGRADE;
            }
            cout << endl;
            break;

        case 4: // Upgrade max-mana
            if (hrdinove[muj_hrdina].penize < CENA_ZA_UPGRADE)
            {
                cout << "Nemáš dostatek peněz.";
            }
            else
            {
                hrdinove[muj_hrdina].max_mana += NAVYSENI_MAX;
                hrdinove[muj_hrdina].penize -= CENA_ZA_UPGRADE;
            }
            cout << endl;
            break;

        case 5: // Upgrade útok
            if (hrdinove[muj_hrdina].penize < CENA_ZA_UPGRADE)
            {
                cout << "Nemáš dostatek peněz.";
            }
            else
            {
                hrdinove[muj_hrdina].utok = hrdinove[muj_hrdina].utok + NAVYSENI_MAX;
                hrdinove[muj_hrdina].penize -= CENA_ZA_UPGRADE;
            }
            cout << endl;
            break;

        case 6: // Opustit vesnici
            cout << "Opouštíš vesnici...";
            cout << endl;
            break;

        default:
            cout << "Neplatná volba, zadej znovu.";
            cout << endl;
            break;
        }
    }
    while(vyber != 6);
}

void tah_monstra(int index, int cislo_tahu)
{
    cout << "Tah " << cislo_tahu << ".: " << endl;
    if (omraceni)   //hrdina pouzil schopnost omráčení a mosntrum vynechava 1 tah
    {
        cout << "Monstrum je omráčené a nemůže útočit" << endl;
        omraceni = false;
    }
    else    //monstrum neni omraceno a je na tahu
    {
        if (monstra[mista[pozice].monstra[index]].jmeno == "HEXARA")    //Zacina boj s HEXAROU
        {
            bool vsechna_kouzla_pouzita = true;
            for(int i = 1; i < sizeof(kouzla)/sizeof(kouzla[0]); i++)   // Bylo kouzlo pouzito?
            {
                if(kouzla[i].pouzito == false)  //Urcovani (ne)pouziti
                {
                    vsechna_kouzla_pouzita = false;
                }
            }
            if(vsechna_kouzla_pouzita)  // Kdyz byla pouzita vsechna kouzla, tak konec
            {
                // hexova smrst
                cout << "HEXARA útočí hexovou smrští všech svých kouzel, kterou žádný hrdina nemůže přežít. " << endl;
                konec(false);
            }
            srand(time(0));
            int id_kouzlo = rand() % 7;
            string kouzlo = kouzla[id_kouzlo].nazev;
            float sila_kouzla = kouzla[id_kouzlo].uder * monstra[mista[pozice].monstra[index]].utok;
            if (id_kouzlo == minule_kouzlo)
            {
                sila_kouzla *= 2;
            }
            kouzla[id_kouzlo].pouzito = true;
            minule_kouzlo = id_kouzlo;

            cout << "HEXARA útočí a používá kouzlo " << kouzlo << ", Hrdina má " << hrdinove[muj_hrdina].zivoty << " životů" << endl;
            hrdinove[muj_hrdina].zivoty = hrdinove[muj_hrdina].zivoty - sila_kouzla;
            if(kouzla[id_kouzlo].leceni > 0)
            {
                monstra[mista[pozice].monstra[index]].zivoty += kouzla[id_kouzlo].leceni;
            }
        }
        else
        {
            cout << "Monstrum útočí silou " << monstra[mista[pozice].monstra[index]].utok << ", Hrdina má " << hrdinove[muj_hrdina].zivoty << " životů" << endl;
            hrdinove[muj_hrdina].zivoty = hrdinove[muj_hrdina].zivoty - monstra[mista[pozice].monstra[index]].utok;
        }
    }

    if(hrdinove[muj_hrdina].zivoty <= 0) // je hrdina mrtvý?
    {
        cout << "Hrdina prohrál souboj" << endl;
        konec(false);
    }
}

void tah_hrdiny(int index, int cislo_tahu)
{
    string volba = "n";
    float s_utok = 1;
    bool s_utok_s = false;

    cout << "Tah " << cislo_tahu << ".: " << endl;

    cout << "Tvoje super schopnost: " << schopnosti[hrdinove[muj_hrdina].schopnost].nazev << endl;
    if (hrdinove[muj_hrdina].mana >= MANA_ZA_SCHOPNOST)
    {
        cout << "Chceš ji použít? (a/n)" << endl;
        cin >> volba;
    }
    else
    {
        cout << "Bohužel nemáš dostatek many na použití tvé schopnosti." << endl;
    }
    if (volba == "a")
    {
        hrdinove[muj_hrdina].mana -= MANA_ZA_SCHOPNOST;
        if (schopnosti[hrdinove[muj_hrdina].schopnost].nazev == "Silnější úder")
        {
            s_utok = schopnosti[hrdinove[muj_hrdina].schopnost].uder;
        }
        if (schopnosti[hrdinove[muj_hrdina].schopnost].nazev == "Oživení")
        {
            hrdinove[muj_hrdina].zivoty += schopnosti[hrdinove[muj_hrdina].schopnost].oziveni;
        }
        if (schopnosti[hrdinove[muj_hrdina].schopnost].nazev == "Omráčení")
        {
            omraceni = true;
        }
        if (schopnosti[hrdinove[muj_hrdina].schopnost].nazev == "Útok světlem")
        {
            s_utok_s = true;
        }
    }
    int pocet_monster = sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]);
    if (s_utok_s && pocet_monster > 1)
    {
        cout << "Útok světlem působí na všechna monstra v místě." << endl;

        for (int j = 0; j < pocet_monster; j++) // prochází všechna monstra v místě
        {
            if (mista[pozice].monstra[j] > 0) // přeskakuje NoMonster
            {
                cout << "Hrdina útočí silou " << hrdinove[muj_hrdina].utok * s_utok<< ", Monstrum má " << monstra[mista[pozice].monstra[j]].zivoty << " životů" << endl;
                monstra[mista[pozice].monstra[j]].zivoty = monstra[mista[pozice].monstra[j]].zivoty - hrdinove[muj_hrdina].utok * s_utok;
            }
        }
    }
    else
    {
        cout << "Hrdina útočí silou " << hrdinove[muj_hrdina].utok * s_utok<< ", Monstrum má " << monstra[mista[pozice].monstra[index]].zivoty << " životů" << endl;
        monstra[mista[pozice].monstra[index]].zivoty = monstra[mista[pozice].monstra[index]].zivoty - hrdinove[muj_hrdina].utok * s_utok;
    }


    if(monstra[mista[pozice].monstra[index]].zivoty <= 0) // je monstrum mrtvé?
    {
        cout << "Hrdina vyhrál souboj ";
        cout << "a má nyní tolik zkušeností: " << ++hrdinove[muj_hrdina].zkusenosti << endl;
        if (hrdinove[muj_hrdina].zkusenosti % 3 == 0) // navyseni levelu
        {
            if(hrdinove[muj_hrdina].level < sizeof(levels)/sizeof(levels[0]) - 1) // máme další level?
            {
                hrdinove[muj_hrdina].level++;
                cout << "Hrdinovi se zvýšil level na úroveň " << hrdinove[muj_hrdina].level << endl;
                hrdinove[muj_hrdina].max_zivoty += levels[hrdinove[muj_hrdina].level].plus_zivoty;
                hrdinove[muj_hrdina].max_mana += levels[hrdinove[muj_hrdina].level].plus_mana;
            }
        }

        srand(time(0));
        int sance = rand() % 2;
        if (monstra[mista[pozice].monstra[index]].boss) //boss dava vzdy penize
        {
            sance = 1;
        }
        if (sance) // sance ziskat penize z boje
        {
            int korist = (10 * (rand() % 6) + 10);
            korist = korist * levels[hrdinove[muj_hrdina].level].odmena; // násobení koeficientem odměny dle levelu
            //if(debug) {cout << "Korist: " << korist << endl;}
            hrdinove[muj_hrdina].penize += korist;
            cout << "Monstrum mělo u sebe " << korist << " peněz." << endl;
        }

        cout << "Hrdina má nyní u sebe " << hrdinove[muj_hrdina].penize << " peněz." << endl;
        if(debug) vypis_hrdiny();
    }
}

void boj() // řeší boj s jedním nebo více monstry na daném místě
{
    for (int i = 0; i < sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]); i++) // prochazi monstra v miste
    {
        bool vyhra_souboj = false;
        int tah = 1;
        if(mista[pozice].monstra[i] > 0 && monstra[mista[pozice].monstra[i]].zivoty > 0) // jsou v místě živá monstra?
        {
            system("pause");
            system("cls");
            cout << "\nBoj s monstrem " << monstra[mista[pozice].monstra[i]].jmeno << endl;
            cout << monstra[mista[pozice].monstra[i]].popis << endl;
            system("pause");
            cout << endl;

            do // zacatek boje s jednim monstrem
            {
                if (monstra[mista[pozice].monstra[i]].boss) // v případě bosse zahajuje boj boss
                {
                    tah_monstra(i, tah);
                    tah++;
                    tah_hrdiny(i, tah);
                    tah++;
                    if(monstra[mista[pozice].monstra[i]].zivoty <= 0) // je monstrum mrtvé?
                    {
                        vyhra_souboj = true;
                    }
                }
                else // boj zahajuje hrdina
                {
                    tah_hrdiny(i, tah);
                    tah++;
                    if(monstra[mista[pozice].monstra[i]].zivoty <= 0) // je monstrum mrtvé?
                    {
                        vyhra_souboj = true;
                    }
                    else
                    {
                        tah_monstra(i, tah);
                        tah++;
                    }
                }
            } while(!vyhra_souboj);
        }
    }
}

void pohyb()
{
    int k = 0;      //pomocna ke klavesam
    int m = 0;      //pomocna k monstrum
    string klavesa;
    int kod_klavesy;
    int xMisto[5];
    string xKlavesa[5];
    bool spravna_klavesa = false;

    cout << endl << "Nacházíš se na místě " << mista[pozice].nazev << endl;

    int pocet_monster = sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]);

    for (int j = 0; j < pocet_monster; j++) // prochází všechna monstra v místě
    {
        if (mista[pozice].monstra[j] > 0) // přeskakuje NoMonster
        {
            cout << "Na tomto místě je toto monstrum: " << monstra[mista[pozice].monstra[j]].jmeno << endl;
            m++;
        }
    }
    if(m) // jsou v místě nějaká monstra?
    {
        boj();
    }
    if(mista[pozice].typ == "vesnice") // pokud seš ve vesnici
    {
        vesnice();
    }

    cout << endl << "Z tohoto místa můžeš jít..." << endl;

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

    cout << "\nKam chceš jít?\n";

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

    int pocet_hrdinu = sizeof(hrdinove)/sizeof(hrdinove[0]);
    char opravdu;

    // Vyber hrdiny
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

            if(cin.fail())
            {
                //cout << "error\n";
                muj_hrdina = 9;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

        } while (muj_hrdina < 0 || muj_hrdina >= pocet_hrdinu);

        vypis_hrdiny();
        cout << "Chceš tuto postavu (a/n)?";
        cin >> opravdu;

    } while (opravdu != 'a' && opravdu != 'A');

    // Zacatek hry
    do
    {
        system("cls");      //pouziti clear screen, https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/, dokumentace c++
        pohyb();

        if (pozice == posledni_misto)
        {
            konec(true);
        }
       // konec_hry = true;
    } while (true);

    return 0;
}
