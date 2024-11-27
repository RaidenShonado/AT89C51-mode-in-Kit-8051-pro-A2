#ifndef _BASE_LIB_H_
#define _BASE_LIB_H_

#ifndef elif
#define elif else if
#endif
#ifndef DECREASE_ONE
#define DECREASE_ONE(VAR) VAR = (VAR>0?(VAR-1):VAR)
#endif
#ifndef REP
#define REP(i, a, b) for(i = (a); i <= (b); ++i)
#endif
#ifndef REV
#define REV(i, a, b) for(i = (a); i >= (b); --i)
#endif


typedef unsigned char uint8;
typedef unsigned int uint32;
typedef char int8;
typedef int int32;

static void delay_us(uint32 us){
    uint32 i = 0; 
    for(i = 0; i < us; i = i + 1){
        // do nothin'
    }
}

void delay_ms(uint32 ms){
    uint32 i = 0; 
    for(i = 0; i < ms*12; i = i + 1){
        // do nothin'
    }
}

enum enum_STATE{ LOW  = 0, HIGH = 1 };



#endif