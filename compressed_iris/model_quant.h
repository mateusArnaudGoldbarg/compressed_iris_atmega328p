#include "math.h"

const uint8_t dense1_quant[4][10] = {{ 41, -10, -19, -39, -28, -34,  15, -56,  26,  21},
 { 50, -20,  41, -16,  24,  16, -39, -81,  19, -14},
 {-65, -38, -19,  20,  37,  29, -26, 107,   3, -19},
 {-35, -19, -20,  -9,  58, -43,   9, 127, -31, -39}};

const float q1 = 0.017235348;

const uint8_t bias1_quant[10] = {  40,   20,   20,   20,  -18,   11,   12, -127,   46,   22};

const float qb1 = 0.0063820635;

const uint8_t dense2_quant[10][10] = {{-42,  74, -41,  51,  38,  61, -43, 100,   9,  28},
 { -9, -34, -46,  33,  45,  -6, -43,  18, -37,  28},
 { -4, -44,  -7,  57, -24,  44,  20,  52,  43, -19},
 { -4,   5,  13,  43, -46, -15,  16,   0, -34, -18},
 { 58, -20,  30,   3,   3,   4, -18,  10,  53, -15},
 {-17, -42, -42,  -9, -46, -22,  28, -61,   0, -46},
 { 38, -13,  43,  28,  37, -24, -15,  16,  21,  -5},
 { 46, -37,  31, -46, -21, -34, -60, -96, 127, -18},
 { 75,  -3,  38, -16, -18, -35, -45,   9, -19, -45},
 {-22, -40,  16,  24, -39, -19,  31,  59,  40,  56}};

const float q2 = 0.010766979;
        
const uint8_t bias2_quant[10] = {70,   -2,   21,   36,   19,   29,   11,  100, -127,  -41};

const float qb2 = 0.0050510303;

const uint8_t dense3_quant[10][3] = {{ -84,   69,   46},
 {  56,  -29,  -25},
 { -92,  -21,  -23},
 {  79,   30,   14},
 {  40,  -49,  -16},
 {  38,   57,  -54},
 { -14,  -55,  -12},
 {  82,   -1, -127},
 { -50,  -43,  105},
 {  14,   -7,   39}};

const float q3 = 0.011761563;
        
const uint8_t bias3_quant[3] = {-40, 127, -60,};

const float qb3 = 0.011295059;


uint8_t predict_quant(float m1, float m2, float m3, float m4){
  uint8_t input[4] = {m1,m2,m3,m4};
  
  uint8_t n1[10] = {0,0,0,0,0,0,0,0,0,0};
  uint8_t n2[10] = {0,0,0,0,0,0,0,0,0,0};
  uint8_t n3[3] = {0,0,0};
  
  //dense1
  for(uint8_t i=0;i<4;i++){
    for(uint8_t j=0;j<10;j++){
      n1[j] += input[i]*dense1_quant[i][j];
    }
  }

  //bias1
  for(uint8_t i=0;i<10;i++){
    n1[i]+=bias1_quant[i];
  }

  //relu
  for(uint8_t i=0;i<10;i++){
          if(n1[i]<0){
                n1[i] = 0;
          }
      }
      
      //dense2
      for(uint8_t i=0;i<10;i++){
    for(uint8_t j=0;j<10;j++){
      n2[j] += n1[i]*dense2_quant[i][j];
    }
  }
  
  //bias2
  for(uint8_t i=0;i<10;i++){
    n2[i]+=bias2_quant[i];
  }
  
  //relu
  for(uint8_t i=0;i<10;i++){
          if(n2[i]<0){
                n2[i] = 0;
          }
      }
      
      //dense3
      for(uint8_t i=0;i<10;i++){
    for(uint8_t j=0;j<3;j++){
      n3[j] += n2[i]*dense3_quant[i][j];
    }
  }
  
  //bias3
  for(uint8_t i=0;i<3;i++){
    n3[i]+=bias3_quant[i];
  }
  
  //softmax
  double p1 = exp(n3[0])/(exp(n3[0])+exp(n3[1])+exp(n3[2]));
  double p2 = exp(n3[1])/(exp(n3[0])+exp(n3[1])+exp(n3[2]));
  double p3 = exp(n3[2])/(exp(n3[0])+exp(n3[1])+exp(n3[2]));
      
  if(p1>p2 && p1>p3)
    return 1;
  if(p2>p1 && p2>p3)
    return 2;
  else
    return 3;
      
  

}
