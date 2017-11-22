#include "point.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>

 std::vector <struct point> ReadFile(const char * str, int * nfacilities, int * range){
    
    std::ifstream file;
    file.open(str, std::ios::in);

    std::vector <struct point> aux;
    int x,y;
    struct point a;

    file >> x >> y >> *nfacilities >> *range;

    while(file.good()){
        file >> a.x >> a.y;
        a.visited = false;

        aux.push_back(a);

    }

    aux.erase(aux.end()-1);
    return aux;
 }

 void GenerateSolution(std::vector <struct point> clients, int facilities, int range, std::vector <int> * solution){
     
    std::srand(std::time(0));

    for(int i = 0; i < facilities; i++){

        int fac = (std::rand()%(clients.size()-1));//Pega um ponto aleatório para ser a nova facilitadora

        while(clients[fac].visited){//Pega um elemento que não foi visitado (Não é facilitadora nem foi ligado a uma)

            fac = (std::rand()%(clients.size()-1));
        }
        
        (solution[i]).push_back(fac);//O primeiro elemento dos vectors será sempre a própria facilitadora
        clients[fac].visited = true;

        //Percorre todos os outros pontos e adiciona eles ao conjuntos desta facilitadora
        //se eles estiverem dentro do raio de cobertura dela
        //e se eles ainda não tiverem sido visitados por outra facilitadora
        for(int j = 0; j < clients.size(); j++){
            if(!(clients[j].visited)){
            //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                float dist = sqrt(((clients[fac].x - clients[j].x)*(clients[fac].x - clients[j].x)) + ((clients[fac].y - clients[j].y)*(clients[fac].y - clients[j].y)));
            
                if(dist <= range){//Se o ponto está na área de cobertura da facilitadora

                    (solution[i]).push_back(j);
                    clients[j].visited = true;
                }
            }
        }
    }


 }

int main(){

    int nfacilities, range;
    std::vector <struct point> clients;
    clients = ReadFile("coord324.txt", &nfacilities, &range);
     
    std::cout << "\n\nNfac: " << nfacilities << "\nRange: " << range;

    // for(int i = 0; i < clients.size();i++){

     //   std::cout << "\n(" << clients[i].x << ", " << clients[i].y << ")";
    // }

    std::vector <int> solution [nfacilities];

    GenerateSolution(clients, nfacilities, range, solution);
    
    std::cout << "\n\nSolução obtida: \n";

    int acc = 0;

    for(int i = 0; i < nfacilities; i++){

        std::cout << "\nFac ponto " << solution[i][0] << ": (" <<clients[ ((solution[i])[0]) ].x << ", " << clients[ ((solution[i])[0]) ].y << ")  || Pontos: ";

        for(int j = 1; j < solution[i].size(); j++){
            std::cout<< ((solution[i])[j]) << ", ";
        }
        acc += (solution[i]).size()-1;
    }

    std::cout << "\n\nTotal obtido: "<< acc <<" pontos alcançados :)\n";

    return 0;
}