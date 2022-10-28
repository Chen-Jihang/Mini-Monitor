#include "INA226.h"
#include "myiic.h"

INA226 ina226_data;

//��ʼ��INA226
void INA226_Init(void)
{	
	IIC_Init();
	INA226_SendData(INA226_ADDR1, CFG_REG, 0x8000);	//��������
	INA226_SendData(INA226_ADDR1, CFG_REG, 0x44df);	// 0100_010_011_011_111 //16��ƽ��, 588us, 588us, ��������������ѹ�����ߵ�ѹ
	INA226_SendData(INA226_ADDR1, CAL_REG, CAL);	//�ֱ���0.02mA, ����655.36mA
	INA226_Get_ID(INA226_ADDR1);					//��ȡina226��ID
}

//���üĴ���ָ��
void INA226_SetRegPointer(u8 addr,u8 reg)
{
	IIC_Start();

	IIC_Send_Byte(addr);
	IIC_Wait_Ack();

	IIC_Send_Byte(reg);
	IIC_Wait_Ack();

	IIC_Stop();
}

//����,д������
void INA226_SendData(u8 addr,u8 reg,u16 data)
{
	u8 temp=0;
	IIC_Start();

	IIC_Send_Byte(addr);
	IIC_Wait_Ack();

	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	
	temp = (u8)(data>>8);
	IIC_Send_Byte(temp);
	IIC_Wait_Ack();

	temp = (u8)(data&0x00FF);
	IIC_Send_Byte(temp);
	IIC_Wait_Ack();
	
	IIC_Stop();
}

//��ȡ����
u16 INA226_ReadData(u8 addr)
{
	u16 temp=0;
	IIC_Start();

	IIC_Send_Byte(addr+1);
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);
	temp<<=8;	
	temp |= IIC_Read_Byte(0);
	
	IIC_Stop();
	return temp;
}

//��ȡid
void INA226_Get_ID(u8 addr)
{
	u16 temp=0;
	INA226_SetRegPointer(addr,INA226_GET_ADDR);
	temp = INA226_ReadData(addr);
	ina226_data.ina226_id = temp;
}

//��ȡУ׼ֵ
u16 INA226_GET_CAL_REG(u8 addr)
{	
	u32 temp=0;
	INA226_SetRegPointer(addr,CAL_REG);
	temp = INA226_ReadData(addr);
	return (u16)temp;
}

//2.5uV/bit
u16 INA226_GetShuntVoltage(u8 addr)
{
	int16_t temp = 0;
	INA226_SetRegPointer(addr,SV_REG);
	temp = INA226_ReadData(addr);
	if(temp&0x8000)	temp = ~(temp - 1);	
	return (u16)temp;	
}

//1.25mV/bit
u16 INA226_GetVoltage(u8 addr)
{
	u32 temp = 0;
	INA226_SetRegPointer(addr,BV_REG);
	temp = INA226_ReadData(addr);
	return (u16)temp;	
}

//0.02mA/bit
u16 INA226_GetShunt_Current(u8 addr)
{
	u16 temp=0;	
	INA226_SetRegPointer(addr,CUR_REG);
	temp = INA226_ReadData(addr);
	if(temp&0x8000)	temp = ~(temp - 1);	
	return temp;
}

//��ȡ������ѹ
void Get_Shunt_voltage(float *Voltage)//uV
{
	*Voltage = (INA226_GetShuntVoltage(INA226_ADDR1)*INA226_VAL_LSB);
}

//��ȡ��ѹ
void GetVoltage(float *Voltage)//mV
{
	*Voltage = INA226_GetVoltage(INA226_ADDR1)*Voltage_LSB;
}

//��ȡ����
void Get_Shunt_Current(float *Current)//mA
{
	*Current = (INA226_GetShunt_Current(INA226_ADDR1)* CURRENT_LSB);
}

//��ȡ����= ���ߵ�ѹ * ����
void get_power(void)//W
{
	GetVoltage(&ina226_data.voltageVal);			//mV
	Get_Shunt_voltage(&ina226_data.Shunt_voltage);	//uV
	Get_Shunt_Current(&ina226_data.Shunt_Current);	//mA
	Get_Power(&ina226_data.Power);
	ina226_data.Power_Val = ina226_data.voltageVal*0.001f * ina226_data.Shunt_Current*0.001f; 	//mV*mA
}

//��ȡ����װ��ֵ,ina226�ڲ�����ĵĹ��ʣ�����δ��У׼���ʲ�����
u16 INA226_Get_Power(u8 addr)
{
	int16_t temp=0;
	INA226_SetRegPointer(addr,PWR_REG);
	temp = INA226_ReadData(addr);
	return (u16)temp;
}

//��ȡ����,ina226�ڲ����㣬��׼ȷ��������
void Get_Power(float *Power)//W
{
	*Power = (INA226_Get_Power(INA226_ADDR1)*POWER_LSB);
}

//�����ñ���������
/*
u8 INA226_AlertAddr()
{
	u8 temp;
	IIC_Start();

	IIC_Send_Byte(INA226_GETALADDR);
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);
	
	IIC_Stop();
	return temp;
}
*/

float calc_average(float last_avg, float next_val, uint32_t num)
{
	float avg_val=0;

	if(num<=1)
	{
		avg_val = next_val;
	}
	else
	{
		avg_val = last_avg * (((float)num-1)/(float)num) + next_val * (1/(float)num);/*must convert to float*/
	}
	return avg_val;
}

