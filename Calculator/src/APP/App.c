/*
 * App.c
 *
 *  Created on: Apr 28, 2022
 *      Author: Ahmed Soliman
 */
#include "../../Bit_utils.h"
#include "../../Std_types.h"
#include "../HAL/Keypad.h"
#include "../HAL/Lcd.h"

typedef enum{
	Calculator_Init,
	Calculator_GetValue,
	Calculator_Reasult,
	Calculator_ErrorHandle,
}CalCualtorStates;

typedef enum{
	Calculator_DELAY_120ms,
	CLEAR_SCREEN,
	GOTO_R1C6,
	DISPLAY_WELCOME,
	GOTO_R2C1,
	DISPLAY_SOLLYCAL,
	WAIT_500msec,
}Init_Process;


typedef enum{
	Calculator_CLRSCREEN,
	Calculator_GetKey,
	Calculator_CheckEntered,
	Calculator_ChackEnterd2,
	CalculatorError,
}Get_Process;

typedef enum{
	Calculator_DispResult,
	Calculator_GotoR2C1,
	Calculator_DispStr,
	Calculator_GetDoneKey,
}Result_Process;

/* THE CONDITON OF THE STATIC PROCESS */
typedef enum{
	DONE,
	NOT_YET,
}Process_Condition_t;
/* BUFFER TO USE HOLD STRING */
buffer_t string ;

static u16 Num1 , Num2, Result;  // NUMBERS VARIABLE HOLD A INTEGER NUMBER THAT USER ENTER
static u8 Oper;                  //USER OPERATOR
static u8 GetKey_Data;

/* THE INITIALLIZATION OF THE CALCULATOR WHEN IT GET START EACH TIME */
static CalCualtorStates Cal_Init(void)
{
	CalCualtorStates Loc_enuStateCond = NOT_YET;
	static u8 Loc_u8swTimer = 0;
	static Init_Process Init = Calculator_DELAY_120ms;
	switch(Init)
	{
	case Calculator_DELAY_120ms:
		/* WAIT 120MS AFTER SETUP THE PINS */
		Loc_u8swTimer++;
		if(Loc_u8swTimer == 2)
		{
			Init = CLEAR_SCREEN;
			Loc_u8swTimer = 0;
		}
		break;
	case CLEAR_SCREEN:
		Lcd_CLRDisplay();
		Init = GOTO_R1C6;
		break;
	case GOTO_R1C6:
		/* CENTER ON THE MIDDLE OF THE LCD */
		Lcd_vidgoto(Lcd_u8Row1,Lcd_u8Clm6);
		Init = DISPLAY_WELCOME;
		break;
	case DISPLAY_WELCOME:
		/* DISPLAY WELCOM IN THE FIRST ROW */
		string.String = "Welcome";
		string.STRG_Size = 7;
		Lcd_vidDisplayStringCOPY(&string);
		Init = GOTO_R2C1;
		break;
	case GOTO_R2C1:
		/* DISPLAY IN THE ROW 2*/
		Lcd_vidgoto(Lcd_u8Row2,Lcd_u8Clm1);
		Init = DISPLAY_SOLLYCAL;
		break;
	case DISPLAY_SOLLYCAL:
		/* DISPLAY SOLLY CALCULATOR IN THE SECOUND ROW */
		string.String = "Solly Calculator";
		string.STRG_Size = 16;
		Lcd_vidDisplayStringCOPY(&string);
		Init = WAIT_500msec;
		break;
	case WAIT_500msec:
		/* WAITE TO MAKE STRING DISPLAY */
		Loc_u8swTimer++;
		if(Loc_u8swTimer == 10)
		{
			Loc_enuStateCond = DONE;
			Loc_u8swTimer = 0;
		}

		break;

	}
	return Loc_enuStateCond;
}


