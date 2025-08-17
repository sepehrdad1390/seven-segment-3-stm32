
#include "main.h"
#include "stdbool.h"


#define a_port A_GPIO_Port
#define a_pin  A_Pin

#define b_port B_GPIO_Port
#define b_pin  B_Pin

#define c_port C_GPIO_Port
#define c_pin  C_Pin

#define d_port D_GPIO_Port
#define d_pin  D_Pin

#define e_port E_GPIO_Port
#define e_pin  E_Pin

#define f_port F_GPIO_Port
#define f_pin  F_Pin

#define g_port G_GPIO_Port
#define g_pin  G_Pin

#define dp_port DP_GPIO_Port
#define dp_pin  DP_Pin


#define com1_port COM1_GPIO_Port
#define com1_pin  COM1_Pin

#define com2_port COM2_GPIO_Port
#define com2_pin  COM2_Pin

#define com3_port COM3_GPIO_Port
#define com3_pin  COM3_Pin

// _____________________________

#define com_Anode 1
#define com_Cathode 0

#define display_com com_Anode
// ______________________________

void sevenSeg_update_Digit();
void sevenSeg_update_Digit_timerIRQ();
void sevenSeg_write_number(float number);
void sevenSeg_write_diffrent(unsigned char digit3,unsigned char digit2,unsigned char digit1);
void sevenSeg_clear();
