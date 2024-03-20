#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>


#ifndef SIMULATOR
#include "main.h"
#include "cmsis_os2.h"
#include <cstring>
#include "cmsis_os.h"

extern "C"
{
	extern osMessageQueueId_t uartQueueHandle;
	uint8_t data2[2048];
}
#endif

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
#ifndef SIMULATOR
	if (osMessageQueueGetCount(uartQueueHandle)>0)
	{
//		if (xQueueReceive(uartQueueHandle, &uartData_r, 10000))
		if (osMessageQueueGet(uartQueueHandle, &data2, 0, 0)== osOK)
		{
			strncpy((char*)RData, (char *)data2, 2048);
			modelListener->uart_Data (RData);
		}
	}
#endif
}
