// Autor: Marek Sýkora
// Popisky, názvy a příběh hry byly vytvořeny za pomocí ChatGPT

// TODO:
// sjednotit ovládání voleb
// obrazovka v ANSI grafice
// úrovně obtížnosti

#include <iostream>
#include <conio.h>
#include <windows.h>        //česká diakritika
#include <cstdlib>
#include <ctime>            //řešení náhodnosti
#include <limits>           //řešení náhodnosti
using namespace std;

    //int tmp_kouzlo      = 0; // test hexové smršti
    int pozice          = 0;
    int muj_hrdina;
    bool konec_hry      = false;
    string klavesy[5]   = {"q", "w", "e", "r", "t"};
    bool omraceni       = false;
    int minule_kouzlo   = 0;

    // Parametry hry

    const int CENA_ZA_UPGRADE = 20;     //Cena upgradu ve vesnici
    const int NAVYSENI_MAX = 20;        //O kolik se navýši max po upgradu ve vesnici
    const int MANA_ZA_SCHOPNOST = 30;   //Kolik many stojí použití schopnosti

    // Barvy textu

    const INT BLACK = 0;
    const INT DARK_BLUE = 1;
    const INT DARK_GREEN = 2;
    const INT DARK_CYAN = 3;
    const INT DARK_RED = 4;
    const INT DARK_MAGENTA = 5;
    const INT DARK_YELLOW = 6;
    const INT GREY = 7;
    const INT DARK_GREY = 8;
    const INT BLUE = 9;
    const INT GREEN = 10;
    const INT CYAN = 11;
    const INT RED = 12;
    const INT MAGENTA = 13;
    const INT YELLOW = 14;
    const INT WHITE = 15;

    // DATOVÉ STRUKTURY

    struct Hrdina          //pouziti struktur, ChatGPT, https://www.w3schools.com/cpp/cpp_structs.asp, dokumentace c++
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
        {10, 10, 1},
        {15, 15, 1.1},
        {20, 20, 1.2},
        {25, 25, 1.3},
        {25, 25, 1.4},
        {25, 25, 1.5},
        {25, 25, 1.6},
        {25, 25, 1.7}
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
        {"Paladin", 0, "Silný a odhodlaný válečník, který bojuje za spravedlnost a chrání své spojence. Disponuje silnějším úderem, který dokáže nepřítele tvrdě zasáhnout.", 100, 100, 100, 100, 10, 0, 0, 60, 1},
        {"Lovec", 1, "Obratný a neúnavný bojovník bojující zblízka. Specializuje se na rychlé útoky a pasti, jeho schopností je omráčení nepřítele, které mu dává taktickou výhodu.", 80, 80, 100, 100, 10, 0, 0, 50, 4},
        {"Mag", 2, "Mocný kouzelník, který ovládá elementální magii. Útočí převážně na dálku a jeho speciální schopností je útok světlem, který způsobuje výrazné škody.", 100, 100, 120, 120, 10, 0, 0, 50, 2},
        {"Warlock", 3, "Temná postava, která čerpá sílu z démonických paktů. Specializuje se na kletby a temnou magii, jeho schopností je oživení, kdy se sám dokáže vyléčit.", 80, 80, 80, 80, 10, 0, 0, 50, 3}
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
        {"Železný Hrůzostraš", "Masivní válečník v těžké železné zbroji, který spoléhá na svou brutální sílu a odolnost. Je pomalý, ale jeho údery jsou ničivé a zastrašující.", true, 300, 50, {}},//MB
        {"Temný Zrádce", "Šikovný a zákeřný protivník, který spoléhá na rychlé a přesné útoky. Je nebezpečný svou obratností a schopností využívat slabiny nepřátel.", true, 400, 75, {}},//MB
        {"HEXARA", "Tajemná a děsivá šestiruká čarodějka chaosu, která ovládá šest smrtících kouzel. Kamkoliv přijde, šíří chaos a paniku. Její přítomnost nahání hrůzu i těm nejodvážnějším.", true, 750, 100, {1, 2, 3, 4, 5, 6}}//VB
    };

    Misto mista[] =
    {
        //nazev; id; popis; penize; typ; navstiveno; pohyb[5]; monstra[3];
        {"Neznámá mýtina", 0, "Tiché místo zahalené stíny stromů, kde se světlo téměř nedostane. Všechno působí cize, jako bys sem vůbec nepatřil. Nevíš, kde přesně jsi, a okolí ti to nijak neusnadňuje.", 0, "zacatek", false, {1}},                     //start
        {"Brloch", 1, "Zem je rozrytá drápy a cítíš pach krve. Vypadá to, že tu pořád něco žije.", 0, "bojiste", false, {2, 3, 4},{17}},
        {"Opuštěná chalupa", 2, "Chalupa plná škrábanců a krve na stěnách. Není jasné, co tu před tebou bylo.", 0, "bojiste", false, {1, 4},{9, 13}},
        {"Les plný mokřadů", 3, "Bahno ukrývá slizké tvory, kteří se plazí pod hladinou. Někdy zahlédneš oko a slyšíš čvachtání.", 0, "bojiste", false, {1, 4, 5},{4, 6}},
        {"Stinná Lhota", 4, "Klidné místo mezi stromy, kde šum listí tlumí i nejtemnější myšlenky. Čas tu plyne jinak.", 0, "vesnice", false, {1, 2, 3, 5, 6}},
        {"Děsivé místo", 5, "Mlha tu nikdy neustupuje a ze tmy se ozývá šepot. Země je prokletá.", 0, "bojiste", false, {3, 4, 6, 7},{12, 16}},
        {"Hřbitov", 6, "Náhrobky jsou popraskané a mezi hroby se cosi hýbe. Tady klid mrtvým nepatří.", 0, "bojiste", false, {4, 5, 7, 8},{2, 11}},
        {"Temná Lhota", 7, "Ves zapomenutá v mlze pod útesy. Stíny se tu hýbou, i když nikdo kolem není.", 0, "vesnice", false, {5, 6, 8, 9, 10}},
        {"Zřicenina hradu", 8, "Zdi jsou ohořelé a slyšet je dávné skřeky. Temnota tu nikdy nezmizela.", 0, "bojiste", false, {6, 7, 10},{5, 10}},
        {"Opuštěná ZOO", 9, "Klece jsou zničené a mezi nimi visí podivné pavučiny. Něco obrovského tu stále přebývá.", 0, "bojiste", false, {7, 10, 11},{8, 13, 15}},
        {"Jeskyně", 10, "Uvnitř je tma, škrábance a kosti. V hlubinách cosi těžce dýchá.", 0, "bojiste", false, {7, 8, 9, 11},{1, 3, 7}},
        {"Tvrz zatracení", 11, "Zrezivělé zbraně a dunivé kroky z dálky naznačují přítomnost kolosu. Kovový strážce se nikdy nezastaví.", 0, "bojiste", false, {9, 10, 12},{18}},//MB
        {"Útočiště Zrádce", 12, "Zřícené svatyni vévodí krví potřísněný oltář. Vzduch vibruje rychlým pohybem něčeho smrtícího.", 0, "bojiste", false, {11, 13},{19}},//MB2
        {"Tichá Lhota", 13, "Tiché domy stojí na spáleništi minulosti. Z komínů už nestoupá kouř, jen vzpomínky.", 0, "vesnice", false, {12, 14}},
        {"Chrám chaosu", 14, "Chrám plný temné magie, kde stěny pulzují chaosem. Ze stínu se ozývá děsivý smích.", 0, "bojiste", false, {15},{20}},//VB
        {"Konec", 15, "Všude je prázdno, mrtvý vzduch a popraskaná zem. Něco skončilo – nebo začalo?", 0, "konec", false, {},{}}//konec
    };

    int pocet_mist      = sizeof(mista)/sizeof(mista[0]);
    int posledni_misto  = pocet_mist - 1;

