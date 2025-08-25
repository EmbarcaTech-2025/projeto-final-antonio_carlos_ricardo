#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

/* Sistema com RTOS (threadsafe background) */
#define NO_SYS                         1
#define SYS_LIGHTWEIGHT_PROT           0

/* IPv4 (simples); IPv6 opcional */
#define LWIP_IPV4                      1
#define LWIP_IPV6                      0

/* Rede básica */
#define LWIP_DHCP                      1
#define LWIP_DNS                       1

/* Sockets (usamos lwIP sockets no Pico W) */
#define LWIP_SOCKET                    0
#define LWIP_NETCONN                   0

/* TCP/UDP */
#define LWIP_TCP                       1
#define LWIP_UDP                       1

/* getaddrinfo()/getnameinfo() */
#define LWIP_HAVE_GETADDRINFO          1

/* Diversos */
#define MEM_ALIGNMENT                  4
#define LWIP_TIMEVAL_PRIVATE           0
#define LWIP_STATS                     0

/* Opcional: logs do lwIP (desative em produção) */
// #define LWIP_DEBUG                   1
// #define IP_DEBUG                     LWIP_DBG_ON
// #define NETIF_DEBUG                  LWIP_DBG_ON
// #define DHCP_DEBUG                   LWIP_DBG_ON
// #define DNS_DEBUG                    LWIP_DBG_ON
// #define TCP_DEBUG                    LWIP_DBG_ON
// #define SOCKETS_DEBUG                LWIP_DBG_ON

/* Se for habilitar SNTP futuramente:
   #define LWIP_SNTP                   1
   (e inicializar o SNTP no seu código)
*/

#endif /* _LWIPOPTS_H */
