#ifndef PCBDeleter_h
#define PCBDeleter_h

namespace Connections::TCP
{
    struct PCBDeleter
    {
        void operator()(struct tcp_pcb* pcb) const
        {
            if (pcb != nullptr)
            {
                cyw43_arch_lwip_begin();
                
                if (tcp_close(pcb) != ERR_OK)
                {
                    tcp_abort(pcb);
                }

                cyw43_arch_lwip_end();
            }
        }
    };
};

#endif