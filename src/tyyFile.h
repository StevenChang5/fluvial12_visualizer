#ifndef TYYFILE_H
#define TYYFILE_H

#include <string>
#include <tuple>
#include <vector>

class TYYFile{
    public:
        TYYFile(std::string file_path);
        void readFile(std::string file_path);
        const std::vector<std::tuple<float,float,float>> getCoordinates();
    private:
        std::vector<std::tuple<float, float, float>> coordinates;
};

#endif