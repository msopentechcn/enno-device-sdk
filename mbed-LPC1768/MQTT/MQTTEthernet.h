
#if !defined(MQTTETHERNET_H)
#define MQTTETHERNET_H

#include "MQTTmbed.h"
#include "EthernetInterface.h"
#include "MQTTSocket.h"

class MQTTEthernet : public MQTTSocket
{
public:    
    MQTTEthernet()
    {
  //      eth.init();                          // revised by v-chawei to adapt enno SDK
  //      eth.connect();
    }
    
    EthernetInterface& getEth()
    {
        return eth;
    }
    void start(){
        eth.init();                          // Use DHCP  dedined by v-chawei to adapt enno SDK
        eth.connect();
    }
    void reconnect()
    {
        eth.connect();  // nothing I've tried actually works to reconnect 
    }
    
private:

    EthernetInterface eth;
    
};


#endif
