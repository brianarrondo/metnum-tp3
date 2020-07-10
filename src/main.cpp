//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "linear_regression.h"

using namespace std;

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

void print_real_state_ad(vector<string> ad) {
    cout << "Anuncio inmobiliario" << endl;
    cout << "id: " << ad[0] << endl;
    cout << "titulo: " << ad[1] << endl;
    cout << "descripcion: " << ad[2] << endl;
    cout << "tipodepropiedad: " << ad[3] << endl;
    cout << "direccion: " << ad[4] << endl;
    cout << "ciudad: " << ad[5] << endl;
    cout << "provincia: " << ad[6] << endl;
    cout << "antiguedad: " << ad[7] << endl;
    cout << "habitaciones: " << ad[8] << endl;
    cout << "garages: " << ad[9] << endl;
    cout << "banos: " << ad[10] << endl;
    cout << "metroscubiertos: " << ad[11] << endl;
    cout << "metrostotales: " << ad[12] << endl;
    cout << "idzona: " << ad[13] << endl;
    cout << "lat: " << ad[14] << endl;
    cout << "lng: " << ad[15] << endl;
    cout << "fecha: " << ad[16] << endl;
    cout << "gimnasio: " << ad[17] << endl;
    cout << "usosmultiples: " << ad[18] << endl;
    cout << "piscina: " << ad[19] << endl;
    cout << "escuelascercanas: " << ad[20] << endl;
    cout << "centroscomercialescercanos: " << ad[21] << endl;
    cout << "precio: " << ad[22] << endl << endl;
}

void set_real_state_ad_to_matrix(vector<string> ad, Matrix &A, unsigned int i) {
    unsigned int index = 0;
    for (int j = 7; j < ad.size(); ++j) {
        double ad_attr = ad[j].length() ? stod(ad[j]) : 0;
        A(i,index) = ad_attr;
        index++;
    }
}

Matrix remove_last_column(Matrix X) {
    X.conservativeResize(X.rows(), X.cols() - 1);
    return X;
}

Vector get_last_column(Matrix X) {
    return X.col(X.cols() - 1);
}

// csv_type = 0 si se trata de datos de entrenamiento | 1 si se trata de datos de prueba
vector<vector<string>> read_csv(string csv_input, unsigned int csv_type) {
    ifstream file;

    file.open(csv_input);
    if (!file) {
        cout << "Error al abrir el archivo: " << csv_input << endl;
        exit(1);
    }

    string line;
    getline(file, line);


    string attr;
    string token = line;
    string delimiter = ",";
    unsigned int token_end = 0;

    // Si parseamos los datos de prueba tenemos 22 columnas
    // Si parseamos los datos de entrenamiento tenemos 22 columnas + 1 columna correspondiente al  precio
    unsigned int cols_amounts = csv_type ? 22 : 23;
    vector<vector<string>> real_state_ads; 

    while (!file.eof()) {

        getline(file, line);
        token = line;

        if (line.length() > 0) {
            vector<string> ad(cols_amounts);
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
                ad[i] = attr;
            }

            real_state_ads.push_back(ad);
            // print_real_state_ad(ad);
        }
    }

    file.close();

    return real_state_ads;
}


int main(int argc, char** argv){

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

    // Se parsean los datos del csv (entrenamiento y prueba), transformandolos en un vector de anuncios inmobiliarios (anuncio = vector<string>)
    auto t0 = clock();
    vector<vector<string>> real_state_ads_train = read_csv(train_set, 0);
    vector<vector<string>> real_state_ads_test = read_csv(test_set, 1);
    auto t1 = clock();
    cout << "Cantidad de instancias de entrenamiento: " << real_state_ads_train.size() << endl;
    cout << "Cantidad de instancias de prueba: " << real_state_ads_test.size() << endl << endl;

    double time = (double(t1 - t0)/CLOCKS_PER_SEC);
    cout << "Execution Time (read input params): " << time << " seconds" << endl << endl;

    // Transformo el vector de instancias de entrenamiento en una matriz, considerando solo las caracteristicas medibles
    unsigned int train_rows = real_state_ads_train.size();
    unsigned int train_cols = 16;
    Matrix train_matrix = Matrix(train_rows, train_cols);
    for (unsigned int i = 0; i < train_rows; ++i) {
        set_real_state_ad_to_matrix(real_state_ads_train[i], train_matrix, i);
    }

    // Transformo el vector de instancias de prueba en una matriz, considerando solo las caracteristicas medibles
    unsigned int test_rows = real_state_ads_test.size();
    unsigned int test_cols = 15;
    Matrix X_test = Matrix(test_rows, test_cols);
    for (unsigned int i = 0; i < test_rows; ++i) {
        set_real_state_ad_to_matrix(real_state_ads_test[i], X_test, i);
    }

    // Creo las matrices de instancias de entrenamiento, considerando solo algunas caracteristicas de los anuncios inmobiliarios
    // X_train = matriz con todas las caracteristicas medibles (variables numericas) de un anuncio inmobiliario
    Matrix X_train = remove_last_column(train_matrix);

    // x_train = se seleccionan algunas de las caracteristicas de los anuncios inmobiliarios
    Matrix x_train = X_train;
    x_train.conservativeResize(x_train.rows(), 2);

    // y_train = el precio del anuncio inmobiliario correspondiente en x_train
    Matrix y_train = get_last_column(train_matrix);

    // Creo la matriz con instancias de prueba, seleccionando alguna de las caracteristicas de los anuncios
    Matrix x_test = X_train;
    x_test.conservativeResize(x_test.rows(), 2);

    // Instanciamos el objeto LinearRegression
    LinearRegression linear_regression = LinearRegression();

    // Entrenamos nuestro clasificador con los datos de entrenamiento
    linear_regression.fit(x_train, y_train);

    // Predecimos las instancias de prueba
    Vector y = linear_regression.predict(x_test);

    cout << y << endl;

    t1 = clock();
    time = (double(t1 - t0)/CLOCKS_PER_SEC);
    cout << "Execution Time (total): " << time << " seconds" << endl << endl;

    return 0;
}