static CalCualtorStates Cal_GetValue(void)
{
	CalCualtorStates Loc_enuStateCond = NOT_YET;
	static Get_Process Loc_enuGetProcess = Calculator_CLRSCREEN;
	u8 Loc_u8GetKeyyData = READED;
	switch(Loc_enuGetProcess)
	{
	case Calculator_CLRSCREEN:
		Lcd_CLRDisplay();
		Loc_enuGetProcess = Calculator_GetKey;
		break;
		/* COVER THAT ANY NEED OF GET VALUE FROM USER */
	case Calculator_GetKey:
		Loc_u8GetKeyyData = Keypad_u8GetKey();
		/* WAITTING UNTILL THE USER THE USER ENTER VALUE */
		if(Loc_u8GetKeyyData != READED)
		{
			// HOLD THE ENTERD VALUE INTO AN GLOBAL VALUE TO BE CHECK IN THE NEXT CYCLE
			GetKey_Data = Loc_u8GetKeyyData;
			Keypad_KeyReaded();

			// PREPARE VALUE OF THE NEXT CYCLE
			/* THAT MEAN THAT THE CHECKENTERD 1 NOT FINISH */
			if(Oper == 0)
			{
				Loc_enuGetProcess = Calculator_CheckEntered;
			}
			else
			{
				Loc_enuGetProcess = Calculator_ChackEnterd2;
			}

		}
		break;
	case Calculator_CheckEntered:
		/* CHECK IF THE USER SEND OPERATOR */
		if(GetKey_Data == '+'  || GetKey_Data == '-' || GetKey_Data == '*' || GetKey_Data == '/')
		{
			/* THE USER ENTER OPERATOR BEFOR THE FIRST NUMBER IT IS AN ERROR */
			if(Num1 == 0)
			{
				Loc_enuGetProcess = CalculatorError;
			}
			/* HOLD THE SENDED OPERATOR IN A GLOBAL VARIABLE TO BE SWITCHED IN REASULT FUNCTION */
			else
			{
				/* DISPALY THE GETTING OPERATOR */
				Lcd_vidDisplayChar(GetKey_Data);
				Oper = GetKey_Data;
				GetKey_Data = READED;
				/* AFTER GET THE OPERATOR IT BE READY TO NEXT STATOF THE PROCESS */
				Loc_enuGetProcess = Calculator_GetKey;
			}
		}
		/* CHECK ABOUT THE NUMBER TO BE IN RANGE 0-9 */
		else if(GetKey_Data >= '0' && GetKey_Data <= '9')
		{
			/* DISPLAY THE GETTING NUMBER */
			Lcd_vidDisplayChar(GetKey_Data);
			Num1 = Num1 * 10 + (GetKey_Data - '0'); // sub from 0 to get the integer represntation
			GetKey_Data = READED;                   // SET THE DATA TO BE READED TO AVOID REDISPLAY */
			Loc_enuGetProcess = Calculator_GetKey;
		}
		else
		{
			Loc_enuGetProcess = CalculatorError;
		}
		break;
	case Calculator_ChackEnterd2:
		/* CHECK IF THE USER SEND EQUAL */
		if(GetKey_Data == '=')
		{
			/* THE USER ENTER = BEFOR THE SECOUND NUMBER IT IS AN ERROR */
			if(Num2 == 0)
			{
				Loc_enuGetProcess = CalculatorError;
			}
			/* HOLD THE SENDED OPERATOR IN A GLOBAL VARIABLE TO BE SWITCHED IN REASULT FUNCTION */
			else
			{
				/* DISPALY THE GETTING OPERATOR */
				Lcd_vidDisplayChar(GetKey_Data);
				/* AFTER GET THE = THE PROCESS IS DONE  */
				Loc_enuStateCond = DONE;
				Loc_enuGetProcess = Calculator_CLRSCREEN;
			}
		}
		/* CHECK ABOUT THE NUMBER TO BE IN RANGE 0-9 */
		else if(GetKey_Data >= '0' && GetKey_Data <= '9')
		{
			/* DISPLAY THE GETTING NUMBER */
			Lcd_vidDisplayChar(GetKey_Data);
			Num2 = Num2 * 10 + (GetKey_Data - '0'); // sub from 0 to get the integer represntation
			GetKey_Data = READED;
			Loc_enuGetProcess = Calculator_GetKey;
		}
		else
		{
			Loc_enuGetProcess = CalculatorError;
		}
		break;
	case CalculatorError:
		break;
	}

	return Loc_enuStateCond;
}


static CalCualtorStates Cal_GetReasult(void)
{
	CalCualtorStates Loc_enuStateCond = NOT_YET;
	static Result_Process Loc_enuResult = Calculator_DispResult;
	u8 Loc_u8GetKeyyData = READED;
	switch(Loc_enuResult)
	{
	case Calculator_DispResult:
		switch(Oper)
		{
		case '+':
			Result = Num1 + Num2;
			break;
		case '-':
			Result = Num1 - Num2;
			break;
		case '*':
			Result = Num1 * Num2;
			break;
		case '/':
			Result = Num1 / Num2;
			break;
		}
		Lcd_vidDisplayNumber(Result);
		Loc_enuResult = Calculator_GotoR2C1;
		break;
		case Calculator_GotoR2C1:
			Lcd_vidgoto(Lcd_u8Row2,Lcd_u8Clm1);
			Loc_enuResult = Calculator_DispStr;
			break;
		case Calculator_DispStr:
			string.String = "press c ";
			string.STRG_Size = 7;
			Lcd_vidDisplayStringCOPY(&string);
			Loc_enuResult = Calculator_GetDoneKey;
			break;
		case Calculator_GetDoneKey:
			Loc_u8GetKeyyData = Keypad_u8GetKey();
			Keypad_KeyReaded();
			/* WAITTING UNTILL THE USER THE USER ENTER VALUE */
			if(Loc_u8GetKeyyData != READED)
			{
				if(Loc_u8GetKeyyData == 'c')
				{
					/* REINIT THE VALUES */
					Num1 = 0;
					Num2 = 0;
					Oper = 0;
					/* INIT THE VALUES OF THE FUNCTION STATE */
					Loc_enuResult = Calculator_DispResult;
					/* THE PROCESS IS DONE */
					Loc_enuStateCond = DONE;
				}
			}
			break;
	}



	return Loc_enuStateCond;
}


void Calculator_Task(void) // TASK CALL EVERY 40 ms
{
	static CalCualtorStates CalState = Calculator_Init;  //INDICATE THE PROCESS OF THE LCD
	Process_Condition_t CurProcessCond = NOT_YET;          // INDICATE THE CURRENT PROCESS CONDITION
	if(CalState == Calculator_Init )
	{
		CurProcessCond = Cal_Init();
		if(CurProcessCond == DONE)
		{
			CalState = Calculator_GetValue;
		}
	}
	else if(CalState == Calculator_GetValue)
	{
		CurProcessCond = Cal_GetValue();
		if(CurProcessCond == DONE)
		{
			CalState = Calculator_Reasult;
		}
	}
	else if(CalState == Calculator_Reasult)
	{
		CurProcessCond = Cal_GetReasult();
		if(CurProcessCond == DONE)
		{
			CalState = Calculator_GetValue;
		}
	}
}
