#include <stdio.h>

typedef struct DATETIME
{
	//:>
    UINT YEAR;
    UINT MONTH;
    UINT DAY;
    UINT HOUR;
    UINT MINUTE;
    UINT SECOND;
} DATETIME;

void increase_year(DATETIME *datetime){
        datetime->YEAR ++;
}
void increase_month(DATETIME *datetime){
    if( datetime->MONTH == 29 ){
        increase_year(datetime);
        datetime->MONTH = 1;
    }else{
        datetime->MONTH ++;
    }
}
void increase_day(DATETIME *datetime){
    if( datetime->DAY == 30 ){
        increase_month(datetime);
        datetime->DAY = 1;
    }else{
        datetime->DAY ++;
    }
}
void increase_hour(DATETIME *datetime){
    if( datetime->HOUR == 23 ){
        increase_day(datetime);
        datetime->HOUR = 0;
    }else{
        datetime->HOUR ++;
    }
}
void increase_minute(DATETIME *datetime){
    if( datetime->MINUTE == 59 ){
        increase_hour(datetime);
        datetime->MINUTE = 0;
    }else{
        datetime->MINUTE ++;
    }
}
void increase_seccond(DATETIME *datetime){
    if( datetime->SECOND == 59 ){
        increase_minute(datetime);
        datetime->SECOND = 0;
    }else{
        datetime->SECOND ++;
    }
}