#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <typeinfo>
#include <cstdlib> 
#include <ctime> 
#include <algorithm>
#include <cmath>
#define JOB_NUMBER 100
#define MACHINE_NUMBER 10
#define MAX_LINE 2056
#define CHOROMOSOME_NUMBER 100
#define MATING_RATE 0.8
#define MUTATING_RATE 0.2
#define ITERATE_NUMBER 50
#define MUTATING_RATE_FOR_ONECHO 0.01
#define WIP_FILE "WIP.csv"
#define EQP_RECIPE_FILE "EQP_RECIPE.csv"
#define Tool_FILE "Tool.csv"
#define set_up_time_FILE "set_up_time.csv"
using namespace std;
class Job
{
     public:
        string job_name;
        vector<int>machine_can_select;
        int machine_selected;
        float arrival_time;
        string recipe;
        float qty;
        float select;
        float sort;
        float process_time;
};

class Machine
{
    public:
        vector<Job>job_in_machine;
        vector<int>set_up;
        vector<int>arrive;
        float makespan;
};

class Choromosome
{
    public:
        vector<float>value;
        vector<Machine>machine;
        float makespan;
        float roulette;
};
class EQP_RECIPE
{
    public:
        int EQP_ID;
        string RECIPE;
        float process_time;
};


