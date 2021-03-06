/*
    Auther       : Heinz Samuelsson
    Date         : 2019-02-12
    File         : dio.c
    Reference    : -
    Description  : -
*/ 
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#define ENUM2STRING(_str) _str == DIN_DB_LOGIC_2 ? "DIN_DB_LOGIC_2" : \
                  	  _str == DIN_DB_LOGIC_3 ? "DIN_DB_LOGIC_3" : \
                  	  _str == DIN_DB_LOGIC_4 ? "DIN_DB_LOGIC_4" : \
                  	  _str == DIN_DB_LOGIC_5 ? "DIN_DB_LOGIC_5" : \
                  	  _str == DIN_DB_LOGIC_6 ? "DIN_DB_LOGIC_6" : \
                  	  _str == DIN_DB_LOGIC_7 ? "DIN_DB_LOGIC_7" : "UNKNOWN"

typedef enum {
	DIN_DB_LOGIC_2,
	DIN_DB_LOGIC_3,
	DIN_DB_LOGIC_4,
	DIN_DB_LOGIC_5,
	DIN_DB_LOGIC_6,
	DIN_DB_LOGIC_7,
	IO_DIN_NUM ///< Number of digital inputs, not a valid value
} IO_DIN_t;

/* Returns 1 if active/high, 0 if inactive/low, possibly -1 if unknown/undefined. */
typedef bool (*din_read_func_t)(unsigned); 

static struct din {

	unsigned num;
	din_read_func_t read_func;

	struct per_input_data {
		int raw_value;
		int filtered_value;
		unsigned filter_len;
		unsigned filter;
	} *pin;
} din;

void dio_set_filter_length(unsigned pin, unsigned length)
{
	//printf("    %s() - length=%d\n", __func__, length);

	assert(pin < din.num);
	din.pin[pin].filter_len = length;
}

bool DIO_GetRawInput(unsigned input) {
	//printf("%s() - \n", __func__);
	bool value=0;

	switch (input) {
	case DIN_DB_LOGIC_6:
		//value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
		break;
	case DIN_DB_LOGIC_2:
		//value = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
		value = 1;
		break;
	case DIN_DB_LOGIC_3:
		//value = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
		break;
	case DIN_DB_LOGIC_4:
		//value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
		value = 1;
		break;
	case DIN_DB_LOGIC_5:
		//value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13);
		break;
	case DIN_DB_LOGIC_7:
		//value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
		break;
	default:
		break;
	}

	printf("%s() - Port=%s, value=%d\n", __func__, ENUM2STRING(input), value);
	return value;
}

int dio_init_inputs(unsigned num, din_read_func_t read_func)
{
	printf("%s() - num=%d\n", __func__, num);

	assert(read_func != NULL);
	din.pin = calloc(num, sizeof(*din.pin));

	if(din.pin == NULL)
		return -ENOMEM;

	for (unsigned i = 0; i < num; i++) {
		din.pin[i].filter = UINT_MAX;
	}
	din.read_func = read_func;
	din.num = num;

	return 0;
}

void IO_Init(void) {
	printf("%s() - IO_DIN_NUM=%d\n", __func__, IO_DIN_NUM);

	dio_init_inputs(IO_DIN_NUM, &DIO_GetRawInput);

	for(int i=0;i<IO_DIN_NUM;i++)
	{
		dio_set_filter_length(i, 32);
	}
}

void dio_update_filter(unsigned pin)
{
	//printf("%s() - \n", __func__);

	assert(pin < din.num);
	assert(din.read_func != NULL);

	din.pin[pin].raw_value = din.read_func(pin);

	if (din.pin[pin].filter == UINT_MAX) {
		din.pin[pin].filtered_value = din.pin[pin].raw_value;
		din.pin[pin].filter = 0;
	} else if (din.pin[pin].filtered_value == din.pin[pin].raw_value) {
		din.pin[pin].filter = 0;
	} else if (din.pin[pin].filter >= din.pin[pin].filter_len) {
		din.pin[pin].filtered_value = din.pin[pin].raw_value;
		din.pin[pin].filter = 0;
	} else {
		din.pin[pin].filter++;
	}
}

void dio_update_filters(void)
{
	for (unsigned i = 0; i < din.num; i++)
		dio_update_filter(i);
}

int dio_get_raw_input(unsigned pin)
{
	printf("%s() - pin=%d\n", __func__, pin);
	return din.pin[pin].raw_value;
}

int dio_get_input(unsigned pin)
{
	printf("%s() -\n", __func__);
	return din.pin[pin].filtered_value;
}

int main(int argc, char *argv[])
{
	IO_Init();

	dio_update_filters();

	int port = DIN_DB_LOGIC_4;
	int rv = dio_get_raw_input(port);
	printf("%s() - Port=%s, rv=%d\n", __func__, ENUM2STRING(port), rv);

	//int port = DIN_DB_LOGIC_2;
	//int rv = dio_get_input(port); 
	//printf("%s() - Port=%s, rv=%d\n", __func__, ENUM2STRING(port), rv);

	//port = DIN_DB_LOGIC_4;
	//rv = dio_get_input(port); 
	//printf("%s() - Port=%s, rv=%d\n", __func__, ENUM2STRING(port), rv);

	//port = DIN_DB_LOGIC_5;
	//rv = dio_get_input(port); 
	//printf("%s() - Port=%s, rv=%d\n", __func__, ENUM2STRING(port), rv);

        return 0;
}
