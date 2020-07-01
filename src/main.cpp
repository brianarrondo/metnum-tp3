//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

tuple<string, string> get_phrase_token(string line) {
    string token = line.substr(1);
    unsigned int token_end = token.find('"');
    string attr = token.substr(0, token_end);
    token = token.substr(token_end + 2);

    return make_tuple(token, attr);
}

bool is_phrase_token(string line) {
    return line[0] == '"';
}

vector<string> split(string line, string delimiter) {
    vector<string> V;
    string token = line;
    unsigned int token_end = 0;

    string attr;
    while (token_end < line.length()) {
        if (is_phrase_token(token)) {
            tuple<string, string> phrase_token = get_phrase_token(token);
            token = get<0>(phrase_token);
            attr = get<1>(phrase_token);
        } else {
            token_end = token.find(delimiter);
            attr = token.substr(0, token_end);
            token = token.substr(token_end + 1);
        }
        V.push_back(attr);
    }

    return V;
}

void print_real_state_ad(vector<string> real_state_ad) {
    string labels[23] = {"id","titulo","descripcion","tipodepropiedad","direccion","ciudad","provincia","antiguedad","habitaciones","garages","banos","metroscubiertos","metrostotales","idzona","lat","lng","fecha","gimnasio","usosmultiples","piscina","escuelascercanas","centroscomercialescercanos","precio"};
    for (int i = 0; i < real_state_ad.size(); ++i) {
        cout << labels[i] << "\t: " << real_state_ad[i] << endl;
    }
    cout << endl << endl;
}

void read_csv(string input_csv) {
    ifstream file;

    file.open(input_csv);
    if (!file) {
        cout << "Error al abrir el archivo: " << input_csv << endl;
        exit(1);
    }

    string line;
    string delimiter = ",";
    unsigned int index = 0;

    getline(file, line);

    while (!file.eof()) {
        getline(file, line);

        if (line.length() > 0) {
            vector<string> real_state_ad = split(line, delimiter);
            print_real_state_ad(real_state_ad);
            index++;
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
