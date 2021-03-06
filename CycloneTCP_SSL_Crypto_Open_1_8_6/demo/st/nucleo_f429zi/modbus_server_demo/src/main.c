/**
 * @file main.c
 * @brief Main routine
 *
 * @section License
 *
 * Copyright (C) 2010-2018 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 1.8.6
 **/

//Dependencies
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "os_port.h"
#include "core/net.h"
#include "drivers/mac/stm32f4x9_eth_driver.h"
#include "drivers/phy/lan8742_driver.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "modbus/modbus_server.h"
#include "error.h"
#include "debug.h"

//Application configuration
#define APP_MAC_ADDR "00-AB-CD-EF-04-29"

#define APP_USE_DHCP ENABLED
#define APP_IPV4_HOST_ADDR "192.168.0.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.0.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::429"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::429"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

//Global variables
uint16_t ledState[2];
DhcpClientSettings dhcpClientSettings;
DhcpClientContext dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;
ModbusServerSettings modbusServerSettings;
ModbusServerContext modbusServerContext;

//Forward declaration of functions
void modbusServerLockCallback(void);
void modbusServerUnlockCallback(void);

error_t modbusServerReadCoilCallback(uint16_t address, bool_t *state);

error_t modbusServerWriteCoilCallback(uint16_t address, bool_t state,
   bool_t commit);

error_t modbusServerReadRegCallback(uint16_t address, uint16_t *value);

error_t modbusServerWriteRegCallback(uint16_t address, uint16_t value,
   bool_t commit);


/**
 * @brief System clock configuration
 **/

void SystemClock_Config(void)
{
   RCC_OscInitTypeDef RCC_OscInitStruct;
   RCC_ClkInitTypeDef RCC_ClkInitStruct;

   //Enable Power Control clock
   __HAL_RCC_PWR_CLK_ENABLE();

   //The voltage scaling allows optimizing the power consumption when the device is
   //clocked below the maximum system frequency, to update the voltage scaling value
   //regarding system frequency refer to product datasheet.
   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

   //Enable HSE Oscillator and activate PLL with HSE as source
   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
   RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
   RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
   RCC_OscInitStruct.PLL.PLLM = 8;
   RCC_OscInitStruct.PLL.PLLN = 360;
   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
   RCC_OscInitStruct.PLL.PLLQ = 7;
   HAL_RCC_OscConfig(&RCC_OscInitStruct);

   //Enable overdrive mode
   HAL_PWREx_EnableOverDrive();

   //Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
   //clocks dividers
   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
   HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}


/**
 * @brief LED blinking task
 **/

void blinkTask(void *param)
{
   //Endless loop
   while(1)
   {
      BSP_LED_On(LED1);
      osDelayTask(100);
      BSP_LED_Off(LED1);
      osDelayTask(900);
   }
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   error_t error;
   NetInterface *interface;
   OsTask *task;
   MacAddr macAddr;
#if (APP_USE_DHCP == DISABLED)
   Ipv4Addr ipv4Addr;
#endif
#if (APP_USE_SLAAC == DISABLED)
   Ipv6Addr ipv6Addr;
#endif

   //HAL library initialization
   HAL_Init();
   //Configure the system clock
   SystemClock_Config();

   //Initialize kernel
   osInitKernel();
   //Configure debug UART
   debugInit(115200);

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("*****************************************\r\n");
   TRACE_INFO("*** CycloneTCP Modbus/TCP Server Demo ***\r\n");
   TRACE_INFO("*****************************************\r\n");
   TRACE_INFO("Copyright: 2010-2018 Oryx Embedded SARL\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: STM32F429\r\n");
   TRACE_INFO("\r\n");

   //LED configuration
   BSP_LED_Init(LED1);
   BSP_LED_Init(LED2);
   BSP_LED_Init(LED3);

   //Clear LEDs
   BSP_LED_Off(LED1);
   BSP_LED_Off(LED2);
   BSP_LED_Off(LED3);

   //Initialize user button
   BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_GPIO);

   //TCP/IP stack initialization
   error = netInit();
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
   }

   //Configure the first Ethernet interface
   interface = &netInterface[0];

   //Set interface name
   netSetInterfaceName(interface, "eth0");
   //Set host name
   netSetHostname(interface, "MBServerDemo");
   //Select the relevant network adapter
   netSetDriver(interface, &stm32f4x9EthDriver);
   netSetPhyDriver(interface, &lan8742PhyDriver);
   //Set host MAC address
   macStringToAddr(APP_MAC_ADDR, &macAddr);
   netSetMacAddr(interface, &macAddr);

   //Initialize network interface
   error = netConfigInterface(interface);
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
   }

