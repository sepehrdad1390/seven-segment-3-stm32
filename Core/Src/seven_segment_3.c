
#include "seven_segment_3.h"


static unsigned char segment_data[10] = {3,159,37,13,153,73,65,31,1,9};
static unsigned char dig[3];
static int timedelayseg = 0;
static char levelprogressseg = 0;

static void float_to_string(float number, char *buffer, uint8_t decimal_places)
{
    if (number < 0.0f)
    {
        *buffer++ = '-';
        number = -number;
    }
    uint32_t int_part = (uint32_t)number;
    float fraction = number - (float)int_part;
    char temp[10];
    int i = 0;
    do
    {
        temp[i++] = (int_part % 10) + '0';
        int_part /= 10;
    } while (int_part);
    while (i--)
        *buffer++ = temp[i];
    *buffer++ = '.'; 
    for (uint8_t j = 0; j < decimal_places; j++)
    {
        fraction *= 10.0f;
        uint8_t digit = (uint8_t)fraction;
        *buffer++ = digit + '0';
        fraction -= (float)digit;
    }
    *buffer = '\0'; 
}

static void pin_value(unsigned char value)
{
   HAL_GPIO_WritePin(dp_port,dp_pin,(value >> 0) & 0x01);
   HAL_GPIO_WritePin(g_port,g_pin,(value >> 1) & 0x01);
   HAL_GPIO_WritePin(f_port,f_pin,(value >> 2) & 0x01);
   HAL_GPIO_WritePin(e_port,e_pin,(value >> 3) & 0x01);
   HAL_GPIO_WritePin(d_port,d_pin,(value >> 4) & 0x01);
   HAL_GPIO_WritePin(c_port,c_pin,(value >> 5) & 0x01);
   HAL_GPIO_WritePin(b_port,b_pin,(value >> 6) & 0x01);
   HAL_GPIO_WritePin(a_port,a_pin,(value >> 7) & 0x01);
}

void sevenSeg_update_Digit()
{
   HAL_GPIO_WritePin(com1_port,com1_pin,display_com);
   pin_value(dig[2]);
   HAL_Delay(3);
   
   HAL_GPIO_WritePin(com1_port,com1_pin,!display_com);
   HAL_GPIO_WritePin(com2_port,com2_pin,display_com);
   pin_value(dig[1]);
   HAL_Delay(3);
    
   HAL_GPIO_WritePin(com2_port,com2_pin,!display_com);
   HAL_GPIO_WritePin(com3_port,com3_pin,display_com);
   pin_value(dig[0]);
   HAL_Delay(3);
   HAL_GPIO_WritePin(com3_port,com3_pin,!display_com);
}

void sevenSeg_update_Digit_timerIRQ()
{
     if(levelprogressseg == 0)
     {
        HAL_GPIO_WritePin(com1_port,com1_pin,display_com);
        pin_value(dig[2]);
        levelprogressseg = 1;
     }
     if(levelprogressseg == 1)
     {
       timedelayseg ++;
     }
     if(levelprogressseg == 1 && timedelayseg >= 3)
     {
        timedelayseg = 0;
         HAL_GPIO_WritePin(com1_port,com1_pin,!display_com);
         HAL_GPIO_WritePin(com2_port,com2_pin,display_com);
         pin_value(dig[1]);
         levelprogressseg = 2;
     }
     if(levelprogressseg == 2)
     {
       timedelayseg ++;
     }
     if(levelprogressseg == 2 && timedelayseg >= 3)
     {
        timedelayseg = 0;
         HAL_GPIO_WritePin(com2_port,com2_pin,!display_com);
         HAL_GPIO_WritePin(com3_port,com3_pin,display_com);
         pin_value(dig[0]);
         levelprogressseg = 3;
     }
      if(levelprogressseg == 3)
     {
       timedelayseg ++;
     }
     if(levelprogressseg == 3 && timedelayseg >= 3)
     {
        timedelayseg = 0;
        HAL_GPIO_WritePin(com3_port,com3_pin,!display_com);
        levelprogressseg = 0;
     }
}

void sevenSeg_write_number(float number)
{
   char fchar[8];
   float_to_string(number,fchar,2);
   if(fchar[0] == '-')
   {
     #if display_com == com_Anode 
     dig[2] = 253;
     #else
     dig[2] = 2;
     #endif
   }
   else
   {
         #if display_com == com_Anode 
         dig[2] = segment_data[fchar[0] - '0'];
         #else
         dig[2] = ~segment_data[fchar[0] - '0'];
         #endif
   }

    if(fchar[1] == '.') 
   {
     #if display_com == com_Anode 
     dig[2] --;
          dig[1] = segment_data[fchar[2] - '0'];
     dig[0] = segment_data[fchar[3] - '0'];
     #else
     dig[2] ++;
          dig[1] = ~segment_data[fchar[2] - '0'];
     dig[0] = ~segment_data[fchar[3] - '0'];
     #endif

   }
   else if(fchar[2] == '.')
   {
#if display_com == com_Anode
     dig[1] = segment_data[fchar[1] - '0'];
       dig[1] --;
        dig[0] = segment_data[fchar[3] - '0'];
#else 
     dig[1] = ~segment_data[fchar[1] - '0'];
     dig[1] ++;
      dig[0] = ~segment_data[fchar[3] - '0'];
#endif
   }
   else
   {
      #if display_com == com_Anode 
      dig[0] = segment_data[fchar[2] - '0'];
      dig[1] = segment_data[fchar[1] - '0'];
     #else
      dig[0] = ~segment_data[fchar[2] - '0'];
      dig[1] = ~segment_data[fchar[1] - '0'];
     #endif

   }

   sevenSeg_update_Digit();
}

void sevenSeg_write_diffrent(unsigned char digit3,unsigned char digit2,unsigned char digit1)
{
  dig[0] = digit1;
  dig[1] = digit2;
  dig[2] = digit3;
  
  sevenSeg_update_Digit();
}

void sevenSeg_clear()
{
#if display_com == com_Anode 
 dig[0] = 0xFF;
  dig[1] = 0xFF;
   dig[2] = 0xFF;
#else
   dig[0] = 0;
  dig[1] = 0;
   dig[2] = 0;
#endif
}