void barva(int color = GREY)//Vlastní funkce na změnu barvy textu
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void pauza(string text = "Jestli se nebojíš, pokračuj kliknutím na cokoli...")//Vlastní funkce pauza
{
    barva(DARK_RED);
    cout << text << endl;
    barva();
    getch();
}

void konec(bool vyhra)
{
    if(vyhra)
    {
        barva(GREEN);
        cout << "Gratuluju, hru jsi úspěšně dohrál" << endl;
    }
    else
    {
        barva(RED);
        cout << "Bohužel jsi prohrál. Zkus to příště." << endl;
    }
    barva(WHITE);
    cout << "Jestli se ti hra líbila, nebo k ní máš nějaké připomínky nebo nápady, napiš mi na marksyxx@gmail.com.";
    barva();
    exit(0);
}

void vypis_hrdiny()
{
    barva(GREEN);
    cout << "\nJméno: " << hrdinove[muj_hrdina].jmeno << endl;
    cout << "Životy: " << hrdinove[muj_hrdina].zivoty << "/" << hrdinove[muj_hrdina].max_zivoty << endl;
    cout << "Mana: " << hrdinove[muj_hrdina].mana << "/" << hrdinove[muj_hrdina].max_mana << endl;
    cout << "Peníze: " << hrdinove[muj_hrdina].penize << endl;
    cout << "Level: " << hrdinove[muj_hrdina].level << endl;
    cout << "Zkušenosti: " << hrdinove[muj_hrdina].zkusenosti << endl;
    cout << "Útok: " << hrdinove[muj_hrdina].utok << endl;
    cout << "Schopnost: " << schopnosti[hrdinove[muj_hrdina].schopnost].nazev << endl << endl;
    barva();
}

