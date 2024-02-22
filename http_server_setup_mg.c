/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : http_server_setup_mg.c
* Description  : The application project demonstrates the typical use of the NetX HTTP Server APIs. The Application
*                Project main thread entry initializes the NetX HTTP Server protocol and FileX using USB mass storage.
*                A user-callback function is entered when an HTTP request is made. The user-specified callback
*                function opens the requested file, reads the file data to the buffer, and sends the buffer data to
*                the client. The following table identifies the target versions for the associated software and
*                hardware used by the application project:
***********************************************************************************************************************/
#include <stdio.h>
#include "http_server_thread.h"
#include "fx_api.h"
#include "nx_http_server.h"
#include "nx_http.h"
#include "http_server_setup_mg.h"

extern FX_MEDIA * g_fx_media0_ptr;

CHAR            volume[32];
CHAR            my_buffer[1024];
UINT            status = SSP_SUCCESS;
ULONG           error_counter;
ULONG           actual_bytes = 1024;
FX_MEDIA        * p_media;
FX_FILE         my_file;


UINT my_authentication_check(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm)
{
    SSP_PARAMETER_NOT_USED(resource);
        SSP_PARAMETER_NOT_USED(request_type);
        SSP_PARAMETER_NOT_USED(server_ptr);
    #if AUTHENTICATION
      *name =  "yourname";
      *password = "yourpassword";
      *realm =  "index.html";
      return(NX_HTTP_BASIC_AUTHENTICATE);
    #else
      SSP_PARAMETER_NOT_USED(name);
      SSP_PARAMETER_NOT_USED(password);
      SSP_PARAMETER_NOT_USED(realm);
      return(NX_SUCCESS);
    #endif
}

/* Handle Client Requests in callback function */
UINT my_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
    status = SSP_SUCCESS;

    SSP_PARAMETER_NOT_USED(request_type);
    SSP_PARAMETER_NOT_USED(packet_ptr);

    memset(my_buffer, 0, sizeof(my_buffer)/sizeof(my_buffer[0]));

    /* return requested resource or query */
    if(strcmp((const char*)resource,(const char*)"/index.html")==0)
    {
        status = fx_file_open(p_media, &my_file,(CHAR *)"index.html", FX_OPEN_FOR_READ);
        if (status != FX_SUCCESS)
        {
            tx_thread_sleep(TX_WAIT_FOREVER);
        }
        status = fx_file_read(&my_file,(CHAR *) my_buffer, 1024, &actual_bytes);
        if (status != FX_SUCCESS)
        {

            tx_thread_sleep(TX_WAIT_FOREVER);
        }
        nx_http_server_callback_data_send(server_ptr,my_buffer,1024);
        memset(my_buffer, 0, sizeof(my_buffer)/sizeof(my_buffer[0]));
        status = fx_file_close(&my_file);
        return(NX_HTTP_CALLBACK_COMPLETED);
    }
return(NX_SUCCESS);
}

/* The application project demonstrates the typical use of the NetX HTTP Server APIs.
 * The Application Project main thread entry initializes the NetX HTTP Server protocol
 * and FileX using USB mass storage. A user-callback function is entered when an HTTP
 * request is made. The user-specified callback function opens the requested file,
 * reads the file data to the buffer, and sends the buffer data to the client. */
void http_server_entry()
{
    #ifdef SEMI_HOSTING
    #ifdef __GNUC__
        /* Initialize semi hosting */
        initialise_monitor_handles();
    #endif
    #endif


    /* Functions result */
    ssp_err_t err;

    /* leds structure */
    bsp_leds_t leds;

    p_media = g_fx_media0_ptr;

    /* Populate the leds structure array to simplify the use of the LEDs on the board */
    err = R_BSP_LedsGet(&leds);
    g_ioport.p_api->pinWrite(leds.p_leds[0],IOPORT_LEVEL_HIGH);
    g_ioport.p_api->pinWrite(leds.p_leds[1],IOPORT_LEVEL_HIGH);
    g_ioport.p_api->pinWrite(leds.p_leds[2],IOPORT_LEVEL_HIGH);
    if (SSP_SUCCESS != err)
    {
        /* This is a fairly simple error handling - it holds the
        application execution. In a more realistic scenarios
        a more robust and complex error handling solution should
        be provided. */
        #ifdef SEMI_HOSTING
            if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
            {
                /* Debugger is connected */
                /* Call this before any calls to printf() */
                printf("Could not get board LED info, error:%d\n",err);
            }
        #endif
            tx_thread_sleep(TX_WAIT_FOREVER);
    }

    status = fx_media_volume_get(p_media, volume, FX_DIRECTORY_SECTOR);
    if (status != FX_SUCCESS)
    {
        #ifdef SEMI_HOSTING
                printf("> fx_media_vol_get status %d\r\n",status);
        #endif
        tx_thread_sleep(TX_WAIT_FOREVER);
    }
    else
    {
        #ifdef SEMI_HOSTING
                printf("> USB drive mounted.\r\n");
        #endif
        g_ioport.p_api->pinWrite(leds.p_leds[0],IOPORT_LEVEL_LOW);
    }

    status =  fx_directory_default_set(p_media, FX_NULL);
    if (status != FX_SUCCESS && status != FX_NOT_FOUND)
    {
        #ifdef SEMI_HOSTING
                printf("> Failed to set the default directory \n");
        #endif
        tx_thread_sleep(TX_WAIT_FOREVER);
    }
    else
    {
        #ifdef SEMI_HOSTING
                printf("> Successfully able to set the directory \n");
        #endif
    }
    /* Start HTTP Server */
     status =  nx_http_server_start(&g_http_server0);
     if (status !=  NX_SUCCESS)
     {
        #ifdef SEMI_HOSTING
                printf("> Failed to start HTTP server \n");
        #endif
        tx_thread_sleep(TX_WAIT_FOREVER);
     }
     else
     {
        #ifdef SEMI_HOSTING
                printf("> HTTP server started \n");
        #endif
        g_ioport.p_api->pinWrite(leds.p_leds[2],IOPORT_LEVEL_LOW);
     }

}
