/*
 * App.h
 *
 *  Created on: Apr 28, 2022
 *      Author: Ahmed Soliman
 */

#ifndef APP_H_
#define APP_H_


typedef enum{
	Calculator_Init,
	Calculator_GetNum1AndOpr,
	Calcualtor_GetNum2,
	Calculator_Reasult,
	Calculator_ErrorHandle,
}CalCualtorStates;


void Calculator_Task(void);


#endif /* APP_H_ */
