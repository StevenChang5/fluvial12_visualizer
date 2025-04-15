#ifndef TY_H
#define TY_H

#include <string>
#include <tuple>
#include <vector>

class TYFile{
    public:
        TYFile(std::string file_path);
        void readFile(std::string file_path);
        const std::vector<std::tuple<float,float,float>> getCoordinates();
    private:
        std::vector<std::tuple<float, float, float>> coordinates;
};

#endif