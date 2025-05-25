#include <iostream>
#include <conio.h>
#include <windows.h>        //česká diakritika
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;


    int pozice          = 0;
    int muj_hrdina;
    bool konec_hry      = false;
    bool debug          = true;
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
        float odmena;
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
        // max_zivoty; max_mana; odmena;
        {100, 100, 1},
        {110, 100, 1.1},
        {115, 110, 1.2}
    };

    Schopnost schopnosti[] =
    {
        // nazev; uder; multi_uder; oziveni;
        {"NoSchopnost", 1, false, 0},
        {"Silnější úder", 1.5, false, 0},
        {"Útok světlem", 1, true, 0},
        {"Oživení", 1, false, 10}
    };
    Kouzlo kouzla[] =
    {
        // nazev; uder;
        {"NoKouzlo", 1},
        {"Blesk", 1.5},
        {"Hrom", 1.3}
    };

    Hrdina hrdinove[] =
    {
        // jmeno; id; popis; zivoty; mana; penize; level; zkusenosti; utok; schopnosti
        {"Paladin", 0, "popis Paladina", 100, 100, 10, 0, 0, 60,{}},
        {"Lovec", 1, "popis Lovce", 80, 100, 10, 0, 0, 80},
        {"Mag", 2, "popis Maga", 1, 120, 10, 0, 0, 50},
        {"Warlock", 3, "popis Warlocka", 120, 80, 10, 0, 0, 50}
    };

    Mostrum monstra[] =
    {
        // jmeno; popis; zivoty; utok; kouzla
        {"NoMonster", "", 0, 0, {}},
        {"Stínokrad", "Tvor z temnoty, který se zjevuje tam, kde se šíří strach. Jeho tělo tvoří živoucí stín.", 100, 10, {}},                                                      //1(cave)                   1
        {"Kostěný rváč", "Nemrtvý válečník se zrezivělým mečem a popraskaným štítem. I po smrti touží po souboji.", 80, 20, {}},                                                    //7(hřbitov)                2
        {"Jeskynní škrabač", "Malý, ale nebezpečný tvor se silnými drápy. Vydává děsivé zvuky, než zaútočí ze tmy.", 80, 20, {}},                                                   //1(cave)                   3
        {"Bažinný slizák", "Mazlavá hrouda hniloby, co se plazí mokřady a pohlcuje vše živé.", 80, 20, {}},                                                                         //3(Les plný mokřadů)       4
        {"Plamenný netvor", "Bytost ze sfér ohně, hořící plameny ji obklopují i během boje.", 80, 20, {}},                                                                          //4(Zřicenina hradu)        5
        {"Lesní běs", "Zkroucený duch přírody, co chrání zapomenuté lesy. Vzývá stromy a trny.", 80, 20, {}},                                                                       //3(Les plný mokřadů)       6
        {"Kamenný kolos", "Obrovitý tvor z kamene, který se pohybuje pomalu, ale devastuje jediným úderem.", 80, 20, {}},                                                           //1(cave)                   7
        {"Krvesajný havran", "Zmutovaný pták velikosti psa, který útočí ve skupinách a cílí na oči.", 80, 20, {}},                                                                  //5(opuštěná ZOO)           8
        {"Krveplaz", "Rozervaný tvor s trhlinami v těle, z nichž visí kůže a maso. Jeho útoky jsou brutální a neúprosné.", 80, 20, {}},                                             //6(opuštělá chalupa)       9
        {"Dýmová příšera", "Nestvůra z černého kouře, která nemá tvar ani duši. Dusí vše, co vdechuje její plyn.", 80, 20, {}},                                                     //4(Zřicenina hradu)        10
        {"Chrlič zatracení", "Oživená kamenná socha z chrámu padlých bohů, věčně bdí a zabíjí vše cizí.", 80, 20, {}},                                                              //7(hřbitov)                11
        {"Šeptač nocí", "Neviditelný přízrak, který láme mysl šeptáním. Je slyšet, ale málokdo ho viděl.", 80, 20, {}},                                                             //2(děsivé místo)           12
        {"Ledová zmije", "Had s krystaly ledu místo šupin. Její dech mrzne krev v žilách.", 80, 20, {}},                                                                            //5(opuštěná ZOO)           13
        {"Bezlící Mučitel", "Bezlící mučitel bez hlavy, který bez zastavení loví a mučí duše svých obětí věčnou bolestí.", 80, 20, {}},                                             //6(opuštělá chalupa)       14
        {"Pavučinová matka", "Obrovský pavouk s lidskou tváří. Ovládá menší pavouky a své oběti balí zaživa.", 80, 20, {}},                                                         //5(opuštěná ZOO)           15
        {"Půlnoční běs", "Noční jezdec beze jména, zjevuje se za úplňku. Jeho oči žhnou a kůň nevrhá stín.", 80, 20, {}},                                                           //2(děsivé místo)           16
        {"Mikeš", "Na pohled roztomilá černá kočička, ve skutečnosti zákeřný démon. Přede, uspává nepřátele a útočí ze stínů. Přináší smůlu těm, kdo se ho dotknou.", 80, 20, {}},  //8(brloch)                 17
        {"MB", "MB", 80, 20, {}},
        {"MB2", "MB2", 80, 20, {}},
        {"HEXARA", "HEXARA", 80, 20, {}}
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
    cout << "- Max životy: " << levels[hrdinove[muj_hrdina].level].max_zivoty << endl;
    cout << "- Max mana: " << levels[hrdinove[muj_hrdina].level].max_mana << endl;
    cout << "Zkušenosti: " << hrdinove[muj_hrdina].zkusenosti << endl;
    cout << "Útok: " << hrdinove[muj_hrdina].utok << endl << endl;
}

