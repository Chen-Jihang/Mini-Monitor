#ifndef __INA226_H
#define __INA226_H
#include "main.h"
//	接线说明：
//	模拟IIC:
//	SDA -- PB6
//	SCL -- PB7

#define 	CFG_REG	 		0x00		//
#define 	SV_REG 			0x01		//分流电压
#define 	BV_REG 			0x02		//总线电压
#define 	PWR_REG 		0x03		//电源功率
#define 	CUR_REG 		0x04		//电流
#define 	CAL_REG 		0x05		//校准，设定满量程范围以及电流和功率测数的 
#define 	ONFF_REG 		0x06		//屏蔽 使能 警报配置和转换准备就绪
#define 	AL_REG 			0x07		//包含与所选警报功能相比较的限定值
#define 	INA226_GET_ADDR 0XFF		//包含唯一的芯片标识号
#define   	INA226_ADDR1	0x80 
//#define   INA226_GETALADDR	0x14 

//定义配置数据
#define 	INA226_VAL_LSB	2.5f	//分流电压 LSB 2.5uV
#define     Voltage_LSB		1.25f   //总线电压 LSB 1.25mV
#define     CURRENT_LSB 	0.02f 	//电流LSB 0.02mA
#define     POWER_LSB       25*CURRENT_LSB
#define     CAL             2560     //0.00512/(Current_LSB*R_SHUNT)*1000 = 0.00512/(0.02*0.1)*1000 = 2560
typedef struct
{
	float voltageVal;			//mV
	float Shunt_voltage;		//uV
	float Shunt_Current;		//mA
	float Shunt_Current_Avg;		//mA
	float Power_Val;			//功率
	float Power;				//功率mW
	u16   ina226_id;
}INA226;

void INA226_SetRegPointer(u8 addr,u8 reg);
void INA226_SendData(u8 addr,u8 reg,u16 data);

u16 INA226_ReadData(u8 addr);
void INA226_Get_ID(u8 addr);			//获取 id
u16 INA226_GetVoltage( u8 addr);		//获取总线电压装载值
u16 INA226_GetShunt_Current(u8 addr);	//获取分流电流装载值
u16 INA226_GetShuntVoltage(u8 addr);	//分流电压装载值
//u16 INA226_Get_Power(u8 addr);		//获取功率装载值，不使用
u16 INA226_GET_CAL_REG(u8 addr);		//获取校准值

void INA226_Init(void);
void GetVoltage(float *Voltage);		
void Get_Shunt_voltage(float *Current);
void Get_Shunt_Current(float *Current);
void get_power(void);						////获取功率= 总线电压 * 电流

u8	INA226_AlertAddr(void);
void Get_Power(float *Power);
extern INA226 ina226_data;

float calc_average(float last_avg, float next_val, uint32_t num);
#endif

