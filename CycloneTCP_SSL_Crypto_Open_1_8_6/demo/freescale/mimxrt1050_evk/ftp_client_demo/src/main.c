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
#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "mimxrt1050_evk.h"
#include "os_port.h"
#include "core/net.h"
#include "drivers/mac/mimxrt1052_eth_driver.h"
#include "drivers/phy/ksz8081_driver.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "ftp/ftp_client.h"
#include "error.h"
#include "debug.h"

//Application configuration
#define APP_MAC_ADDR "00-AB-CD-EF-10-52"

#define APP_USE_DHCP ENABLED
#define APP_IPV4_HOST_ADDR "192.168.0.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.0.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::1052"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::1052"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

//Global variables
DhcpClientSettings dhcpClientSettings;
DhcpClientContext dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;


/**
 * @brief System clock configuration
 **/

void systemClockConfig(void)
{
   clock_arm_pll_config_t armPllConfig;
#ifndef SKIP_SYSCLK_INIT
   clock_sys_pll_config_t sysPllConfig;
#endif
#ifndef SKIP_USB_PLL_INIT
   clock_usb_pll_config_t usbPllConfig;
#endif

   //Set external XTAL oscillator frequency
   CLOCK_SetXtalFreq(24000000);
   CLOCK_SetRtcXtalFreq(32768);

   //Set PERIPH_CLK2 MUX to OSC
   CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
   //Set PERIPH_CLK MUX to PERIPH_CLK2
   CLOCK_SetMux(kCLOCK_PeriphMux, 1);

   //Set the value of the VDD_SOC to 1.5V
   DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(18);

   //Configure ARM PLL to 1200MHz
   armPllConfig.loopDivider = 100;
   CLOCK_InitArmPll(&armPllConfig);

#ifndef SKIP_SYSCLK_INIT
   //Configure SYS PLL to 528MHz
   sysPllConfig.loopDivider = 1;
   sysPllConfig.numerator = 0;
   sysPllConfig.denominator = 0;
   CLOCK_InitSysPll(&sysPllConfig);
#endif

#ifndef SKIP_USB_PLL_INIT
   //Configure USB1 PLL to 480MHz
   usbPllConfig.loopDivider = 0;
   CLOCK_InitUsb1Pll(&usbPllConfig);
#endif

   //Set ARM PODF divider to 2
   CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
   //Set AHB PODF divider to 1
   CLOCK_SetDiv(kCLOCK_AhbDiv, 0);
   //Set IPG PODF divider to 4
   CLOCK_SetDiv(kCLOCK_IpgDiv, 3);

   //Set PRE_PERIPH_CLK to PLL1 (1200MHz)
   CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);
   //Set PERIPH_CLK MUX to PRE_PERIPH_CLK
   CLOCK_SetMux(kCLOCK_PeriphMux, 0);

   //Disable unused peripheral clocks
   CCM->CCGR0 = 0x00C0000F;
   CCM->CCGR1 = 0x30000000;
   CCM->CCGR2 = 0xFF3F303F;
   CCM->CCGR3 = 0xF0000330;
   CCM->CCGR4 = 0x0000FF3C;
   CCM->CCGR5 = 0xF003330F;
   CCM->CCGR6 = 0x00FC0F00;

   //Power down all unused PLLs
   CLOCK_DeinitAudioPll();
   CLOCK_DeinitVideoPll();
   CLOCK_DeinitEnetPll();
   CLOCK_DeinitUsb2Pll();

   //Update system core clock
   SystemCoreClockUpdate();
}


/**
 * @brief MPU configuration
 **/

void mpuConfig(void)
{
   //Disable MPU
   ARM_MPU_Disable();

   //Region 0 setting
   MPU->RBAR = ARM_MPU_RBAR(0, 0xC0000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

   //Region 1 setting
   MPU->RBAR = ARM_MPU_RBAR(1, 0x80000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

   //Region 2 setting
   MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_512MB);

   //Region 3 setting
   MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

   //Region 4 setting
   MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

   //Region 5 setting
   MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

   //Region 6 setting
   MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

   //Region 7 setting (non-cacheable SRAM)
   MPU->RBAR = ARM_MPU_RBAR(7, 0x20238000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 1, 0, 0, 0, ARM_MPU_REGION_SIZE_32KB);

#if defined(SDRAM_MPU_INIT)
   //Region 8 setting
   MPU->RBAR = ARM_MPU_RBAR(8, 0x80000000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_32MB);

   //Region 9 setting
   MPU->RBAR = ARM_MPU_RBAR(9, 0x81E00000U);
   MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_2MB);
#endif

   //Enable MPU
   ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
}


/**
 * @brief I/O initialization
 **/

