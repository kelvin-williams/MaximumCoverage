#include "point.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <iterator>
#include <map>

#define ALPHA 0.5

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
        a.facility = false;

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

 void GenerateSolution2(std::vector <struct point> & clients, int facilities, int range, std::vector <int> * solution){


    for(int i = 0; i < facilities; i++){

        std::map <int, int> values;//Este mapa vai guardar os valores de cada ponto se ele fosse uma facilitadora

        for(int j = 0; j < clients.size(); j++){
        
            values[j] = 0;
        }


        for(int j = 0; j < clients.size(); j++){
        
            for(int k = 0; k < clients.size(); k++){
            
                if(!(clients[k].visited)){
                    
                    //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                    float dist = sqrt(((clients[j].x - clients[k].x)*(clients[j].x - clients[k].x)) + ((clients[j].y - clients[k].y)*(clients[j].y - clients[k].y)));
            
                    if(dist <= range){
                    
                        values[j]++;
                    }
                }
            }
        }
        
        int fac;
        int biggest = 0;

        //Procura o elemento com o maior valor do mapa
        for(int j = 0; j < clients.size(); j++){
        
            if(values[j] > biggest && clients[j].visited == false){
                
                biggest = values[j];
                fac = j;
            }
        }
        
        //Adiciona esse elemento à solução
        (solution[i]).push_back(fac);//O primeiro elemento dos vectors será sempre a própria facilitadora
        clients[fac].visited = true;
        clients[fac].facility = true;

        //Percorre todos os outros pontos e adiciona eles ao conjuntos desta facilitadora
        //se eles estiverem dentro do raio de cobertura dela
        //e se eles ainda não tiverem sido visitados por outra facilitadora
        for(int j = 0; j < clients.size(); j++){
            if(!(clients[j].visited)){
            //Calcula a distância: sqrt( (Xa - Xb)² + (Ya - Yb)²)
                float dist = sqrt(((clients[fac].x - clients[j].x)*(clients[fac].x - clients[j].x)) + ((clients[fac].y - clients[j].y)*(clients[fac].y - clients[j].y)));
            
                if(dist <= range){//Se o ponto está na área de cobertura da facilitadora

                    (solution[i]).push_back(j);
                    clients[j].visited = true;
                }
            }
        }
    }
 }

  void GenerateSolution2Random(std::vector <struct point> & clients, int facilities, int range, std::vector <int> * solution){

    std::srand(std::time(0));

    for(int i = 0; i < facilities; i++){

        std::map <int, int> values;//Este mapa vai guardar os valores de cada ponto se ele fosse uma facilitadora

        for(int j = 0; j < clients.size(); j++){
        
            values[j] = 0;
        }


        for(int j = 0; j < clients.size(); j++){
        
            for(int k = 0; k < clients.size(); k++){
            
                if(!(clients[k].visited)){
                    
                    //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                    float dist = sqrt(((clients[j].x - clients[k].x)*(clients[j].x - clients[k].x)) + ((clients[j].y - clients[k].y)*(clients[j].y - clients[k].y)));
            
                    if(dist <= range){
                    
                        values[j]++;
                    }
                }
            }
        }
        
        int big, small;
        int biggest = 0, smallest = INT_MAX;

        //Procura os elementos com o maior e o menor valor do mapa
        for(int j = 0; j < clients.size(); j++){
        
            if(values[j] > biggest && clients[j].visited == false){
                
                biggest = values[j];
                big = j;
            }

            if(values[j] < smallest && clients[j].visited == false){
                
                smallest = values[j];
                small = j;
            }
        }
        
        //Soma os dois e multiplica por ALPHA
        int separatorValue = (biggest + smallest) * ALPHA;

        //Escolhe um elemento aleatório que tenha um valor maior ou igual ao calculado
        int fac = (std::rand()%(clients.size()-1));

        while(values[fac] < separatorValue){

            fac = (std::rand()%(clients.size()-1));
        }
        
        //Depois de achar um elemento adiciona ele à solução
        (solution[i]).push_back(fac);//O primeiro elemento dos vectors será sempre a própria facilitadora
        clients[fac].visited = true;
        clients[fac].facility = true;

        //Percorre todos os outros pontos e adiciona eles ao conjuntos desta facilitadora
        //se eles estiverem dentro do raio de cobertura dela
        //e se eles ainda não tiverem sido visitados por outra facilitadora
        for(int j = 0; j < clients.size(); j++){
            if(!(clients[j].visited)){
            //Calcula a distância: sqrt( (Xa - Xb)² + (Ya - Yb)²)
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

void NeighbourMove2(std::vector <struct point> & clients, int facilities, int range, std::vector <int> * solution){

    for(int k = 0; k < facilities; k++){

        //Subtitui pelo próximo ponto livre
        for(int i = ((solution[k])[0] + 1); i < clients.size(); i = (i+1)%(clients.size()-1)){

            if(!(clients[i].visited)){//Se o ponto ainda não foi incluido na solução
                
            //std::cout << "\nsize do vetor a ser retirado: " << solution[k].size();
                //Retira os pontos que estavam incluídos da solução
                for(int u = 0; u < solution[k].size(); u++){

                    clients[ solution[k][u] ].visited = false;
                }

                solution[k].clear();

                (solution[k]).push_back(i);//O primeiro elemento dos vectors será sempre a própria facilitadora
                clients[i].visited = true;

                //Percorre todos os outros pontos e adiciona eles ao conjuntos desta facilitadora
                //se eles estiverem dentro do raio de cobertura dela
                //e se eles ainda não tiverem sido visitados por outra facilitadora
                for(int j = 0; j < clients.size(); j++){
                    if(!(clients[j].visited)){
                    //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                        float dist = sqrt(((clients[i].x - clients[j].x)*(clients[i].x - clients[j].x)) + ((clients[i].y - clients[j].y)*(clients[i].y - clients[j].y)));
                
                        if(dist <= range){//Se o ponto está na área de cobertura da facilitadora

                            (solution[k]).push_back(j);
                            clients[j].visited = true;
                        }
                    }
                }

                break;
            }else{

                if(i == (solution[k])[0])
                    break;
            }
        }
    }
}

void NeighbourMove2Heavy(std::vector <struct point> & clients, int facilities, int range, std::vector <int> * solution){

    for(int k = 0; k < facilities; k++){
        
        //Primeiro retira a facilitadora da solução
        clients[ solution[k][0] ].facility = false;

        for(int u = 0; u < solution[k].size(); u++){

            clients[ solution[k][u] ].visited = false;
        }

        solution[k].clear();


        //Agora calcula o valor de cada ponto que não é uma facilitadora, incluindo pontos já pegos
        std::map <int, int> values;

        for(int j = 0; j < clients.size(); j++){
        
            values[j] = 0;
        }


        for(int j = 0; j < clients.size(); j++){
            
            if(!(clients[j].facility)){

                for(int l = 0; l < clients.size(); l++){
            
                    if(!(clients[l].facility)){
                    
                        //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                        float dist = sqrt(((clients[j].x - clients[l].x)*(clients[j].x - clients[l].x)) + ((clients[j].y - clients[l].y)*(clients[j].y - clients[l].y)));
            
                        if(dist <= range){
                    
                            values[j]++;
                        }
                    }
                }
            }
        }
        
        int fac;
        int biggest = 0;

        //Procura o elemento com o maior valor do mapa
        for(int j = 0; j < clients.size(); j++){
        
            if(values[j] > biggest){
                
                biggest = values[j];
                fac = j;
            }
        }

        //Retira da solução atual caso o ponto já esteja incluído
        if(clients[fac].visited){

            for(int i = 0; i < facilities; i++){

                for(int m = 0; m < solution[i].size(); m++){

                    if(solution[i][m] == fac){

                        solution[i].erase(solution[i].begin() + m);
                        clients[fac].visited = false;
                        break;
                    }
                }
            }
        }


        //Inclui este elemento na solução

        (solution[k]).push_back(fac);//O primeiro elemento dos vectors será sempre a própria facilitadora
        clients[fac].visited = true;
        clients[fac].facility = true;

        //Percorre todos os outros pontos e adiciona eles ao conjuntos desta facilitadora
        //se eles estiverem dentro do raio de cobertura dela
        //e se eles ainda não tiverem sido visitados por outra facilitadora
        for(int j = 0; j < clients.size(); j++){
            if(!(clients[j].visited)){
            //Calcula a distância: sqrt( (Xa-Xb)² + (Ya - Yb)²)
                float dist = sqrt(((clients[fac].x - clients[j].x)*(clients[fac].x - clients[j].x)) + ((clients[fac].y - clients[j].y)*(clients[fac].y - clients[j].y)));
    
                if(dist <= range){//Se o ponto está na área de cobertura da facilitadora

                    (solution[k]).push_back(j);
                    clients[j].visited = true;
                }
            }
        }
    }
}



void VND(std::vector <struct point> clients, int facilities, int range){

    //Primeiramente nós pegamos uma solução qualquer
    std::vector <int> solution [facilities];
    std::vector <struct point> s1clients = clients;
    int s1value = 0;

    GenerateSolution2Random(s1clients, facilities, range, solution);

    //Calcula o "valor da solução" (número total de clientes atendidos)
    for(int i = 0; i < facilities; i++)
        s1value += solution[i].size() - 1;

    int k = 0;
    while(k < 2){
        std::vector <int> solution2 [facilities] = solution;
        std::vector <struct point> s2clients = s1clients;
        int s2value = 0;
        
        if(k == 0){
            NeighbourMove2(s2clients, facilities, range, solution2);
        }else{
            NeighbourMove2Heavy(s2clients, facilities, range, solution2);            
        }

        //Calcula o valor da solução 2
        for(int i = 0; i < facilities; i++)
            s2value += solution2[i].size() - 1;

        std::cout << "\nTentando trocar solução com valor " << s1value << " para uma com valor "<< s2value;
                
        if(s2value > s1value){//Se a nova solução for melhor
            std::cout << "\nTrocando solução com valor " << s1value << " para uma com valor "<< s2value;
            //solution = solution2
            for(int m = 0; m < facilities; m++)
                solution[m] = solution2[m];

            s1clients = s1clients;
            s1value = s2value;
            
            k = 0;
        }else{
            k++;
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

    std::cout << "\n\n"<< acc <<" Pontos alcançados\n";
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
