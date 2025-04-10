#include "hydrograph.h"
#include "utils.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <QDebug>

using namespace std;

HydrographFile::HydrographFile(string file_path){
    ifstream hg_ifstream(file_path);
    vector<string> lines;
    string line;
    int id = 1;
    approx_peak = "";
    approx_end = "";
    
    while(getline(hg_ifstream, line)){
        lines.push_back(line);
    }
    hg_ifstream.close();

    for(int row = 0; row < lines.size(); row++){
        vector<string> split = split_string(lines[row]);
        if(split.size() == 0) continue;
        else if(split[0] == "G1"){
            // Get Peak and End values from the G1 header
            peak = split[8];
            end = split[9];
        }else if(split[0] == "X1"){
            // Collect crosssection metadata at initial time=0
            Crosssection* cs = new Crosssection(id, split[1], std::stoi(split_string(split[2],'.')[0]));
            int subrow = row + 1;
            int num_coor = cs->get_num_coor();
            while(split_string(lines[subrow])[0] != "GR") subrow++;
            for(int i = 0; i < ceil(num_coor/5.0); i++){
                vector<string> temp_coor = split_string(lines[subrow]);
                for(int j = 1; j < temp_coor.size(); j += 2){
                    cs->add_coor(stof(temp_coor[j+1]),stof(temp_coor[j]),"0");
                }
                subrow++;
            }
            sections[id] = cs;
            id++;
        }else if(split[0] == "ID\r"){
            int subrow = row + 1;
            vector<string> temp = split_string(lines[subrow]);
            int temp_id = stoi(temp[0]);
            string temp_time = temp[5];
            if(approx_peak == ""){
                approx_peak = temp_time;
            }
            else{
                approx_peak = closest_to_peak(temp_time, approx_peak);
            }
            if(approx_end == ""){
                approx_end = temp_time;
            }
            else{
                approx_end = closest_to_end(temp_time, approx_end);
            }
            Crosssection* temp_cs = sections[temp_id];
            subrow += 4;
            int num_coor = temp_cs->get_num_coor();
            for(int i = 0; i < ceil(num_coor/3.0); i++){
                vector<string> temp_coor = split_string(lines[subrow]);
                for(int j = 0; j < temp_coor.size(); j+=4){
                    temp_cs->add_coor(stof(temp_coor[j+3]),stof(temp_coor[j]),temp_time);
                }
                subrow++;
            }
        }
    }
}

HydrographFile::~HydrographFile(){
    for(auto& cs: sections){
        delete cs.second;
    }
}

std::string HydrographFile::closest_to_peak(const std::string& time1, const std::string& time2){
    float t1 = stof(time1);
    float t2 = stof(time2);
    float pk = stof(peak);
    if(abs(pk-t1) < abs(pk-t2)){
        return time1;
    }else{
        return time2;
    }
}

std::string HydrographFile::closest_to_end(const std::string& time1, const std::string& time2){
    float t1 = stof(time1);
    float t2 = stof(time2);
    float ed = stof(end);
    if(abs(ed-t1) < abs(ed-t2)){
        return time1;
    }else{
        return time2;
    }
}

std::string HydrographFile::get_approx_peak(){
    return approx_peak;
}

std::string HydrographFile::get_approx_end(){
    return approx_end;
}