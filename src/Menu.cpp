#include "Menu.h"

Menu::Menu(Graph *railwayGraph): railwayGraph(railwayGraph), metrics(railwayGraph) {
    principal();
}

void Menu::menu() {
    std::cout << "\n=====================================================================================================\n";
    std::cout << " \t\t\t\t\t\t\t\t\t\t\t\tMENU \n";
    std::cout << "=====================================================================================================\n\n";

    std::cout << "Selecione uma das seguintes opções (0-6):" << std::endl;
    std::cout << "1- Número máximo de trens que podem viajar simultaneamente entre duas estações específicas" << std::endl;
    std::cout << "2- Pares de estações que requerem a maior quantidade de trens" << std::endl;
    std::cout << "3- Top-k municípios e distritos com mais necessidade de transportes" << std::endl;
    std::cout << "4- Número máximo de trens que podem chegar simultaneamente numa estação específica" << std::endl;
    std::cout << "5- Número máximo de trens que podem viajar simultaneamente entre duas estações com custo mínimo" << std::endl;
    std::cout << "6- Número máximo de trens que podem viajar simultaneamente entre duas estações com conexão reduzida" << std::endl;
    std::cout << "7- Top-k estações mais afetadas por cada falha num segmento" << std::endl;
    std::cout << "0- Sair" << std::endl;
}

void Menu::principal() {
    do {
        menu();
        std::cin >> option;
        std::cin.ignore();

        switch (option) {
            case 0:
                quit = true;
                break;
            case 1:
                submenu1();
                break;
            case 2:
                submenu2();
                break;
            case 3:
                submenu3();
                break;
            case 4:
                submenu4();
                break;
            case 5:
                submenu5();
                break;
            case 6:
                submenu6();
                break;
            case 7:
                submenu7();
                break;

            default:
                std::cout << "Opção inválida. Favor selecionar uma opção válida\n";
                break;
        }
    } while (!quit);
}

