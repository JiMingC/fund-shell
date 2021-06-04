#include <stdlib.h>
#include "typedef.h"
#include "cJSON.h"
#include "myjson.h"
#include <string.h>

typedef struct fundInfo{
    char f_name[30];
    char f_code[30];
    float l_val;
    float c_val;
    float gain;
    char* time;
}fundInfo_s;

struct fundSet {
    int num;
    fundInfo_s fundInfo[30];
};