void vesnice() // řeší možnosti vesnice, doplnění vlastností hrdiny
{
    int vyber = 0;
    cout << "Ve vesnici si můžeš doplnit životy, vylepšit max životy, manu a útok." << endl;
    do
    {
        barva(GREEN);
        cout << "\nTvuj status:" << endl; //Tátův archaismus
        cout << "Jméno: " << hrdinove[muj_hrdina].jmeno << endl;
        cout << "Životy: " << hrdinove[muj_hrdina].zivoty << "/" << hrdinove[muj_hrdina].max_zivoty << endl;
        cout << "Mana: " << hrdinove[muj_hrdina].mana << "/" << hrdinove[muj_hrdina].max_mana << endl;
        cout << "Útok: " << hrdinove[muj_hrdina].utok << endl;
        cout << "Peníze: " << hrdinove[muj_hrdina].penize << endl << endl;

        barva(DARK_GREEN);
        cout << "1) Doplní životy (" << CENA_ZA_UPGRADE << " zlaťáků)" << endl;
        cout << "2) Upgrade max-životy (" << CENA_ZA_UPGRADE << " zlaťáků)" << endl;
        cout << "3) Doplní manu (" << CENA_ZA_UPGRADE << " zlaťáků)" << endl;
        cout << "4) Upgrade max-mana (" << CENA_ZA_UPGRADE << " zlaťáků)" << endl;
        cout << "5) Upgrade útok (" << CENA_ZA_UPGRADE << " zlaťáků)" << endl;
        cout << "6) Opustit vesnici" << endl << endl;
        barva();
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
                cout << "Nemáš dostatek zlaťáků." << endl;
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
                cout << "Nemáš dostatek zlaťáků.";
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
                cout << "Nemáš dostatek zlaťáků.";
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
                cout << "Nemáš dostatek zlaťáků.";
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
                cout << "Nemáš dostatek zlaťáků.";
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
    barva(CYAN);
    cout << "Tah " << cislo_tahu << ".: " << endl;
    barva();
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
            for(int i = 0; i < sizeof(kouzla)/sizeof(kouzla[0]); i++)   // Bylo kouzlo pouzito?
            {
                if(kouzla[i].pouzito == false)  //Urcovani (ne)pouziti
                {
                    vsechna_kouzla_pouzita = false;
                    break;
                }
            }
            if(vsechna_kouzla_pouzita)  // Kdyz byla pouzita vsechna kouzla, tak konec
            {
                // hexova smrst
                cout << "HEXARA útočí hexovou smrští všech svých kouzel, kterou žádný hrdina nemůže přežít. " << endl;
                konec(false);
            }
            srand(time(0));
            int id_kouzlo = rand() % 6;
            //id_kouzlo = tmp_kouzlo++; // test hexové smršti
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

    barva(CYAN);
    cout << "Tah " << cislo_tahu << ".: " << endl;
    barva();

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
                hrdinove[muj_hrdina].max_zivoty += levels[hrdinove[muj_hrdina].level].plus_zivoty;  //+max zivoty dle levelu
                hrdinove[muj_hrdina].zivoty += levels[hrdinove[muj_hrdina].level].plus_zivoty;      //+ stejny pocet jako max zivoty
                hrdinove[muj_hrdina].max_mana += levels[hrdinove[muj_hrdina].level].plus_mana;      //+max mana dle levelu
                hrdinove[muj_hrdina].mana += levels[hrdinove[muj_hrdina].level].plus_mana;          //+ stejny pocet jako max mana
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
            hrdinove[muj_hrdina].penize += korist;
            cout << "Monstrum mělo u sebe " << korist << " zlaťáků." << endl;
        }

        cout << "Hrdina má nyní u sebe " << hrdinove[muj_hrdina].penize << " zlaťáků." << endl;
        vypis_hrdiny();
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
            pauza();
            system("cls");
            barva(CYAN);
            cout << "\nBoj s monstrem ";
            cout << monstra[mista[pozice].monstra[i]].jmeno << endl;
            barva();
            cout << monstra[mista[pozice].monstra[i]].popis << endl;
            pauza();
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

    barva(YELLOW);
    cout << mista[pozice].nazev << endl;
    barva(WHITE);
    cout << mista[pozice].popis << endl;
    barva();

    mista[pozice].navstiveno = true;

    int pocet_monster = sizeof(mista[pozice].monstra)/sizeof(mista[pozice].monstra[0]);

    for (int j = 0; j < pocet_monster; j++) // prochází všechna monstra v místě
    {
        if (mista[pozice].monstra[j] > 0) // přeskakuje NoMonster
        {
            cout << "Na tomto místě je toto monstrum: " << monstra[mista[pozice].monstra[j]].jmeno << endl;
            m++;
        }
    }
    if(m) //Jsou v místě nějaká monstra?
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
            string navstiveno = " ";
            int id_budouci = mista[pozice].pohyb[j];

            if (mista[id_budouci].navstiveno)
            {
                navstiveno = "Objeveno";
            }

            cout << mista[id_budouci].nazev << "(" << klavesy[k]<< ") ";
            barva(WHITE);
            cout << navstiveno << endl;
            barva();
            xMisto[j] = id_budouci;
            xKlavesa[j] = klavesy[k];
            k++;
        }
    }
    barva(YELLOW);
    cout << "\nKam chceš jít?\n";
    barva();
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

    barva(WHITE);
    cout << "HEXARA\n\n";
    cout << "Se zmizením slunce přišla Hexara – šestiruká čarodějnice, která zlomila rovnováhu světa. Kraj zahalil stín, lesy potemněly, mrtví vstali a monstra ovládla opuštěná místa.\n\n";
    cout << "Lidé prchli, vesnice utichly a šeptá se jen o jediné – nic nepřežije tam, kam Hexara pohlédne.\n\n";
    cout << "Ty ses probudil v Neznámé mýtině, bez jména, bez paměti, ale s pocitem, že tě sem něco vedlo. Ať už jsi kdysi byl kdokoliv, teď jsi ten, kdo může vše změnit.\n\n";
    cout << "Cíl je jediný – najít Hexaru a ukončit její vládu.\n\n";
    barva();

    pauza("Vítej ve hře Hexara, užij si její příběh! Pro začátek hry stiskni cokoli!");

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
                cout << i;
                cout << ") ";
                barva(MAGENTA);
                cout << hrdinove[i].jmeno;
                barva(WHITE);
                cout << "\n" <<hrdinove[i].popis << "\n" << endl;
                barva();
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
