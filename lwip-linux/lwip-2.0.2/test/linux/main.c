/*
 * main.c
 *
 *  Created on: Jul 18, 2017
 *      Author: haohd
 *
 *  Copyright (C) 2017 miniHome
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library.
 *  If not, see <http://www.gnu.org/licenses/>.
 */
#include "lwip.h"
#if 0
int main(void)
{
  pthread_t thread;

	if(net_init("wlxe4fac4521afb") != ERR_OK)   //The interface name should be passed to this function
	{
		printf("Failed to initialize netif!\n");
		goto _EXIT;
	}

	thread = start_netif();
	if (thread < 0)
	{
    printf("Failed to start netif!\n");
    goto _EXIT;
	}

#if TEST_ID == ECHO_SERVER
  if (create_echo_server() != ERR_OK)
  {
    printf("Failed to create echo server!\n");
    goto _EXIT;
  }
#elif TEST_ID == TCP_CLIENT
  ip_addr_t ip_addr = { TCP_REMOTE_SERVER_ADDR };
  if (tcp_client(&ip_addr, TCP_REMOTE_SERVER_PORT) != ERR_OK)
  {
    printf("Failed to connect to server!\n");
    goto _EXIT;
  }
#endif


	pthread_join(thread, NULL);
_EXIT:
  net_quit();
	return 1;
}
#endif

#if 1
#include <stdio.h>
#include <stdlib.h>
#include "lwip/tcp.h"
#include "lwip/err.h"
#include "lwip/dns.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/netdb.h"

static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *arg);

static err_t http_data_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  if (p == NULL)
  {
    // Connection closed by the server
    tcp_close(tpcb);
    return ERR_OK;
  }

  // Print the received data (raw data)
  printf("Received data: %.*s", p->len, (char *)p->payload);

  // Free the buffer and acknowledge that we've received the data
  tcp_recved(tpcb, p->tot_len);
  pbuf_free(p);

  return ERR_OK;
}

static err_t http_connected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
  if (err == ERR_OK)
  {
    // Prepare HTTP GET request
    const char *request = "GET / HTTP/1.1\r\nHost: example.com\r\nConnection: close\r\n\r\n";

    // Send the HTTP request
    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);

    // Set the data reception callback
    tcp_recv(tpcb, http_data_recv);
  }
  else
  {
    printf("Connection failed\n");
    tcp_close(tpcb);
  }

  return err;
}

static void http_connection_err(void *arg, err_t err)
{
  printf("Connection error: %d\n", err);
}

void http_request(const char *hostname, const char *path)
{
  struct tcp_pcb *pcb;
  ip_addr_t server_ip;
  //struct addrinfo hints;
  //struct  addrinfo *res;

  

  // Create a new TCP PCB (Protocol Control Block)
  pcb = tcp_new();
  if (pcb == NULL)
  {
    printf("Failed to create TCP PCB\n");
    return;
  }

  // Define a callback for error handling
  tcp_err(pcb, http_connection_err);

  // DNS lookup (non-blocking, it will call back with IP address)
  err_t err = dns_gethostbyname(hostname, &server_ip, dns_callback, pcb);
  if (err == ERR_OK)
  {
    // DNS lookup success, IP address already available
    printf("IP address found, connecting to server...\n");
    tcp_connect(pcb, &server_ip, 80, http_connected);
  }
  else if (err == ERR_INPROGRESS)
  {
    // DNS lookup is in progress, the callback will be triggered later
    printf("DNS lookup in progress...\n");
  }
  else
  {
    printf("DNS lookup failed\n");
    tcp_close(pcb);
  }
}

// DNS Callback function
static void dns_callback(const char *name, const ip_addr_t *ipaddr, void *arg)
{
  struct tcp_pcb *pcb = (struct tcp_pcb *)arg;

  if (ipaddr != NULL)
  {
    printf("DNS resolved, IP: %s\n", ipaddr_ntoa(ipaddr));

    // Connect to the server after DNS resolution
    tcp_connect(pcb, ipaddr, 80, http_connected);
  }
  else
  {
    printf("DNS resolution failed\n");
    tcp_close(pcb);
  }
}

int main(void)
{

  pthread_t thread;
   // Initialize the lwIP stack
  lwip_init();

  if (net_init("wlxe4fac4521afb") != ERR_OK) // The interface name should be passed to this function
  {
    printf("Failed to initialize netif!\n");
    goto _EXIT;
  }

  thread = start_netif();
  if (thread < 0)
  {
    printf("Failed to start netif!\n");
    goto _EXIT;
  }

 

  // Set up network interfaces, etc. (You need to set up IP addressing)
  // struct netif netif;
  // ip4_addr_t ipaddr, netmask, gw;

  // IP4_ADDR(&ipaddr, 192, 168, 134, 83);    // Set a static IP for your device
  // IP4_ADDR(&netmask, 255, 255, 255, 0); // Subnet mask
  // IP4_ADDR(&gw, 192, 168, 134, 254);        // Gateway address

  // netif_add(&netif, &ipaddr, &netmask, &gw, NULL, netif_init, netif_input);
  // netif_set_default(&netif);
  // netif_set_up(&netif);

  // Send an HTTP GET request
  http_request("google.com", "/");

  // Main loop to process timeouts and received packets
  while (1)
  {
    sys_check_timeouts();
    // Call netif poll function if necessary
  }

  pthread_join(thread, NULL);
_EXIT:
  net_quit();
  return 1;

  return 0;
}

#endif