void vesnice()
{
    int vyber = 0;
    cout << "Ve vesnici si můžeš doplnit životy, vylepšit max životy, manu a útok." << endl;
    do
    {
    cout << "Tvé staty:" << endl;
    cout << "\nJméno: " << hrdinove[muj_hrdina].jmeno << endl;
    cout << "Životy: " << hrdinove[muj_hrdina].zivoty << "/" << levels[hrdinove[muj_hrdina].level].max_zivoty << endl;
    cout << "Mana: " << hrdinove[muj_hrdina].mana << "/" << levels[hrdinove[muj_hrdina].level].max_mana << endl;
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
        case 1:
            hrdinove[muj_hrdina].zivoty = levels[hrdinove[muj_hrdina].level].max_zivoty;
            hrdinove[muj_hrdina].penize -= 20;
            cout << endl;
            break;

        case 2:
            levels[hrdinove[muj_hrdina].level].max_zivoty = levels[hrdinove[muj_hrdina].level].max_zivoty + 20;
            hrdinove[muj_hrdina].penize -= 20;
            cout << endl;
            break;

        case 3:
            hrdinove[muj_hrdina].mana = levels[hrdinove[muj_hrdina].level].max_mana;
            hrdinove[muj_hrdina].penize -= 20;
            cout << endl;
            break;

        case 4:
            levels[hrdinove[muj_hrdina].level].max_mana = levels[hrdinove[muj_hrdina].level].max_mana + 20;
            hrdinove[muj_hrdina].penize -= 20;
            cout << endl;
            break;

        case 5:
            hrdinove[muj_hrdina].utok = hrdinove[muj_hrdina].utok + 20;
            hrdinove[muj_hrdina].penize -= 20;
            cout << endl;
            break;

        case 6:
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

void boj()
{
    /**
    cout << hrdinove[muj_hrdina].jmeno << endl;
    for (int i = 0; i < sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]); i++)
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
    **/

    for (int i = 0; i < sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]); i++)
    {
        bool vyhra_souboj = false;
        int tah = 1;
        if(mista[pozice].monstra[i] > 0 && monstra[mista[pozice].monstra[i]].zivoty > 0)
        {
            system("pause");
            system("cls");
            cout << "\nBoj s monstrem " << monstra[mista[pozice].monstra[i]].jmeno << endl;
            cout << monstra[mista[pozice].monstra[i]].popis << endl;
            system("pause");
            cout << endl;

            do
            {
                cout << "Tah " << tah++ << ".: " << endl;
                cout << "Hrdina útočí silou " << hrdinove[muj_hrdina].utok << ", Monstrum má " << monstra[mista[pozice].monstra[i]].zivoty << " životů" << endl;
                monstra[mista[pozice].monstra[i]].zivoty = monstra[mista[pozice].monstra[i]].zivoty - hrdinove[muj_hrdina].utok;

                if(monstra[mista[pozice].monstra[i]].zivoty <= 0)
                {
                    cout << "Hrdina vyhrál souboj ";
                    cout << "a má nyní tolik zkušeností: " << ++hrdinove[muj_hrdina].zkusenosti << endl;
                    if (hrdinove[muj_hrdina].zkusenosti % 3 == 0)
                    {
                        hrdinove[muj_hrdina].level++;
                        cout << "Hrdinovi se zvýšil level na úroveň " << hrdinove[muj_hrdina].level << endl;
                    }

                    srand(time(0));
                    int sance = rand() % 2;
                    if (sance)
                    {
                        int korist = (10 * (rand() % 6) + 10);
                        korist = korist * levels[hrdinove[muj_hrdina].level].odmena;
                        //if(debug) {cout << "Korist: " << korist << endl;}
                        hrdinove[muj_hrdina].penize = hrdinove[muj_hrdina].penize + korist;
                        cout << "Monstrum mělo u sebe " << korist << " peněz." << endl;
                    }
                    cout << "Hrdina má nyní u sebe " << hrdinove[muj_hrdina].penize << " peněz." << endl;
                    vyhra_souboj = true;
                    if(debug) vypis_hrdiny();
                }
                else
                {
                    cout << "Tah " << tah++ << ".: " << endl;
                    cout << "Monstrum útočí silou " << monstra[mista[pozice].monstra[i]].utok << ", Hrdina má " << hrdinove[muj_hrdina].zivoty << " životů" << endl;
                    hrdinove[muj_hrdina].zivoty = hrdinove[muj_hrdina].zivoty - monstra[mista[pozice].monstra[i]].utok;

                    if(hrdinove[muj_hrdina].zivoty <= 0)
                    {
                        cout << "Hrdina prohrál souboj" << endl;
                        konec(false);
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
        //boj();
    }
    if(mista[pozice].typ == "vesnice")
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
            konec_hry = true;
        }
       // konec_hry = true;
    } while (!konec_hry);

    cout << "\nGAME OVER!!" << endl;
    return 0;
}
