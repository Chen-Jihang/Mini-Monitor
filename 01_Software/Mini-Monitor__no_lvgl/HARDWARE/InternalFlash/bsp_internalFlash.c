  
#include "bsp_internalFlash.h"   

/*准备写入的测试数据*/
//#define DATA_32                 ((uint32_t)0x00000001)


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* 要擦除内部FLASH的起始地址 */
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_3   
/* 要擦除内部FLASH的结束地址 */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_4  


static uint32_t GetSector(uint32_t Address);


/**
  * @brief  InterFlash_Read_Word,对内部FLASH进行读
  * @param  None
  * @retval None
  */
uint32_t InterFlash_Read_Word(uint32_t Address)
{
	return *(__IO uint32_t*)Address;
}

/**
  * @brief  InterFlash_Write_Word,对内部FLASH进行读写
  * @param  None
  * @retval None
  */
int InterFlash_Write_Word(uint32_t oneword)
{
	/*要擦除的起始扇区(包含)及结束扇区(不包含)，如8-12，表示擦除8、9、10、11扇区*/
	uint32_t FirstSector = 0;
	uint32_t NbOfSectors = 0;
	
	uint32_t SECTORError = 0;
	
	uint32_t Address = 0;

	__IO uint32_t Data32 = 0;
	__IO uint32_t MemoryProgramStatus = 0;
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	/* FLASH 解锁 ********************************/
	/* 使能访问FLASH控制寄存器 */
	HAL_FLASH_Unlock();

	FirstSector = GetSector(FLASH_USER_START_ADDR);
	NbOfSectors = 1;
//	NbOfSectors = GetSector(FLASH_USER_END_ADDR)- FirstSector + 1;
	
	/* 擦除用户区域 (用户区域指程序本身没有使用的空间，可以自定义)**/
	/* Fill EraseInit structure*/
	EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;/* 以“字”的大小进行操作 */ 
	EraseInitStruct.Sector        = FirstSector;
	EraseInitStruct.NbSectors     = NbOfSectors;
	/* 开始擦除操作 */
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	{
		/*擦除出错，返回，实际应用中可加入处理 */
		return -1;
	}

	/* 以“字”的大小为单位写入数据 ********************************/
	Address = FLASH_USER_START_ADDR;

//	while (Address < FLASH_USER_END_ADDR)
	while (Address < FLASH_USER_START_ADDR + 4) //only store 1 word
	{
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, oneword) == HAL_OK)
		{
		  Address = Address + 4;
		}
		else
		{ 
		  /*写入出错，返回，实际应用中可加入处理 */
				return -1;
		}
	}


	/* 给FLASH上锁，防止内容被篡改*/
	HAL_FLASH_Lock(); 


	/* 从FLASH中读取出数据进行校验***************************************/
	/*  MemoryProgramStatus = 0: 写入的数据正确
	  MemoryProgramStatus != 0: 写入的数据错误，其值为错误的个数 */
	Address = FLASH_USER_START_ADDR;
	MemoryProgramStatus = 0;

//	while (Address < FLASH_USER_END_ADDR)
	while (Address < FLASH_USER_START_ADDR + 4) //only ckeck 1 word
	{
		Data32 = *(__IO uint32_t*)Address;

		if (Data32 != oneword)
		{
		  MemoryProgramStatus++;  
		}

		Address = Address + 4;
	}  
	/* 数据校验不正确 */
	if(MemoryProgramStatus)
	{    
		return -1;
	}
	else /*数据校验正确*/
	{ 
		return 0;   
	}
}

/**
  * @brief  根据输入的地址给出它所在的sector
  *					例如：
						uwStartSector = GetSector(FLASH_USER_START_ADDR);
						uwEndSector = GetSector(FLASH_USER_END_ADDR);	
  * @param  Address：地址
  * @retval 地址所在的sector
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
	else if((Address < (uint32_t)0x08040000) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
//  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
//  {
//    sector = FLASH_SECTOR_5;  
//  }
//  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
//  {
//    sector = FLASH_SECTOR_6;  
//  }
//  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
//  {
//    sector = FLASH_SECTOR_7;  
//  }
  return sector;
}



