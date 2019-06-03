#ifndef __CMSTRUCT_H__
#define __CMSTRUCT_H__


typedef struct {
        int ratio1;
        int ratio2;
        int ratio3;
        int ratio4;
        int ratio5;
        int ratio6;
        int ratio7;
        int ratio8;
        int ratio9;
        int ratio10;
        int ratio11;
        int ratio12;
        int ratio1_cp;
        int ratio2_cp;
        int ratio3_cp;
        // int ratio4_cp;
        // int ratio5_cp;
        // int ratio6_cp;
        // int ratio5_cp;
        // int ratio6_cp;
        // int ratio7_cp;
        // int ratio8_cp;
        // int ratio9_cp;
        // int ratio10_cp;
        // int ratio11_cp;
        // int ratio12_cp;
        int brightness;
}ratio_val;

ratio_val ratio_123;

typedef struct ph_str_val
{
    float _temp_c;
    float _phValue;
    float _acidVoltage;    //buffer solution 4.0 at 25C
    float _neutralVoltage; //buffer solution 7.0 at 25C
    float _voltage;
} ph_str_val;

ph_str_val ph_val;



#endif
