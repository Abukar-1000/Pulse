#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// Common settings used in Pico W examples
#define NO_SYS                      1
#define LWIP_SOCKET                 0
#define LWIP_NETCONN                0
#define LWIP_NETIF_STATUS_CALLBACK  1
#define LWIP_NETIF_LINK_CALLBACK    1

// Memory settings
#define MEM_LIBC_MALLOC             0
#define MEM_ALIGNMENT               4
#define MEM_SIZE                    4000
#define MEMP_NUM_TCP_PCB            5
#define MEMP_NUM_PBUF               16

// TCP settings - Required for your server
#define LWIP_TCP                    1
#define TCP_TTL                     255
#define TCP_QUEUE_OOSEQ             0
#define MEMP_NUM_TCP_SEG            32
#define TCP_MSS                     1460
#define TCP_WND                     (8 * TCP_MSS)
#define TCP_SND_BUF                 (8 * TCP_MSS)
#define TCP_SND_QUEUELEN            ((4 * (TCP_SND_BUF) + (TCP_MSS - 1)) / (TCP_MSS))

// UDP settings (Turn off if you absolutely don't use it, or leave 1 for DHCP)
#define LWIP_UDP                    1
#define LWIP_DHCP                   1

// Stats settings (Optional, set to 0 to save space)
#define LWIP_STATS                  0

#endif
