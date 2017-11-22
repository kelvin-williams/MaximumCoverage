#include "point.hpp"
#include <fstream>

 std::vector <struct point> ReadFile(const char * str, int * nfacilities, int * range){
    
    std::ifstream file;
    file.open(str, std::ios::in);

    std::vector <struct point> aux;
    int x,y;
    struct point a;

    file >> x >> y >> *nfacilities >> *range;

    while(file.good()){
        file >> a.x >> a.y;

        aux.push_back(a);

    }

    aux.erase(aux.end()-1);
    return aux;
 }

 int main(){

    int nfacilities, range;
    std::vector <struct point> clients;
    clients = ReadFile("coord324.txt", &nfacilities, &range);

    std::cout << "\n\nNfac: " << nfacilities << "\nRange: " << range << "\n\nCoords: ";

    for(int i = 0; i < clients.size();i++){

        std::cout << "\n(" << clients[i].x << ", " << clients[i].y << ")";
    }
     return 0;
 }