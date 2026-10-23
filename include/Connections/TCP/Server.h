#ifndef Server_TCP_h
#define Server_TCP_h
#include <string>
#include "lwip/tcp.h"
#include "../IConnection.h"
#include "pico/cyw43_arch.h"
#include "./Resources/PCBDeleter.h"
#include "../../Animations/ConnectedToWifi.h"
#include "../../Config/Config.h"

namespace Connections::TCP
{  
    class Server: public IConnection
    {
        private:
            uint16_t port;
            std::unique_ptr<struct tcp_pcb, PCBDeleter> pcb;
            bool isListening;
            static void OnError(void *arg, err_t err);
            static err_t OnAccept(void *arg, struct tcp_pcb *client_pcb, err_t err);
            static err_t OnReceive(void *arg, struct tcp_pcb *client_pcb, struct pbuf *p, err_t err);
            static void FreeBuffer(struct pbuf *p);
            void Error(err_t err);
            err_t Accept(struct tcp_pcb *client_pcb, err_t err);
            err_t Receive(struct tcp_pcb *client_pcb, struct pbuf *p, err_t err);
        public:
            ~Server();
            Server(void);
            virtual void Start(void) override;
            virtual void Stop(void) override;
            bool IsListening(void) const noexcept;
            bool IsConnected(void) const noexcept;
    };
}
#endif