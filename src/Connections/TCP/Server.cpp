#include "../../../include/Connections/TCP/Server.h"

namespace Connections::TCP
{
    err_t Server::OnAccept(void *arg, struct tcp_pcb *client_pcb, err_t err)
    {
        Server* server = static_cast<Server*>(arg);
        if (server != nullptr)
        {
            return server->Accept(client_pcb, err);
        }

        return ERR_ARG;
    }
    
    err_t Server::OnReceive(void *arg, struct tcp_pcb *client_pcb, struct pbuf *p, err_t err)
    {
        Server* server = static_cast<Server*>(arg);
        if (server != nullptr)
        {
            return server->Receive(client_pcb, p, err);
        }

        Server::FreeBuffer(p);
        return ERR_ARG;
    }

    void Server::OnError(void *arg, err_t err)
    {
        Server* server = static_cast<Server*>(arg);
        if (server != nullptr)
        {
            server->Error(err);
        }
    }

    err_t Server::Accept(struct tcp_pcb *client_pcb, err_t err)
    {
        if (err != ERR_OK || client_pcb == nullptr)
        {
            return ERR_VAL;
        }

        // Client Connected Animation here 
        tcp_arg(client_pcb, this);
        tcp_recv(client_pcb, Server::OnReceive);
        return ERR_OK;
    }

    err_t Server::Receive(struct tcp_pcb *client_pcb, struct pbuf *p, err_t err)
    {
        if (p == nullptr)
        {
            tcp_close(client_pcb);
            return ERR_OK;
        }

        if (err == ERR_OK && p != nullptr && p->tot_len > 0)
        {
            std::string receivedData;
            receivedData.reserve(p->tot_len);
            while (p != nullptr)
            {
                // Process the received data here
                receivedData.append(static_cast<char*>(p->payload), p->len);
                p = p->next;
            }

            tcp_recved(client_pcb, p->tot_len);
        }

        Server::FreeBuffer(p);
        return ERR_OK;
    }

    void Server::Error(err_t err)
    {
        printf("Error: %d\n", err);
        switch (err) 
        {
            case ERR_RST:
                printf("Connection reset by peer.\n");
                break;
            case ERR_CLSD:
                printf("Connection closed.\n");
                break;
            case ERR_TIMEOUT:
                printf("Connection timed out.\n");
                break;
            default:
                printf("Generic network error occurred.\n");
                break;
        }
    }

    void Server::FreeBuffer(struct pbuf *p)
    {
        if (p != nullptr)
        {
            pbuf_free(p);
        }
    }

    bool Server::IsListening(void) const noexcept
    {
        return this->pcb != nullptr && this->isListening;
    }

    bool Server::IsConnected(void) const noexcept
    {
        cyw43_thread_enter();
        int status = cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA);
        cyw43_thread_exit();
        return status == CYW43_LINK_UP;
    }

    Server::Server()
    :   port(4242),
        pcb(nullptr),
        isListening(false)
    {
        this->Start();
    }
    
    void Server::Start(void)
    {
        Config config;
        cyw43_arch_enable_sta_mode();

        if(cyw43_arch_wifi_connect_timeout_ms(
            Config::WIFI_SSID.c_str(),
            Config::WIFI_PASSWORD.c_str(),
            CYW43_AUTH_WPA2_AES_PSK,
            10000
        ))
        {
            // failed to connect to wifi
            return;
        }

        Animations::ConnectedToWifi animation;
        animation.Run();
        
        cyw43_thread_enter();
        struct tcp_pcb* rawPcbPtr = tcp_new_ip_type(IPADDR_TYPE_ANY);
        if (rawPcbPtr == nullptr)
        {
            // failed to create new TCP PCB
            cyw43_thread_exit();
            return;
        }

        this->pcb = std::unique_ptr<struct tcp_pcb, PCBDeleter>(rawPcbPtr, PCBDeleter());
        tcp_bind(this->pcb.get(), IP_ANY_TYPE, this->port);
        tcp_listen_with_backlog(this->pcb.get(), 1);

        if (this->pcb != nullptr)
        {
            this->isListening = true;
        }

        tcp_arg(this->pcb.get(), this);
        tcp_accept(this->pcb.get(), Server::OnAccept);
        cyw43_thread_exit();
    }

    void Server::Stop(void)
    {
        if (this->pcb == nullptr)
        {
            return;
        }
        
        cyw43_arch_lwip_begin();
        tcp_accept(this->pcb.get(), nullptr);
        err_t err = tcp_close(this->pcb.get());
        cyw43_arch_lwip_end();
        
        if (err == ERR_OK)
        {
            this->isListening = false;
            this->pcb.reset();
            return;
        }

        cyw43_arch_lwip_begin();
        tcp_abort(this->pcb.get());
        cyw43_arch_lwip_end();
        this->isListening = false;
        this->pcb.reset();
    }

    Server::~Server()
    {
        this->Stop();
    }
}