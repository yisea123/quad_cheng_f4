#include "Parameter.h"
#include "SPIFlash.h"


/********************************************
接口
get_size,read_param,write_param
成员
bool _save,
uint32_t offset;

加载时设置所有offset
保存：
读取全部，对需要保存的修改，擦除，重新写入



magic1,magic2,magic3,
ver,sub_ver
总长度
字符串长度,字符串,数据长度,数据

1.读取总数据长度,用于在后面补充数据
2.匹配名字,查看内存中是否存在该变量,若不存在,则在最后补充..
******************************************/

ParameterBase* ParameterBase::ParametersAddress[PARAMETER_NUM_MAX];
uint16_t ParameterBase::ParametersNum = 0;
bool ParameterBase::_save_all = false;


static uint8_t ParameterBuffer[PARAMETER_BUFFER_SIZE];





typedef struct 
{
	uint8_t magic1;
	uint8_t magic2;
	uint8_t magic3;

	uint16_t count;

	uint8_t version;
	uint8_t sub_version;


}ParamHeader;





ParameterBase::ParameterBase()
{
	if(ParametersNum < PARAMETER_NUM_MAX)
	{
		ParametersAddress[ParametersNum] = this;
		ParametersNum ++;
	}
	else
	{
		
	}
}



int ParameterBase::Count()
{
	return ParametersNum;
}


ParameterBase* ParameterBase::GetAddress(uint16_t index)
{
	if (index >= ParametersNum)
	{
		return NULL;
	}
	else
	{
		return ParametersAddress[index];
	}
}


void ParameterBase::Setup()
{
	uint16_t param_count = ParameterBase::Count();					//参数个数
	param_debug("Parameter Count:%d\r\n",ParameterBase::Count());	


	ParamHeader *ph = (ParamHeader *)ParameterBuffer;
	uint8_t *pparam = (uint8_t *)ParameterBuffer;

	spiflash.read(PARAM_FLASH_ADDR_BASE,ParameterBuffer,sizeof(ParamHeader));		//读取信息
	if (ph->magic1 == PARAMETER_MAGIC1 && ph->magic2 == PARAMETER_MAGIC2 && ph->magic3 == PARAMETER_MAGIC3&&
		ph->count == param_count && ph->version == PARAMETER_VER && ph->sub_version == PARAMETER_SUB_VER)	//验证头信息
	{
		param_debug("param check pass!\r\n");
		spiflash.read(PARAM_FLASH_ADDR_BASE, pparam, PARAMETER_BUFFER_SIZE);		//读取所有数据
		pparam += sizeof(ParamHeader);
		for (int i = 0; i < ParameterBase::Count(); i++)
		{
			ParameterBase *pb = ParameterBase::GetAddress(i);
			pparam += pb->param_get(pparam);
		}
		return;
	}

	param_debug("param check fail!\r\n");
	param_debug("param init....!\r\n");
	ph->magic1 = PARAMETER_MAGIC1;
	ph->magic2 = PARAMETER_MAGIC2;
	ph->magic3 = PARAMETER_MAGIC3;
	ph->count = param_count;
	ph->version = PARAMETER_VER;
	ph->sub_version = PARAMETER_SUB_VER;
	
	ParameterBase::SaveAll();
	param_debug("param finish....!\r\n");
}


void ParameterBase::SaveAll()
{
	spiflash.erase_sector(PARAM_FLASH_ADDR_BASE/spiflash.sector_size());

	uint8_t *pparam = ParameterBuffer + sizeof(ParamHeader);
	for (int i = 0; i < ParameterBase::Count(); i++)
	{
		ParameterBase *pb = ParameterBase::GetAddress(i);
		pparam += pb->param_set(pparam);
	}	
	spiflash.write_sector(PARAM_FLASH_ADDR_BASE,ParameterBuffer,pparam - ParameterBuffer);
	
	_save_all = false;

	param_debug("param size=%d\r\n",pparam - ParameterBuffer);

}