void Menu::submenu1() {
    std::string nameA, nameB;
    Vertex *s, *t;
    int maxFlow;

    while (true) {
        std::cout << "Escolha a estação de origem:" << std::endl;
        getline(std::cin, nameA);
        s = railwayGraph->findVertex(nameA);
        if (s == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        std::cout << "Escolha a estação de destino:" << std::endl;
        getline(std::cin, nameB);
        t = railwayGraph->findVertex(nameB);
        if (t == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        break;
    }

    maxFlow = metrics.findMaxFlow(s, t);
    std::cout << "\nO número máximo de trens será: ";
    std::cout << maxFlow << std::endl;
}

void Menu::submenu2() {
    std::list<std::pair<Vertex*, Vertex*>> pairs = metrics.pairsMaxFlow();

    for (auto pair : pairs) {
        std::cout << pair.first->getName() << ' ' << pair.second->getName() << std::endl;
    }
}

void Menu::submenu3() {
    std::list<std::list<std::string>> topk;
    int k;

    while (true) {
        std::cout << "Escolha o valor de k:\n";
        std::cin >> k;
        std::cin.ignore();
        if (k <= 0) {
            std::cout << "Valor inválido. Escolha outro valor.\n";
            continue;
        }

        break;
    }

    topk = metrics.topkMunicDist(k);

    std::cout << "\nTop-" << k << " Municípios: \n\n";
    for (auto &mun : topk.front()) {
        std::cout << mun << "\n";
    }

    std::cout << "\nTop-" << k << " Distritos: \n\n";
    for (auto &dist : topk.back()) {
        std::cout << dist << '\n';
    }

}

void Menu::submenu4() {
    std::string name;
    Vertex *v;

    while (true) {
        std::cout << "Escolha a estação:" << std::endl;
        getline(std::cin, name);
        v = railwayGraph->findVertex(name);

        if (v == nullptr) {
            std::cout << "Estação inválida" << std::endl;
            continue;
        }

        break;
    }

    std::cout << metrics.maxTrainArriving(v) << '\n';
}

void Menu::submenu5() {
    std::string nameA, nameB;
    Vertex *s, *t;
    std::pair<int, int> maxFlowMinCost;

    while (true) {
        std::cout << "Escolha a estação de origem:" << std::endl;
        getline(std::cin, nameA);
        s = railwayGraph->findVertex(nameA);
        if (s == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        std::cout << "Escolha a estação de destino:" << std::endl;
        getline(std::cin, nameB);
        t = railwayGraph->findVertex(nameB);
        if (t == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        break;
    }

    maxFlowMinCost = metrics.maxFlowMinCost(s, t);
    if (maxFlowMinCost.first > 0) {
        std::cout << "\nO número máximo de trens será: ";
        std::cout << maxFlowMinCost.first << std::endl;
        std::cout << "O custo mínimo será: " << maxFlowMinCost.second << "€" << std::endl;
    } else {
        std::cout << "Não há fluxo máximo para as estações fornecidas \n";
    }
}

void Menu::submenu6() {
    std::string nameA, nameB;
    int maxFlow;
    Vertex *v1, *v2;
    std::vector<std::pair<Vertex*, Vertex *>> redConex;

    std::cout << "Primeiro, é necessário reduzir a conexão. Para isso escolha duas estações que estejam ligadas para serem desconectadas\n";
    while(option) {
        std::cout << "Escolha a primeira estação:" << std::endl;
        getline(std::cin, nameA);
        v1 = railwayGraph->findVertex(nameA);
        if (v1 == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        std::cout << "Escolha a segunda estação:" << std::endl;
        getline(std::cin, nameB);
        v2 = railwayGraph->findVertex(nameB);
        if (v2 == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        if (metrics.reduceAugmConectivity(v1, v2, true)) {
            redConex.push_back(std::make_pair(v1, v2));
            std::cout << "Conexão reduzida! Deseja reduzir mais? Selecione (0-1):\n";
            while (true) {
                std::cout << "1- Sim\n0- Não\n";
                std::cin >> option;
                std::cin.ignore();

                if (option != 0 && option != 1) {
                    std::cout << "Opção inválida. Tente novamente\n";
                    continue;
                }

                break;
            }
        } else {
            std::cout << "Conexão não pode ser reduzida. Tente novamente\n";
            continue;
        }
    }

    std::cout << "Agora com a conexão reduzida, escolha as duas estações que deseja saber o número máximo de trens\n";

    while (true) {
        std::cout << "Escolha a estação de origem:" << std::endl;
        getline(std::cin, nameA);
        v1 = railwayGraph->findVertex(nameA);
        if (v1 == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        std::cout << "Escolha a estação de destino:" << std::endl;
        getline(std::cin, nameB);
        v2 = railwayGraph->findVertex(nameB);
        if (v2 == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        maxFlow = metrics.findMaxFlow(v1, v2);
        std::cout << "\nO número máximo de trens será: ";
        std::cout << maxFlow << std::endl;

        break;
    }

    for (auto p : redConex) {
        metrics.reduceAugmConectivity(p.first, p.second, false);
    }
}

void Menu::submenu7() {
    int k;
    std::string nameA, nameB;
    Vertex *v1, *v2;
    std::vector<std::string> mostAffected;

    std::cout << "Primeiro, é necessário escolher o segmento que deseja considerar\n";
    while(true) {
        std::cout << "Escolha a primeira estação:" << std::endl;
        getline(std::cin, nameA);
        v1 = railwayGraph->findVertex(nameA);
        if (v1 == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        std::cout << "Escolha a segunda estação:" << std::endl;
        getline(std::cin, nameB);
        v2 = railwayGraph->findVertex(nameB);
        if (v2 == nullptr) {
            std::cout << "Estação inválida. Tente novamente" << "\n\n";
            continue;
        }

        if (metrics.reduceAugmConectivity(v1, v2, false)) {
            break;
        } else {
            std::cout << "Essas estações não formam um segmento. Tente novamente\n";
        }
    }

    std::cout << "Agora, escolha o valor de k:\n";
    std::cin >> k;
    std::cin.ignore();

    mostAffected = metrics.topkMostAffectStations(v1, v2, k);

    int n = mostAffected.size();
    if (n != k) {
        std::cout << "Não há " << k << " estações afetadas. Há um total de " << n << " estações mais afetadas\n";
    }

    if (n != 0) {
        std::cout << "\nAs estações mais afetadas são: \n";
        for (auto &s: mostAffected) {
            std::cout << s << '\n';
        }
    }
}
