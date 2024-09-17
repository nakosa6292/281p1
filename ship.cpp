#include <vector>
#include <string>
#include <iostream>
#include <algorithm> // std::sort
#include <getopt.h>
#include <string>    // needed for VS
#include <cctype>
#include "ship_functions.h"

//IDENTIFIER  = 950181F63D0A883F183EC0A5CC67B19928FE896A


using namespace std;



//get opt for ship.cpp


int main(int argc, char** argv){

    std::ios::sync_with_stdio(false);

    Ship station(0,0);
    station.get_options(argc, argv);
    //first line of file is input mode
    char input_mode;
    cin >> input_mode;
    if(input_mode != 'L' && input_mode != 'M'){
        cout << "Invalid map input mode\n";
        exit(1);
    }
    //second line is number of levels
    size_t levels;
     if (!(cin >> levels)) {  // Check if input is valid for levels
        cout << "Invalid map level\n";
        cout << levels;
        exit(1);
    }

    //size of levels nxn
    size_t grid_size;
    cin >> grid_size;


    if((levels > 10) || (levels < 1)){
        cout << "Invalid map level\n";
        exit(1);
    }

    Location dot;
    dot.value = '.';

    vector<vector<vector<Location>>> station_map(levels, vector<vector<Location>>(grid_size, vector<Location>(grid_size, dot)));

    station.store_level(levels);
    station.store_size(grid_size);
    
    if(input_mode == 'M'){
        station.create_map_m(station_map);
    }
    if(input_mode == 'L'){
       station.create_map_l(station_map); 
    }


    station.find_hangar(station_map);


    //if hangar is found then change map to put direction in correct tiles
    if(station.hangar_found == true){
        station.backtrack(station_map);
    }

    if(station.output_choice == 'M'){
        station.output_map(station_map);
    }
    if(station.output_choice == 'L'){
        if(station.hangar_found == false){
            cout << "//path taken\n";
        }
        else{
            station.output_list(station_map);
        }
    }
    return 0;
    
}




/*
arguments:
stack -s
queue -q
output -o (argument: M or L)
help -h
*/

//get opt