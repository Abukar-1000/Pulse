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
            uint16_t totalLen = p->tot_len;
            std::string receivedData;
            receivedData.reserve(p->tot_len);

            struct pbuf* q = p; 
            while (q != nullptr)
            {
                // Process the received data here
                receivedData.append(static_cast<char*>(q->payload), q->len);
                q = q->next;
            }

            tcp_recved(client_pcb, totalLen);
            this->hasActiveRequests = true;
            this->SendResponse("Acknowledged Request: " + receivedData, client_pcb);

            tcp_arg(client_pcb, nullptr);
            tcp_recv(client_pcb, nullptr);
            
            err_t closeErr = tcp_close(client_pcb);
            if (closeErr != ERR_OK) {
                tcp_abort(client_pcb);
            }
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

    bool Server::SendResponse(const std::string& response, struct tcp_pcb *client_pcb) noexcept
    {
        cyw43_arch_lwip_begin();
        uint16_t availableBufferSpace = tcp_sndbuf(client_pcb);
        bool responseExceedsLimit = response.length() > availableBufferSpace;
        std::string msg = (
            responseExceedsLimit? 
                response.substr(0, availableBufferSpace)
            :
                response
        ); 

        err_t writeErr = tcp_write(
            client_pcb, 
            msg.c_str(), 
            msg.length(), 
            TCP_WRITE_FLAG_COPY
        );

        if (writeErr == ERR_OK)
        {
            tcp_output(client_pcb);
        }
        
        cyw43_arch_lwip_end();
        return (writeErr == ERR_OK); 
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
    :   port(8080),
        pcb(nullptr),
        isListening(false),
        hasActiveRequests(false),
        recAnimation(Animations::RequestReceived(10))
    {}

    void Server::ProcessActiveEvents(void) noexcept
    {
        if (this->hasActiveRequests)
        {
            this->hasActiveRequests = false;
            this->recAnimation.Run();
        }
    }

    bool Server::ConnectedToWifi(void) noexcept
    {
        if(cyw43_arch_wifi_connect_timeout_ms(
            Config::WIFI_SSID.c_str(),
            Config::WIFI_PASSWORD.c_str(),
            CYW43_AUTH_WPA2_AES_PSK,
            10000
        ))
        {
            // failed to connect to wifi
            Animations::FailedToConnect animation;
            animation.Run();
            this->isListening = false;
            return false;
        }

        Animations::ConnectedToWifi animation;
        animation.Run();
        return true;
    }

    void Server::AttemptReconnectToWifi(uint16_t maxAttempts, bool infinite) noexcept
    {
        uint16_t delay = 10;
        bool connected = false;
        if (infinite)
        {
            while(!connected)
            {
                connected = this->ConnectedToWifi();
                sleep_ms(delay);
            }
            return;
        }

        uint16_t count = 0;
        while (!connected && count < maxAttempts)
        {
            connected = this->ConnectedToWifi();
            count++;
            sleep_ms(delay);
        }
    }
    
    void Server::Start(void)
    {
        Config config;
        cyw43_arch_enable_sta_mode();
        this->AttemptReconnectToWifi(0, true);

        cyw43_arch_lwip_begin();
        struct tcp_pcb* rawPcbPtr = tcp_new_ip_type(IPADDR_TYPE_ANY);
        if (rawPcbPtr == nullptr)
        {
            // failed to create new TCP PCB
            cyw43_arch_lwip_end();
            return;
        }

        
        err_t bindErr = tcp_bind(rawPcbPtr, IP_ANY_TYPE, this->port);
        if (bindErr != ERR_OK) {
            // failed to bind
            tcp_abort(rawPcbPtr);
            cyw43_arch_lwip_end();
            return;
        }

        struct tcp_pcb* listeningPcbPtr = tcp_listen_with_backlog(rawPcbPtr, 1);
        if (listeningPcbPtr == nullptr) {
            // failed to listen
            cyw43_arch_lwip_end();
            return;
        }
        
        this->pcb = std::unique_ptr<struct tcp_pcb, PCBDeleter>(listeningPcbPtr, PCBDeleter());
        if (this->pcb != nullptr)
        {
            this->isListening = true;
        }

        tcp_arg(this->pcb.get(), this);
        tcp_accept(this->pcb.get(), Server::OnAccept);
        cyw43_arch_lwip_end();
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
    {}
}