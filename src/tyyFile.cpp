#include "tyyFile.h"
#include "utils.h"

#include <fstream>

using namespace std;

TYYFile::TYYFile(string file_path){
    readFile(file_path);
}

void TYYFile::readFile(string file_path){
    ifstream ty_ifstream(file_path);
    string line;
    while(getline(ty_ifstream, line)){
        vector<string> split = split_string(line);
        if(split.size() == 0) continue;
        else{
            tuple<float, float, float> coor(stof(split[0]),stof(split[1]),stof(split[2]));
            coordinates.push_back(coor);
        }
    }
}

const vector<tuple<float,float,float>> TYYFile::getCoordinates(){
    return coordinates;
}