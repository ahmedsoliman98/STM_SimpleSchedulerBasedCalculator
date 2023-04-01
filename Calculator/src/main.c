#include "../../Std_types.h"
#include "../../Bit_utils.h"
#include "MCAL/Rcc.h"
#include "Sched/Sched.h"
#include "HAL/Keypad.h"
#include "HAL/Lcd.h"



int
main(int argc, char* argv[])
{
  Rcc_enuPerpheralClk_Control(Rcc_u8AHB1_BUS,Rcc_u32GPIOAEN_Perpheral_AHB1,Rcc_u8ClkON_State);
  Rcc_enuPerpheralClk_Control(Rcc_u8AHB1_BUS,Rcc_u32GPIOBEN_Perpheral_AHB1,Rcc_u8ClkON_State);
  Rcc_enuPerpheralClk_Control(Rcc_u8AHB1_BUS,Rcc_u32GPIOCEN_Perpheral_AHB1,Rcc_u8ClkON_State);

  Lcd_vidInit();
  Keypad_enuInit();
  Sched_init();
  Sched_Start();

return 0;

}

