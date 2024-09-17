#include <vector>
#include <iostream>
#include <algorithm> // std::sort
#include <getopt.h>
#include <string> 
#include <deque> 
#include <stack>


//IDENTIFIER  = 950181F63D0A883F183EC0A5CC67B19928FE896A


using namespace std;

//create ship class for storage
//get opt for ship.cpp


struct Location{
    char value;
    char direction;
    bool found = false;
};

struct Start{
    size_t level;
    size_t row;
    size_t column;
};

struct disc_location{
    size_t level;
    size_t row;
    size_t column;
};


struct Hangar{
    size_t level;
    size_t row;
    size_t column;
};

//
class Ship {
    public:
    //constructor:
    Ship(size_t ship_size, size_t ship_levels) : size(ship_size), levels(ship_levels) {}
    

    //store level and size
    void store_level(size_t value);
    void store_size(size_t value);
    void store_start(size_t level, size_t row, size_t column, size_t datum);

    //function to search for hangar using a queue
    void find_hangar(std::vector<std::vector<std::vector<Location>>> &station_map);

    //function to search for hangar using a stack
    //void find_hangar_s(std::vector<std::vector<std::vector<Location>>> &station_map);
    


    //member functions
    void get_options(int argc, char** argv);

    //function that stores map details and variables into a 3D vector
    void create_map_m(std::vector<std::vector<std::vector<Location>>> &station_map);

    //function that stores list variables into a 3D vector
    void create_map_l(std::vector<std::vector<std::vector<Location>>> &station_map);


    //functions to output while debugging I will just use terminal
    void output_map(std::vector<std::vector<std::vector<Location>>> &station_map);

    void output_list(std::vector<std::vector<std::vector<Location>>> &station_map);


    //backtrack functions that changes the variable to the correct direction as needed
    void backtrack(std::vector<std::vector<std::vector<Location>>> &station_map);

    //public variables:
    char policy = '\0';
    char output_choice = 'M';
    Start start_location;
    Hangar the_hangar;
    disc_location robot;

    Location hangar;
    bool hangar_found = false;
    deque<disc_location> the_deque;


    private:

    //private variables:
    size_t size;
    size_t levels;
    bool output = false;



};



//functions:

//create the deque


