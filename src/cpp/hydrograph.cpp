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
    
    while(getline(hg_ifstream, line)){
        lines.push_back(line);
    }
    hg_ifstream.close();

    for(int row = 0; row < lines.size(); row++){
        vector<string> split = split_string(lines[row]);

        if(split.size() == 0) continue;
        else if(split[0] == "G1"){
            // Get Peak and End values from the G1 header
            set_peak(split[8]);
            set_end(split[9]);
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
        }else if(split[0] == "ID"){
            int subrow = row + 1;
            vector<string> temp = split_string(lines[subrow]);
            int temp_id = stoi(temp[0]);
            string temp_time = temp[5];
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
        // qDebug() << "END OF LINE\n";
    }
    
}

HydrographFile::~HydrographFile(){
    for(auto& cs: sections){
        delete cs.second;
    }
}


void HydrographFile::set_peak(string peak_time){
    peak = peak_time;
}

void HydrographFile::set_end(string end_time){
    end = end_time;
}