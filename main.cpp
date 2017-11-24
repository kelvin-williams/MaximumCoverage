#include "point.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iterator>

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


 void GenerateSolution(std::vector <struct point> & clients, int facilities, int range, std::vector <int> * solution){
     
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

 void NeighbourMove(std::vector <struct point> & clients, int facilities, int range, std::vector <int> * solution){

     int leastclients = INT_MAX;
     int leastclientsindex;

     //Primeiro procura a facilitadora que possui menos pontos cobertos
     for(int i = 0; i < facilities; i++){

         if(solution[i].size()-1 < leastclients){
             leastclients = solution[i].size()-1;
             leastclientsindex = i;
         }
     }

     //Subtitui pelo próximo ponto livre
     for(int i = ((solution[leastclientsindex])[0] + 1); i < clients.size(); i = (i+1)%(clients.size()-1)){

         if(!(clients[i].visited)){//Se o ponto ainda não foi incluido na solução
            
           //std::cout << "\nsize do vetor a ser retirado: " << solution[leastclientsindex].size();
            //Retira os pontos que estavam incluídos da solução
            for(int u = 0; u < solution[leastclientsindex].size(); u++){

                clients[ solution[leastclientsindex][u] ].visited = false;
            }

            solution[leastclientsindex].clear();

            (solution[leastclientsindex]).push_back(i);//O primeiro elemento dos vectors será sempre a própria facilitadora
            clients[i].visited = true;

            //Percorre todos os outros pontos e adiciona eles ao conjuntos desta facilitadora
            //se eles estiverem dentro do raio de cobertura dela
            //e se eles ainda não tiverem sido visitados por outra facilitadora
            for(int j = 0; j < clients.size(); j++){
                if(!(clients[j].visited)){
                //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                    float dist = sqrt(((clients[i].x - clients[j].x)*(clients[i].x - clients[j].x)) + ((clients[i].y - clients[j].y)*(clients[i].y - clients[j].y)));
            
                    if(dist <= range){//Se o ponto está na área de cobertura da facilitadora

                        (solution[leastclientsindex]).push_back(j);
                        clients[j].visited = true;
                    }
                }
            }

            break;
         }else{

             if(i == (solution[leastclientsindex])[0])
                break;
         }
     }
 }


void VND(std::vector <struct point> clients, int facilities, int range){

    //Primeiramente nós pegamos uma solução qualquer
    std::vector <int> solution [facilities];
    std::vector <struct point> s1clients = clients;
    int s1value = 0;

    GenerateSolution(s1clients, facilities, range, solution);

    //Calcula o "valor da solução" (número total de clientes atendidos)
    for(int i = 0; i < facilities; i++)
        s1value += solution[i].size() - 1;

    //
    for(int k = 0; k < 10; k++){
        std::vector <int> solution2 [facilities] = solution;
        std::vector <struct point> s2clients = s1clients;
        int s2value = 0;

        NeighbourMove(s2clients, facilities, range, solution2);

        //Calcula o valor da solução 2
        for(int i = 0; i < facilities; i++)
            s2value += solution2[i].size() - 1;


        if(s2value > s1value){//Se a nova solução for melhor
            std::cout << "\n" << s2value << " é maior que "<< s1value;
            //solution = solution2
            for(int m = 0; m < facilities; m++)
                solution[m] = solution2[m];

            s1clients = s1clients;
            s1value = s2value;
            
            k = 0;
        }
    }

    //Printando a melhor solução:
    std::cout << "\n\nMelhor solução obtida: \n";

    int acc = 0;

    for(int i = 0; i < facilities; i++){

        std::cout << "\nFac ponto " << solution[i][0] << ": (" <<s1clients[ ((solution[i])[0]) ].x << ", " << s1clients[ ((solution[i])[0]) ].y << ")  || Pontos: ";

        for(int j = 1; j < solution[i].size(); j++){
            std::cout<< ((solution[i])[j]) << ", ";
        }
        acc += (solution[i]).size()-1;
    }

    std::cout << "\n\nTotal obtido: "<< acc <<" pontos alcançados :)\n";
}

int main(){

    int nfacilities, range;
    std::vector <struct point> clients;
    clients = ReadFile("coord324.txt", &nfacilities, &range);
     
    std::cout << "\n\nNfac: " << nfacilities << "\nRange: " << range;

    
    VND(clients, nfacilities, range);
    
    // for(int i = 0; i < clients.size();i++){

     //   std::cout << "\n(" << clients[i].x << ", " << clients[i].y << ")";
    // }

    //std::vector <int> solution [nfacilities];

    //GenerateSolution(clients, nfacilities, range, solution);

   //Print de Solução
   /* 
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
    */

    return 0;
}