void ioInit(void)
{
   gpio_pin_config_t pinConfig;

   //Enable IOMUXC clock
   CLOCK_EnableClock(kCLOCK_Iomuxc);
   //Enable IOMUXC SNVS clock
   CLOCK_EnableClock(kCLOCK_IomuxcSnvs);

   //Configure GPIO_AD_B0_09 pin as GPIO1_IO09
   IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0);

   //Set GPIO_AD_B0_09 pad properties
   IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,
      IOMUXC_SW_PAD_CTL_PAD_HYS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(0) |
      IOMUXC_SW_PAD_CTL_PAD_PKE(0) |
      IOMUXC_SW_PAD_CTL_PAD_ODE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SPEED(0) |
      IOMUXC_SW_PAD_CTL_PAD_DSE(7) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

   //Configure WAKEUP pin as GPIO5_IO00
   IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00, 0);

   //Set WAKEUP pad properties
   IOMUXC_SetPinConfig(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,
      IOMUXC_SW_PAD_CTL_PAD_HYS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(3) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(1) |
      IOMUXC_SW_PAD_CTL_PAD_PKE(1) |
      IOMUXC_SW_PAD_CTL_PAD_ODE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SPEED(0) |
      IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

   //Configure user LED
   pinConfig.direction = kGPIO_DigitalOutput;
   pinConfig.outputLogic = 0;
   pinConfig.interruptMode = kGPIO_NoIntmode;
   GPIO_PinInit(USER_LED_GPIO, USER_LED_PIN, &pinConfig);

   //Configure user button
   pinConfig.direction = kGPIO_DigitalInput;
   pinConfig.outputLogic = 0;
   pinConfig.interruptMode = kGPIO_NoIntmode;
   GPIO_PinInit(USER_BUTTON_GPIO, USER_BUTTON_PIN, &pinConfig);
}


/**
 * @brief FTP client test routine
 * @return Error code
 **/

error_t ftpClientTest(void)
{
   error_t error;
   size_t length;
   IpAddr ipAddr;
   FtpClientContext ftpContext;
   static char_t buffer[256];

   //Debug message
   TRACE_INFO("\r\n\r\nResolving server name...\r\n");
   //Resolve FTP server name
   error = getHostByName(NULL, "ftp.gnu.org", &ipAddr, 0);

   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_INFO("Failed to resolve server name!\r\n");
      //Exit immediately
      return error;
   }

   //Debug message
   TRACE_INFO("Connecting to FTP server %s\r\n", ipAddrToString(&ipAddr, NULL));
   //Connect to the FTP server
   error = ftpConnect(&ftpContext, NULL, &ipAddr, 21, FTP_NO_SECURITY | FTP_PASSIVE_MODE);

   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_INFO("Failed to connect to FTP server!\r\n");
      //Exit immediately
      return error;
   }

   //Debug message
   TRACE_INFO("Successful connection\r\n");

   //Start of exception handling block
   do
   {
      //Login to the FTP server using the provided username and password
      error = ftpLogin(&ftpContext, "anonymous", "password", "");
      //Any error to report?
      if(error)
         break;

      //Open the specified file for reading
      error = ftpOpenFile(&ftpContext, "welcome.msg", FTP_FOR_READING | FTP_BINARY_TYPE);
      //Any error to report?
      if(error)
         break;

      //Dump the contents of the file
      while(1)
      {
         //Read data
         error = ftpReadFile(&ftpContext, buffer, sizeof(buffer) - 1, &length, 0);
         //End of file?
         if(error)
            break;

         //Properly terminate the string with a NULL character
         buffer[length] = '\0';
         //Dump current data
         TRACE_INFO("%s", buffer);
      }

      //End the string with a line feed
      TRACE_INFO("\r\n");
      //Close the file
      error = ftpCloseFile(&ftpContext);

      //End of exception handling block
   } while(0);

   //Close the connection
   ftpClose(&ftpContext);
   //Debug message
   TRACE_INFO("Connection closed...\r\n");

   //Return status code
   return error;
}


/**
 * @brief User task
 **/

void userTask(void *param)
{
   //Endless loop
   while(1)
   {
      //User button pressed?
      if(!GPIO_PinRead(USER_BUTTON_GPIO, USER_BUTTON_PIN))
      {
         //FTP client test routine
         ftpClientTest();

         //Wait for the user button to be released
         while(!GPIO_PinRead(USER_BUTTON_GPIO, USER_BUTTON_PIN));
      }

      //Loop delay
      osDelayTask(100);
   }
}


/**
 * @brief LED blinking task
 **/

void blinkTask(void *param)
{
   //Endless loop
   while(1)
   {
      //GPIO_PinWrite(USER_LED_GPIO, USER_LED_PIN, 0);
      osDelayTask(100);
      //GPIO_PinWrite(USER_LED_GPIO, USER_LED_PIN, 1);
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

   //Configure the system clock
   systemClockConfig();

   //Disable I-cache and D-cache
   SCB_DisableICache();
   SCB_DisableDCache();

   //MPU configuration
   mpuConfig();

   //Enable I-cache and D-cache
   SCB_EnableICache();
   SCB_EnableDCache();

   //Initialize kernel
   osInitKernel();
   //Configure debug UART
   debugInit(115200);

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("*** CycloneTCP FTP Client Demo ***\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("Copyright: 2010-2018 Oryx Embedded SARL\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: i.MX RT1052\r\n");
   TRACE_INFO("\r\n");

   //Configure I/Os
   ioInit();

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
   netSetHostname(interface, "FTPClientDemo");
   //Select the relevant network adapter
   netSetDriver(interface, &mimxrt1052EthDriver);
   netSetPhyDriver(interface, &ksz8081PhyDriver);
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

   //Create user task
   task = osCreateTask("User Task", userTask, NULL, 500, OS_TASK_PRIORITY_NORMAL);
   //Failed to create the task?
   if(task == OS_INVALID_HANDLE)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
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
