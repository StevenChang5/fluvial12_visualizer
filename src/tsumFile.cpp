#include "tsumFile.h"
#include "utils.h"

#include <fstream>

using namespace std;

TSUMFile::TSUMFile(string file_path){
    readFile(file_path);
}

void TSUMFile::readFile(std::string file_path){
    ifstream ts_ifstream(file_path);
    string line;
    while(getline(ts_ifstream, line)){
        vector<string> split = split_string(line);
        if(split.size() == 0) continue;
        else{
            tuple<float,float,float,float,float> coor(
                stof(split[0]),
                stof(split[1]),
                stof(split[2]),
                stof(split[3]),
                stof(split[4])
            );
            coordinates.push_back(coor);
        }
    }
}

const vector<tuple<float,float,float,float,float>> TSUMFile::getCoordinates(){
    return coordinates;
}