#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP == ENABLED)
   //Get default settings
   dhcpClientGetDefaultSettings(&dhcpClientSettings);
   //Set the network interface to be configured by DHCP
   dhcpClientSettings.interface = interface;
   //Disable rapid commit option
   dhcpClientSettings.rapidCommit = FALSE;

   //DHCP client initialization
   error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
   //Failed to initialize DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize DHCP client!\r\n");
   }

   //Start DHCP client
   error = dhcpClientStart(&dhcpClientContext);
   //Failed to start DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start DHCP client!\r\n");
   }
#else
   //Set IPv4 host address
   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
   ipv4SetHostAddr(interface, ipv4Addr);

   //Set subnet mask
   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
   ipv4SetSubnetMask(interface, ipv4Addr);

   //Set default gateway
   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   ipv4SetDefaultGateway(interface, ipv4Addr);

   //Set primary and secondary DNS servers
   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 0, ipv4Addr);
   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (APP_USE_SLAAC == ENABLED)
   //Get default settings
   slaacGetDefaultSettings(&slaacSettings);
   //Set the network interface to be configured
   slaacSettings.interface = interface;

   //SLAAC initialization
   error = slaacInit(&slaacContext, &slaacSettings);
   //Failed to initialize SLAAC?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize SLAAC!\r\n");
   }

   //Start IPv6 address autoconfiguration process
   error = slaacStart(&slaacContext);
   //Failed to start SLAAC process?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start SLAAC!\r\n");
   }
#else
   //Set link-local address
   ipv6StringToAddr(APP_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   ipv6SetLinkLocalAddr(interface, &ipv6Addr);

   //Set IPv6 prefix
   ipv6StringToAddr(APP_IPV6_PREFIX, &ipv6Addr);
   ipv6SetPrefix(interface, 0, &ipv6Addr, APP_IPV6_PREFIX_LENGTH);

   //Set global address
   ipv6StringToAddr(APP_IPV6_GLOBAL_ADDR, &ipv6Addr);
   ipv6SetGlobalAddr(interface, 0, &ipv6Addr);

   //Set default router
   ipv6StringToAddr(APP_IPV6_ROUTER, &ipv6Addr);
   ipv6SetDefaultRouter(interface, 0, &ipv6Addr);

   //Set primary and secondary DNS servers
   ipv6StringToAddr(APP_IPV6_PRIMARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 0, &ipv6Addr);
   ipv6StringToAddr(APP_IPV6_SECONDARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 1, &ipv6Addr);
#endif
#endif

   //Get default settings
   modbusServerGetDefaultSettings(&modbusServerSettings);
   //Bind Modbus/TCP server to the desired interface
   modbusServerSettings.interface = &netInterface[0];
   //Listen to port 502
   modbusServerSettings.port = MODBUS_TCP_PORT;
   //Callback functions
   modbusServerSettings.lockCallback = modbusServerLockCallback;
   modbusServerSettings.unlockCallback = modbusServerUnlockCallback;
   modbusServerSettings.readCoilCallback = modbusServerReadCoilCallback;
   modbusServerSettings.writeCoilCallback = modbusServerWriteCoilCallback;
   modbusServerSettings.readRegCallback = modbusServerReadRegCallback;
   modbusServerSettings.writeRegValueCallback = modbusServerWriteRegCallback;

   //Modbus/TCP server initialization
   error = modbusServerInit(&modbusServerContext, &modbusServerSettings);
   //Failed to initialize Modbus/TCP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize Modbus/TCP server!\r\n");
   }

   //Start Modbus/TCP server
   error = modbusServerStart(&modbusServerContext);
   //Failed to start Modbus/TCP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start Modbus/TCP server!\r\n");
   }

   //Create a task to blink the LED
   task = osCreateTask("Blink", blinkTask, NULL, 500, OS_TASK_PRIORITY_NORMAL);
   //Failed to create the task?
   if(task == OS_INVALID_HANDLE)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Start the execution of tasks
   osStartKernel();

   //This function should never return
   return 0;
}


/**
 * @brief Lock Modbus table callback function
 **/

