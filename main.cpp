#include <iostream>

#include "src/Data.h"
#include "src/Menu.h"

int main()
{   int o;
    std::cout << "Digite: \n";
    std::cout << "1- Para usar a rede ferroviária padrão\n";
    std::cout << "2- Para usar uma rede ferroviária fornecida\n";
    std::cin >> o;
    std::cin.ignore();

    if (o == 1) {
        Data data("../dataset/stations.csv","../dataset/network.csv");
        Menu menu(data.getRailwayGraph());
    } else {
        std::string file1, file2;
        std::cout << "Digite o nome do ficheiro de estações que deseja utilizar\n";
        getline(std::cin, file1);
        std::cout << "Digite o nome do ficheiro de conexões que deseja utilizar\n";
        getline(std::cin, file2);
        Data data(file1, file2);
        Menu menu(data.getRailwayGraph());
    }

    return 0;
}
