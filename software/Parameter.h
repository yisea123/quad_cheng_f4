#ifndef _PARAMETER_H_
#define _PARAMETER_H_


#include "sys.h"
#include "string.h"



//ͷ��Ϣ===============================
const uint8_t  PARAMETER_MAGIC1 = 'P';
const uint8_t  PARAMETER_MAGIC2 = 'A';
const uint8_t  PARAMETER_MAGIC3 = 'M';
const uint8_t PARAMETER_VER		= 20;
const uint8_t PARAMETER_SUB_VER	= 13;

//====================================

const uint32_t PARAM_FLASH_ADDR_BASE =	0;	//�������ڴ��еĵ�ַ
const int PARAMETER_NUM_MAX = 100;			//��������
const int PARAMETER_BUFFER_SIZE = 512;		//�����ڴ��С


/*************************************************************************/
class ParameterBase
{
public:
	ParameterBase();
	virtual ~ParameterBase() {}; 

	static int Count();									//��ȡ��������
	static ParameterBase* GetAddress(uint16_t index);	//��ȡ������ַ
	static void Setup();								//��ȡ�ڴ浽���в���
	static void SaveAll();								//�������в������ڴ�
	
	void save() { _save_all = true; }
	static bool SaveRequeset() { return _save_all; }

	//virtual uint8_t get_size() = 0;
protected:
	//������Ҫʵ������������
	virtual uint8_t param_get(uint8_t *p) = 0;
	virtual uint8_t param_set(uint8_t *p)const = 0;
private:
	static ParameterBase* ParametersAddress[PARAMETER_NUM_MAX];
	static uint16_t ParametersNum;

	static bool _save_all;
//	bool _save;
	uint32_t _offset;	//��flash�е�ƫ�Ƶ�ַ�����ڶ�ȡ��λ
};

/*************************************************************************/


template <typename T>
class ParameterBasic:public ParameterBase
{
public:
	ParameterBasic(T default_val = 0) :ParameterBase(),_val(default_val)
	{}

	const T& operator=(const T& rhs) { _val = rhs; return _val; }	//��ֵ����
	operator T(){	return _val;}									//ȡֵ����
	T get()const {return _val;}
	void set(const T& rhs) { _val = rhs; }

private:
	virtual uint8_t param_get(uint8_t *p);
	virtual uint8_t param_set(uint8_t *p)const;
	T _val;
};
template <typename T>
uint8_t ParameterBasic<T>::param_get(uint8_t *p)
{
	
	memcpy((uint8_t*)&_val,p,sizeof(T));
	return sizeof(T);
}
template <typename T>
uint8_t ParameterBasic<T>::param_set(uint8_t *p)const
{
	memcpy(p,&_val,sizeof(T));
	return sizeof(T);
}


/*************************************************************************/


typedef ParameterBasic<float> 			Param_Float;
typedef ParameterBasic<double> 			Param_Double;
typedef ParameterBasic<uint16_t> 		Param_UInt16;
typedef ParameterBasic<int16_t> 		Param_Int16;
typedef ParameterBasic<uint32_t> 		Param_UInt32;
typedef ParameterBasic<int32_t> 		Param_Int32;
typedef ParameterBasic<uint8_t> 		Param_UInt8;
typedef ParameterBasic<int8_t> 			Param_Int8;
typedef ParameterBasic<char> 			Param_Char;

/*************************************************************************/

#endif