void modbusServerLockCallback(void)
{
}


/**
 * @brief Unlock Modbus table callback function
 **/

void modbusServerUnlockCallback(void)
{
}


/**
 * @brief Get coil state callback function
 * @param[in] address Coil address
 * @param[out] state Current coil state
 * @return Error code
 **/

error_t modbusServerReadCoilCallback(uint16_t address, bool_t *state)
{
   error_t error;

   //Check register address
   if(address == 0)
   {
      //Retrieve the state of the LED
      *state = ledState[0];
      //Successful processing
      error = NO_ERROR;
   }
   else if(address == 1)
   {
      //Retrieve the state of the LED
      *state = ledState[1];
      //Successful processing
      error = NO_ERROR;
   }
   if(address == 10000)
   {
      //Retrieve the state of the user button
      *state = BSP_PB_GetState(BUTTON_KEY);
      //Successful processing
      error = NO_ERROR;
   }
   else
   {
      //The register address is not acceptable
      error = ERROR_INVALID_ADDRESS;
   }

   //Return status code
   return error;
}


/**
 * @brief Set coil state callback function
 * @param[in] address Address of the coil
 * @param[in] state Desired coil state
 * @param[in] commit This flag indicates the current phase (validation phase
 *   or write phase if the validation was successful)
 * @return Error code
 **/

error_t modbusServerWriteCoilCallback(uint16_t address, bool_t state,
   bool_t commit)
{
   error_t error;

   //Check register address
   if(address == 0)
   {
      //Write phase?
      if(commit)
      {
         //Update the state of the LED
         if(state)
            BSP_LED_On(LED2);
         else
            BSP_LED_Off(LED2);

         //Save the state of the LED
         ledState[0] = state;
      }

      //Successful processing
      error = NO_ERROR;
   }
   else if(address == 1)
   {
      //Write phase?
      if(commit)
      {
         //Update the state of the LED
         if(state)
            BSP_LED_On(LED3);
         else
            BSP_LED_Off(LED3);

         //Save the state of the LED
         ledState[1] = state;
      }

      //Successful processing
      error = NO_ERROR;
   }
   else
   {
      //The register address is not acceptable
      error = ERROR_INVALID_ADDRESS;
   }

   //Return status code
   return error;
}


/**
 * @brief Get register value callback function
 * @param[in] address Register address
 * @param[out] state Current register value
 * @return Error code
 **/

error_t modbusServerReadRegCallback(uint16_t address, uint16_t *value)
{
   error_t error;

   //Check register address
   if(address == 30000)
   {
      //Retrieve the state of the user button
      *value = BSP_PB_GetState(BUTTON_KEY);
      //Successful processing
      error = NO_ERROR;
   }
   else if(address == 40000)
   {
      //Retrieve the state of the LED
      *value = ledState[0];
      //Successful processing
      error = NO_ERROR;
   }
   else if(address == 40001)
   {
      //Retrieve the state of the LED
      *value = ledState[1];
      //Successful processing
      error = NO_ERROR;
   }
   else
   {
      //The register address is not acceptable
      error = ERROR_INVALID_ADDRESS;
   }

   //Return status code
   return error;
}


/**
 * @brief Set register value callback function
 * @param[in] address Register address
 * @param[in] state Desired register value
 * @param[in] commit This flag indicates the current phase (validation phase
 *   or write phase if the validation was successful)
 * @return Error code
 **/

error_t modbusServerWriteRegCallback(uint16_t address, uint16_t value,
   bool_t commit)
{
   error_t error;

   //Check register address
   if(address == 40000)
   {
      //Write phase?
      if(commit)
      {
         //Update the state of the LED
         if(value & 1)
            BSP_LED_On(LED2);
         else
            BSP_LED_Off(LED2);

         //Save the state of the LED
         ledState[0] = value;
      }

      //Successful processing
      error = NO_ERROR;
   }
   else if(address == 40001)
   {
      //Write phase?
      if(commit)
      {
         //Update the state of the LED
         if(value & 1)
            BSP_LED_On(LED3);
         else
            BSP_LED_Off(LED3);

         //Save the state of the LED
         ledState[1] = value;
      }

      //Successful processing
      error = NO_ERROR;
   }
   else
   {
      //The register address is not acceptable
      error = ERROR_INVALID_ADDRESS;
   }

   //Return status code
   return error;
}
