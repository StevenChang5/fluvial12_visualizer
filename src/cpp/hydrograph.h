#ifndef HYDROGRAPH_H
#define HYDROGRAPH_H

#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

class Crosssection{
    public:
        inline Crosssection(int cs_id, std::string cs_name, int cs_num_coor){
            id = cs_id;
            name = cs_name;
            num_coor = cs_num_coor;
        }

        inline void add_coor(float station, float elevation, std::string time){
            std::tuple<float, float> coor_pair(station, elevation);
            coordinates[time].push_back(coor_pair);
            return;
        }

        inline const std::vector<std::tuple<float,float>> get_coor(std::string time){
            return coordinates[time];
        }

        inline const int get_num_coor(){
            return num_coor;
        }

    private:
        int id;
        std::string name;
        int num_coor;
        // Key: time, Value: vector of coordinates @ time
        std::unordered_map<std::string,std::vector<std::tuple<float, float>>> coordinates;
        
};

class HydrographFile{
    public:
        HydrographFile(std::string file_path);
        ~HydrographFile();
        
        // Key: crosssection ID, Value: crosssection object
        std::unordered_map<int, Crosssection*> sections;

    private:
        void set_peak(std::string peak_time);
        void set_end(std::string end_time);
        std::string end;
        std::string peak;
};

#endif