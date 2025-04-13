#include <iostream>
using namespace std;

int main() {
    int volba;

    cout << "Vyber classu:" << endl; //dopsat vice class, Popis jednotlivých class, upreavit hodnoty, rozmyslet pokracovani!!!!
    cout << "1) Paladin" << endl;
    cout << "2) Lovec" << endl;
    cout << "Volba: ";
    cin >> volba;

    switch (volba) {
        case 1:
            cout << endl;
            cout << "Classa: Paladin" << endl;
            cout << "Zivoty: 5" << endl;
            cout << "Mana: 5" << endl;
            cout << "Utok: 3" << endl;
            cout << "Schopnosti:\n- Utok: Utoci za 3 (0 many)\n- Svetlo: 1 vsem nepratelum (2 many)\n- Leceni: Prida 1 zivot (1 mana)" << endl;
            break;

        case 2:
            cout << endl;
            cout << "Classa: Lovec" << endl;
            cout << "Zivot: 4" << endl;
            cout << "Mana: 3" << endl;
            cout << "Utok: 4" << endl;
            cout << "Schopnosti:\n- Strelba: Utoci za 4 (0 many)\n- Silna strela: Utoci za 6 (2 many)\n- Uhyb: Vyhne se utoku (1 mana)" << endl;
            break;

        default:
            cout << "Neplatna volba. Hra konci." << endl;
            return 0;
    }

    cout << endl;
    cout << "Classa vybrana. Zacatek hry..." << endl;

    return 0;
}
