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
#ifndef HTTP_SERVER_SETUP_MG_H_
#define HTTP_SERVER_SETUP_MG_H_

/* Uncomment to turnon Semi-Hosting */
#define SEMI_HOSTING

#ifdef SEMI_HOSTING
#include <stdio.h>
#ifdef __GNUC__
extern void initialise_monitor_handles(void);
#endif
#endif

#define AUTHENTICATION  0
#define htmlwrite(p,s)  (nx_packet_data_append(p,s,(ULONG)strlen((const char*)s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER))
#define htmlresponse "HTTP/1.0 200 \r\nContent-Type: text/html\r\n\r\n"
#define htmltag "<HTML>"
#define htmlendtag "</HTML>"
#define sysvars_anchor_tag "<I> <a href=\"/\" > System Variables </a> </I>"
#define bodytag "<body bgcolor=\"#FFFF00\" >"
#define bodyendtag "</body>"
#define hrline "<HR SIZE=6 WIDTH=\"90%\" NOSHADE COLOR=\"black\"  >"
#define titleline "<HEAD><TITLE>NetX HTTP Server</TITLE></HEAD>\r\n"
#define h1line " <H1 ALIGN=\"center\"> <img src=\"/nxlogo.gif\"> NetX HTTP Server Demonstration <img src=\"/txlogo.gif\"></H1>\r\n"
#define tabletag "<TABLE BORDER=\"1\" ALIGN=\"center\" >"
#define tableendtag "</TABLE>"
#define captiontag "<CAPTION ALIGN=\"top\" >"
#define captionendtag "</CAPTION>"
#define trtag "<TR>"
#define trendtag "</TR>"
#define tdtag "<TD>"
#define tdendtag "</TD>"
#define anchortag "<A href=\"/test.htm?Display="
#define gtchar "\">"
#define anchorendtag "</A>"

UINT my_authentication_check(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm);

UINT my_request_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);

void http_server_entry();

#endif /* HTTP_SERVER_SETUP_MG_H_ */
