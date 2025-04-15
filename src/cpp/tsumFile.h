#ifndef TSUMFILE_H
#define TSUMFILE_H

#include <string>
#include <tuple>
#include <vector>

class TSUMFile{
    public:
        TSUMFile(std::string file_path);
        void readFile(std::string file_path);
        const std::vector<std::tuple<float,float,float,float,float>> getCoordinates();

    private:
        std::vector<std::tuple<float,float,float,float,float>> coordinates;
};

#endif