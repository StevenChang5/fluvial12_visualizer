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

        inline void addSyd(std::string time, float syd_point){
            syd[time] = syd_point;
            return;
        }

        inline float getSyd(std::string time){
            return syd[time];
        }

        inline void addWsElev(std::string time, float ws_elev_point){
            ws_elev[time] = ws_elev_point;
        }
        
        inline const float getWsElev(std::string time){
            return ws_elev[time];
        }
        
        inline void addScour(float station, float elevation){
            std::tuple<float,float> coor_pair(station,elevation);
            scour.push_back(coor_pair);
        }

        inline const std::vector<std::tuple<float,float>> get_coor(std::string time){
            return coordinates[time];
        }

        inline const int get_num_coor(){
            return num_coor;
        }

        inline const int get_id(){
            return id;
        }

        inline const std::string get_name(){
            return name;
        }

    private:
        int id;
        std::string name;
        int num_coor;
        // Key: time, Value: vector of coordinates @ time
        std::unordered_map<std::string,std::vector<std::tuple<float, float>>> coordinates;
        std::unordered_map<std::string,float> syd;
        std::unordered_map<std::string,float> ws_elev;
        std::vector<std::tuple<float, float>> scour;
};

class HydrographFile{
    public:
        HydrographFile(std::string file_path);
        ~HydrographFile();

        void uploadScourFile(std::string file_path);

        // Key: crosssection ID, Value: crosssection object
        std::unordered_map<int, Crosssection*> sections;
        std::string get_approx_peak();
        std::string get_approx_end();

    private:
        /* 
            NOTE: 'peak' is the value recorded on line G1, 
                'approx_peak' is the closest time in the ID sections
                of the hydrograph file.
                Same for 'end' and 'approx_end.'
        */ 
        std::string peak;
        std::string approx_peak; 
        std::string end;
        std::string approx_end;
        std::string closest_to_peak(const std::string& time1, const std::string& time2);
        std::string closest_to_end(const std::string& time1, const std::string& time2);

        float exponent_to_float(const std::string& exp);
};

#endif