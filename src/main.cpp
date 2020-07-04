//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct real_state_ad {
    string id;
    string titulo;
    string descripcion;
    string tipodepropiedad;
    string direccion;
    string ciudad;
    string provincia;
    string antiguedad;
    string habitaciones;
    string garages;
    string banos;
    string metroscubiertos;
    string metrostotales;
    string idzona;
    string lat;
    string lng;
    string fecha;
    string gimnasio;
    string usosmultiples;
    string piscina;
    string escuelascercanas;
    string centroscomercialescercanos;
    string precio;
};

void set_real_state_ad(real_state_ad *ad, string attr, unsigned int i) {
    if (i == 0) ad->id = attr;
    if (i == 1) ad->titulo = attr;
    if (i == 2) ad->descripcion = attr;
    if (i == 3) ad->tipodepropiedad = attr;
    if (i == 4) ad->direccion = attr;
    if (i == 5) ad->ciudad = attr;
    if (i == 6) ad->provincia = attr;
    if (i == 7) ad->antiguedad = attr;
    if (i == 8) ad->habitaciones = attr;
    if (i == 9) ad->garages = attr;
    if (i == 10) ad->banos = attr;
    if (i == 11) ad->metroscubiertos = attr;
    if (i == 12) ad->metrostotales = attr;
    if (i == 13) ad->idzona = attr;
    if (i == 14) ad->lat = attr;
    if (i == 15) ad->lng = attr;
    if (i == 16) ad->fecha = attr;
    if (i == 17) ad->gimnasio = attr;
    if (i == 18) ad->usosmultiples = attr;
    if (i == 19) ad->piscina = attr;
    if (i == 20) ad->escuelascercanas = attr;
    if (i == 21) ad->centroscomercialescercanos = attr;
    if (i == 22) ad->precio = attr;
}

tuple<string, string> get_phrase_token(string line) {
    string token = line;
    unsigned int token_end = token.find('"');
    string attr = token.substr(0, token_end);
    token = token.substr(token_end + 2);

    return make_tuple(token, attr);
}

bool is_phrase_token(string line) {
    return line[0] == '"';
}

void print_real_state_ad(real_state_ad ad) {
    cout << "Anuncio inmobiliario" << endl;
    cout << "id: " << ad.id << endl;
    cout << "titulo: " << ad.titulo << endl;
    cout << "descripcion: " << ad.descripcion << endl;
    cout << "tipodepropiedad: " << ad.tipodepropiedad << endl;
    cout << "direccion: " << ad.direccion << endl;
    cout << "ciudad: " << ad.ciudad << endl;
    cout << "provincia: " << ad.provincia << endl;
    cout << "antiguedad: " << ad.antiguedad << endl;
    cout << "habitaciones: " << ad.habitaciones << endl;
    cout << "garages: " << ad.garages << endl;
    cout << "banos: " << ad.banos << endl;
    cout << "metroscubiertos: " << ad.metroscubiertos << endl;
    cout << "metrostotales: " << ad.metrostotales << endl;
    cout << "idzona: " << ad.idzona << endl;
    cout << "lat: " << ad.lat << endl;
    cout << "lng: " << ad.lng << endl;
    cout << "fecha: " << ad.fecha << endl;
    cout << "gimnasio: " << ad.gimnasio << endl;
    cout << "usosmultiples: " << ad.usosmultiples << endl;
    cout << "piscina: " << ad.piscina << endl;
    cout << "escuelascercanas: " << ad.escuelascercanas << endl;
    cout << "centroscomercialescercanos: " << ad.centroscomercialescercanos << endl;
    cout << "precio: " << ad.precio << endl << endl;
}

tuple<string, string, int> get_phrase_token_attr(string token, string attr, ifstream &file, string line) {
    token = token.substr(1);
    int token_end = token.find('"');
    if (token_end == -1) {
        attr = token;
        while (token_end == -1) {
            getline(file, line);
            token = line;
            token_end = token.find('"');
            if (token_end != -1) {
                token = token.substr(0, token_end);
            }
            attr = attr + token;
        }
        token = line.substr(token_end + 2);
    } else {
        tuple<string, string> phrase_token = get_phrase_token(token);
        token = get<0>(phrase_token);
        attr = get<1>(phrase_token);
    }

    return make_tuple(token, attr, token_end);
}

void read_csv(string input_csv) {
    ifstream file;

    file.open(input_csv);
    if (!file) {
        cout << "Error al abrir el archivo: " << input_csv << endl;
        exit(1);
    }

    string line;
    getline(file, line);


    string attr;
    string token = line;
    string delimiter = ",";
    unsigned int token_end = 0;
    unsigned int cols_amounts = 23;

    vector<real_state_ad> real_state_ads; 

    while (!file.eof()) {

        getline(file, line);
        token = line;

        if (line.length() > 0) {
            real_state_ad ad;
            for (int i = 0; i < cols_amounts; ++i) {

                if (is_phrase_token(token)) {
                    tuple<string, string, int> phrase_token_attr_token_end = get_phrase_token_attr(token, attr, file, line);
                    token = get<0>(phrase_token_attr_token_end);
                    attr = get<1>(phrase_token_attr_token_end);
                    token_end = get<2>(phrase_token_attr_token_end);
                } else {
                    token_end = token.find(delimiter);
                    attr = token.substr(0, token_end);
                    token = token.substr(token_end + 1);
                }
                set_real_state_ad(&ad, attr, i);
                real_state_ads.push_back(ad);
            }

            print_real_state_ad(ad);
        }
    }

    file.close();
}

void read_input_params(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Debe ingresar 3 parametros de entrada" << endl;
        exit(1);
    }

    string train_set = argv[1];
    string test_set = argv[2];
    string output = argv[3];

    cout << endl;
    cout<< "Train_set: " << train_set << endl;
    cout<< "Test_set: " << test_set << endl;
    cout<< "Output: " << output << endl;
    cout << endl;

    read_csv(train_set);
}

int main(int argc, char** argv){

    auto t0 = clock();
    read_input_params(argc, argv);
    auto t1 = clock();

    double time = (double(t1 - t0)/CLOCKS_PER_SEC);
    cout << "Execution Time (read input params): " << time << " seconds" << endl;
    cout << endl;

  return 0;
}
