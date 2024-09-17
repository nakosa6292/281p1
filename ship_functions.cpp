#include <vector>
#include <iostream>
#include <algorithm> // std::sort
#include <getopt.h>
#include <string> 
#include <deque> 
#include <stack>
#include "ship_functions.h"

//IDENTIFIER  = 950181F63D0A883F183EC0A5CC67B19928FE896A




void Ship::store_level(size_t value){
    levels = value;
}

void Ship::store_size(size_t value){
    size = value;
}



//reads in file from a map into a 3D vector
//need to make sure I check for invalid rows and columns
void Ship::create_map_m(std::vector<std::vector<std::vector<Location>>> &station_map){
    char spot;
    string comment;

    //this function starts at the top of the file 
    //and reads in every part of the map to the 3D vector
    for(size_t i = 0; i < levels; i++){
            for(size_t j = 0; j < size; j++){
                for(size_t k = 0; k < size; k++){
                    cin >> spot;
                    
                    //skip the comments
                    while(spot == '/'){
                    getline(cin, comment);
                    cin >> spot;
                    }
                    if((spot != '.') && (spot != 'H') && (spot != '#') && (spot != 'E') && (spot != 'S')){
                        cout << "Invalid map character\n";
                        exit(1);
                    }
                    station_map[i][j][k].value = spot;

                    //store starting spot
                    if(spot == 'S'){
                        start_location.level = i;
                        start_location.row = j;
                        start_location.column = k;
                    }
                }//k loop
        }//j loop
    }//i loop
}



/*
office hours questions:
will the push_back on the elevators work how I want them too??
does the order that we search through floors for elevators matter?
*/

void Ship::create_map_l(std::vector<std::vector<std::vector<Location>>> &station_map){
    char spot;
    size_t x;
    size_t y;
    size_t z;
    char wasted_char;
    string comment;

    // this function goes through every single item in the list and stores 
    // it in the correct spot in the 3D vector with the correct char
    size_t spots = levels * size * size;
        for(size_t i = 0; i < spots; i++){
            cin >> spot;
            while(spot == '/'){
                    getline(cin, comment);
                    cin >> spot;
            }

            cin >> x >> wasted_char >> y >> wasted_char >> z >> wasted_char >> spot >> wasted_char;
            //check for invalid map level and columns
            if((x >= levels)){
                cout << "Invalid map level\n";
                exit(1);
            }
            if((y >= size)){
                cout << "Invalid map row\n";
                exit(1);
            }
            if((z >= size)){
                cout << "Invalid map column\n";
                exit(1);
            }
            if((spot != '.') && (spot != 'H') && (spot != '#') && (spot != 'E') && (spot != 'S')){
                cout << "Invalid map character\n";
                exit(1);
            }
            station_map[x][y][z].value = spot;

            //store starting spot
            if(spot == 'S'){
                start_location.level = x;
                start_location.row = y;
                start_location.column = z;
            }
        }//i loop
}



void Ship::output_map(std::vector<std::vector<std::vector<Location>>> &station_map){
    cout << "Start in level " << start_location.level << ", row " << start_location.row << ", column " << start_location.column << '\n';
    for(size_t i = 0; i < levels; i++){
        cout << "//level " << i << "\n";
        for(size_t j = 0; j < size; j++){
            for(size_t k = 0; k < size; k++){
                    cout << station_map[i][j][k].value;
            }//k loop
            cout << "\n";
        }//j loop
    }//i loop
}

void Ship::output_list(std::vector<std::vector<std::vector<Location>>> &station_map){
    cout << "//path taken\n";
    size_t l = start_location.level;
    size_t r = start_location.row;
    size_t c = start_location.column;
    Location temp = station_map[l][r][c];
        while(temp.value != 'H'){
                cout << '(' << l << "," << r << ','  << c << ',' << temp.value << ')' << '\n';
                if(temp.value == 'n'){
                    temp = station_map[l][r - 1][c];
                    r = r - 1;
                }
                else if(temp.value == 'e'){
                    temp = station_map[l][r][c + 1];
                    c = c + 1;
                }
                else if(temp.value == 's'){
                    temp = station_map[l][r + 1][c];
                    r = r + 1;
                }
                else if(temp.value == 'w'){
                    temp = station_map[l][r][c - 1];
                    c = c - 1;
                }
                else{
                l =  static_cast<size_t>(station_map[l][r][c].value - '0');
                temp = station_map[l][r][c];
                }
        }
}


