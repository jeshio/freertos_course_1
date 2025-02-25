/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TASK_DEBUG
#define IDDLE_MON
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId myTask01Handle;
osThreadId myTask03Handle;
osThreadId myTask02Handle;
osThreadId myTask04Handle;
osMessageQId myQueue01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
// переключатели для отслеживания задач
void TaskSwitchedIn(int tag)
{
#ifdef TASK_DEBUG
  switch (tag)
  {
  case 1:
    GPIOA->BSRR = GPIO_PIN_8;
    break;
  case 2:
    GPIOB->BSRR = GPIO_PIN_10;
    break;
  case 3:
    GPIOB->BSRR = GPIO_PIN_4;
    break;
  case 4:
    GPIOB->BSRR = GPIO_PIN_5;
    break;
  default:
    break;
  }
#endif
}
void TaskSwitchedOut(int tag)
{
#ifdef TASK_DEBUG
  switch (tag)
  {
  case 1:
    GPIOA->BSRR = (uint32_t)GPIO_PIN_8 << 16U;
    break;
  case 2:
    GPIOB->BSRR = (uint32_t)GPIO_PIN_10 << 16U;
    break;
  case 3:
    GPIOB->BSRR = (uint32_t)GPIO_PIN_4 << 16U;
    break;
  case 4:
    GPIOB->BSRR = (uint32_t)GPIO_PIN_5 << 16U;
    break;
  default:
    break;
  }
#endif
}
/* USER CODE END FunctionPrototypes */

void StartTask01(void const *argument);
void StartTask03(void const *argument);
void StartTask02(void const *argument);
void StartTask04(void const *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize);

/* Hook prototypes */
void vApplicationIdleHook(void);
void vApplicationTickHook(void);

/* USER CODE BEGIN 2 */
__weak void vApplicationIdleHook(void)
{
/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
task. It is essential that code added to this hook function never attempts
to block in any way (for example, call xQueueReceive() with a block time
specified, or call vTaskDelay()). If the application makes use of the
vTaskDelete() API function (as this demo application does) then it is also
important that vApplicationIdleHook() is permitted to return to its calling
function, because it is the responsibility of the idle task to clean up
memory allocated by the kernel to any task that has since been deleted. */
#ifdef IDDLE_MON
  // статистика простоя для анализатора
  GPIOB->BSRR = GPIO_PIN_3;
  __NOP();
  GPIOB->BSRR = (uint32_t)GPIO_PIN_3 << 16U;
#endif
}
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
__weak void vApplicationTickHook(void)
{
  /* This function will be called by each tick interrupt if
  configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
  added here, but the tick hook is called from an interrupt context, so
  code must not attempt to block, and only the interrupt safe FreeRTOS API
  functions can be used (those that end in FromISR()). */

  GPIOA->BSRR = GPIO_PIN_10;
  GPIOA->BSRR = (uint32_t)GPIO_PIN_10 << 16U;
}
/* USER CODE END 3 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
  /* USER CODE BEGIN Init */

  /* Define the tasks */
  osThreadDef(myTask01, StartTask01, osPriorityNormal, 0, 128);
  osThreadDef(myTask02, StartTask02, osPriorityNormal, 0, 128);
  osThreadDef(myTask03, StartTask03, osPriorityHigh, 0, 128);
  osThreadDef(myTask04, StartTask04, osPriorityNormal, 0, 128);

  /* Start the tasks */
  myTask01Handle = osThreadCreate(osThread(myTask01), NULL);
  myTask02Handle = osThreadCreate(osThread(myTask02), NULL);
  myTask03Handle = osThreadCreate(osThread(myTask03), NULL);
  myTask04Handle = osThreadCreate(osThread(myTask04), NULL);

  /* USER CODE END Init */
  /* USER CODE BEGIN Header */
  /**
   ******************************************************************************
   * File Name          : freertos.c
   * Description        : Code for freertos applications
   ******************************************************************************
   * @attention
   *
   * Copyright (c) 2025 STMicroelectronics.
   * All rights reserved.
   *
   * This software is licensed under terms that can be found in the LICENSE file
   * in the root directory of this software component.
   * If no LICENSE file comes with this software, it is provided AS-IS.
   *
   ******************************************************************************
   */
  /* USER CODE END Header */

  /**
   * @}
   */

  /**
   * @}
   */
}

/* USER CODE BEGIN Header_StartTask01 */
/**
 * @brief  Function implementing the myTask01 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask01 */
void StartTask01(void const *argument)
{
  /* USER CODE BEGIN StartTask01 */
#ifdef TASK_DEBUG
  // отмечаем выполнение задачи 1 для логического анализатора
  vTaskSetApplicationTaskTag(NULL, (void *)1);
#endif
  /* Infinite loop */
  for (;;)
  {
    // нагрузка для увеличения времени выполнения задачи
    for (int i = 0; i < 0x2F; i++)
    {
      __NOP();
    }
    osDelay(1);
  }
  /* USER CODE END StartTask01 */
}

/* USER CODE BEGIN Header_StartTask03 */
/**
 * @brief Function implementing the myTask03 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask03 */
void StartTask03(void const *argument)
{
  /* USER CODE BEGIN StartTask03 */
#ifdef TASK_DEBUG
  // отмечаем выполнение задачи 1 для логического анализатора
  vTaskSetApplicationTaskTag(NULL, (void *)3);
#endif
  /* Infinite loop */
  for (;;)
  {
    // нагрузка для увеличения времени выполнения задачи
    for (int i = 0; i < 0x2F; i++)
    {
      __NOP();
    }
    osDelay(1);
  }
  /* USER CODE END StartTask03 */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
 * @brief Function implementing the myTask02 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask02 */
void StartTask02(void const *argument)
{
  /* USER CODE BEGIN StartTask02 */
#ifdef TASK_DEBUG
  // отмечаем выполнение задачи 1 для логического анализатора
  vTaskSetApplicationTaskTag(NULL, (void *)2);
#endif
  /* Infinite loop */
  for (;;)
  {
    // нагрузка для увеличения времени выполнения задачи
    for (int i = 0; i < 0x2F; i++)
    {
      __NOP();
    }
    osDelay(1);
  }
  /* USER CODE END StartTask02 */
}

/* USER CODE BEGIN Header_StartTask04 */
/**
 * @brief Function implementing the myTask04 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTask04 */
void StartTask04(void const *argument)
{
  /* USER CODE BEGIN StartTask04 */
#ifdef TASK_DEBUG
  // отмечаем выполнение задачи 1 для логического анализатора
  vTaskSetApplicationTaskTag(NULL, (void *)4);
#endif
  /* Infinite loop */
  for (;;)
  {
    // нагрузка для увеличения времени выполнения задачи
    for (int i = 0; i < 0x2F; i++)
    {
      __NOP();
    }
    osDelay(1);
  }
  /* USER CODE END StartTask04 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
