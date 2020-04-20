
#include <stdio.h>
#include <stdbool.h>
#include "gps.h"
#define TAG "GPS"



//Configure a port to output UBX, NMEA, RTCM3 or a combination thereof
bool setI2COutput(uint8_t comSettings, uint16_t maxWait){

    return (setPortOutput(COM_PORT_I2C, comSettings, maxWait));

}

//Configure a given port to output UBX, NMEA, RTCM3 or a combination thereof
//Port 0=I2c, 1=UART1, 2=UART2, 3=USB, 4=SPI
//Bit:0 = UBX, :1=NMEA, :5=RTCM3
bool setPortOutput(uint8_t portID, uint8_t outStreamSettings, uint16_t maxWait)
{
  //Get the current config values for this port ID
  if (getPortSettings(portID, maxWait) == false)
    return (false); //Something went wrong. Bail.

  if (commandAck != UBX_ACK_ACK)
  {
    if (_printDebug == true)
    {
       ESP_LOGE(TAG, "setPortOutput failed to ACK");
    }
    return (false);
  }

  packetCfg.cls = UBX_CLASS_CFG;
  packetCfg.id = UBX_CFG_PRT;
  packetCfg.len = 20;
  packetCfg.startingSpot = 0;

  //payloadCfg is now loaded with current bytes. Change only the ones we need to
  payloadCfg[14] = outStreamSettings; //OutProtocolMask LSB - Set outStream bits

  return ((sendCommand(packetCfg, maxWait)) == SFE_UBLOX_STATUS_DATA_SENT); // We are only expecting an ACK
}




//Given a message, calc and store the two byte "8-Bit Fletcher" checksum over the entirety of the message
//This is called before we send a command message
void calcChecksum(ubxPacket *msg)
{
  msg->checksumA = 0;
  msg->checksumB = 0;

  msg->checksumA += msg->cls;
  msg->checksumB += msg->checksumA;

  msg->checksumA += msg->id;
  msg->checksumB += msg->checksumA;

  msg->checksumA += (msg->len & 0xFF);
  msg->checksumB += msg->checksumA;

  msg->checksumA += (msg->len >> 8);
  msg->checksumB += msg->checksumA;

  for (uint16_t i = 0; i < msg->len; i++)
  {
    msg->checksumA += msg->payload[i];
    msg->checksumB += msg->checksumA;
  }
}