//function to search for hangar using a queue
    void Ship::find_hangar(std::vector<std::vector<std::vector<Location>>> &station_map){
        size_t l = start_location.level;
        size_t r = start_location.row;
        size_t c = start_location.column;
        //Location current_loc = station_map[l][r][c];
        disc_location look;
        robot.level = l;
        robot.row = r;
        robot.column = c;
        station_map[l][r][c].found = true;
        the_deque.push_back(robot);
        while(!(the_deque.empty())){
            if(policy == 'q'){
                robot = the_deque[0];
                the_deque.pop_front();  
            }
            else{
                robot = the_deque.back();
                the_deque.pop_back();
            }

            l = robot.level;
            r = robot.row;
            c = robot.column;
            //look north
            if(r != 0 && (station_map[l][r - 1][c].value != '#') && (station_map[l][r - 1][c].found == false)){
                station_map[l][r - 1][c].direction = 'n';
                if(station_map[l][r - 1][c].value == 'H'){
                    hangar_found = true;
                    the_hangar.level = l;
                    the_hangar.row = r - 1;
                    the_hangar.column = c;
                    break;
                }
                else{
                    station_map[l][r - 1][c].found = true;
                    station_map[l][r - 1][c].direction = 'n';
                    look.level = l;
                    look.row = r - 1;
                    look.column = c;
                    the_deque.push_back(look);
                }
            }

            //look east
            if(c != (size - 1) && (station_map[l][r][c + 1].value != '#') && (station_map[l][r][c + 1].found == false)){
                station_map[l][r][c + 1].direction = 'e';
                if(station_map[l][r][c + 1].value == 'H'){
                    hangar_found = true;
                    the_hangar.level = l;
                    the_hangar.row = r;
                    the_hangar.column = c + 1;
                    break;
                }
                else{
                    station_map[l][r][c + 1].found = true;
                    station_map[l][r][c + 1].direction = 'e';
                    look.level = l;
                    look.row = r;
                    look.column = c + 1;
                    the_deque.push_back(look);
                }
            }

            //look south
            if(r != (size - 1) && (station_map[l][r + 1][c].value != '#') && (station_map[l][r + 1][c].found == false)){
                station_map[l][r + 1][c].direction = 's';
                if(station_map[l][r + 1][c].value == 'H'){
                    hangar_found = true;
                    the_hangar.level = l;
                    the_hangar.row = r + 1;
                    the_hangar.column = c;
                    break;
                }
                else{
                    station_map[l][r + 1][c].found = true;
                    station_map[l][r + 1][c].direction = 's';
                    look.level = l;
                    look.row = r + 1;
                    look.column = c;
                    the_deque.push_back(look);
                }
            }

            //look west
            if((c != 0) && (station_map[l][r][c - 1].value != '#') && (station_map[l][r][c - 1].found == false)){
                station_map[l][r][c - 1].direction = 'w';
                if(station_map[l][r][c - 1].value == 'H'){
                    hangar_found = true;
                    the_hangar.level = l;
                    the_hangar.row = r;
                    the_hangar.column = c - 1;
                    break;
                }
                else{
                    station_map[l][r][c - 1].found = true;
                    station_map[l][r][c - 1].direction = 'w';
                    look.level = l;
                    look.row = r;
                    look.column = c - 1;
                    the_deque.push_back(look);
                }
            }
            if(station_map[l][r][c].value == 'E'){
                for(size_t i = 0; i < levels; i++){
                    if(i == l){
                        ;
                    }
                    else{
                        if(station_map[i][r][c].value == 'E' && (station_map[i][r][c].found == false)){
                            station_map[i][r][c].found = true;
                            disc_location elevator;
                            elevator.level = i;
                            elevator.row = r;
                            elevator.column = c;
                            station_map[i][r][c].direction = static_cast<char>(l + '0');
                            the_deque.push_back(elevator);
                        }//if
                    }//else
                }//i loop
            }//if


        }


    }


    void Ship::backtrack(std::vector<std::vector<std::vector<Location>>> &station_map){
        size_t l = the_hangar.level;
        size_t r = the_hangar.row;
        size_t c = the_hangar.column;
        //char hdirection = station_map[l][r][c].direction;
        char tempvar = station_map[l][r][c].direction;

        while((start_location.level != l) || (start_location.row != r) || (start_location.column != c)){
            if(tempvar == 'n'){
                tempvar = station_map[l][r + 1][c].direction;
                station_map[l][r + 1][c].value = 'n';
                r = r + 1;
            }
            else if(tempvar == 'e'){
                tempvar = station_map[l][r][c - 1].direction;
                station_map[l][r][c - 1].value = 'e';
                c = c - 1;
            }
            else if(tempvar == 's'){
                tempvar = station_map[l][r - 1][c].direction;
                station_map[l][r - 1][c].value = 's';
                r = r - 1;
            }
            else if(tempvar == 'w'){
                tempvar = station_map[l][r][c + 1].direction;
                station_map[l][r][c + 1].value = 'w';
                c = c + 1;
            }
            else{
                station_map[static_cast<size_t>(tempvar - '0')][r][c].value = static_cast<char>(l + '0');
                tempvar = station_map[static_cast<size_t>(tempvar - '0')][r][c].direction;
                l =  static_cast<size_t>(station_map[l][r][c].direction - '0');
            }
        }

    }
    





//get opt
void Ship::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    bool select_s = false;
    bool select_q = false;
    struct option longOpts[] = {{ "stack", no_argument, nullptr, 's' },
                                { "queue", no_argument, nullptr, 'q' },
                                {"output", required_argument, nullptr, 'o'},
                                { "help", no_argument, nullptr, 'h' },
                                { nullptr, 0, nullptr, '\0' }};
    

    while ((option = getopt_long(argc, argv, "sqo:h", longOpts, &option_index)) != -1) {
        switch (option) {
            case 's':
                policy = 's';
                select_s = true;
                break;
       
            case 'q':
                policy = 'q';
                select_q = true;
                break;


            case 'o':
                output_choice = optarg[0];
                output = true;
                break; 


            case 'h':
                std::cout << "Invalid input arguments, please use -s or -q and if you want -s or -m after\n";
                exit(0);
        }//switch
    } //while


    if(policy == '\0'){
       std::cout << "Invalid output mode specified\n";
        exit(1); 
    }
    if(select_s && select_q){
        cout << "Multiple routing modes specified\n";
        exit(1);
    }
    if(output_choice != 'M' && output_choice != 'L'){
        cout << "Invalid output mode specified\n";
        exit(1);
    }
}