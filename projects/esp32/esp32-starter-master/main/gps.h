#ifndef _GPS_H_
#define _GPS_H_

#include <stdio.h>

bool  _printDebug = false; //Flag to print the serial commands we are sending to the Serial port for debug


#define MAX_PAYLOAD_SIZE 256 //We need ~220 bytes for getProtocolVersion on most ublox modules

uint8_t _gpsI2Caddress = 0x42; //Default 7-bit unshifted address of the ublox 6/7/8/M8/F9 series


// ubxPacket validity
typedef enum
{
  SFE_UBLOX_PACKET_VALIDITY_NOT_VALID,
  SFE_UBLOX_PACKET_VALIDITY_VALID,
  SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED
} sfe_ublox_packet_validity_e;


//-=-=-=-=- UBX binary specific variables
typedef struct ubxPacket
{
	uint8_t cls;
	uint8_t id;
	uint16_t len;		   //Length of the payload. Does not include cls, id, or checksum bytes
	uint16_t counter;	  //Keeps track of number of overall bytes received. Some responses are larger than 255 bytes.
	uint16_t startingSpot; //The counter value needed to go past before we begin recording into payload array
	uint8_t *payload;
	uint8_t checksumA; //Given to us from module. Checked against the rolling calculated A/B checksums.
	uint8_t checksumB;
	sfe_ublox_packet_validity_e valid; //Goes from NOT_DEFINED to VALID or NOT_VALID when checksum is checked
} ubxPacket;

// Global Status Returns
typedef enum
{
	SFE_UBLOX_STATUS_SUCCESS,
	SFE_UBLOX_STATUS_FAIL,
	SFE_UBLOX_STATUS_CRC_FAIL,
	SFE_UBLOX_STATUS_TIMEOUT,
	SFE_UBLOX_STATUS_COMMAND_UNKNOWN,
	SFE_UBLOX_STATUS_OUT_OF_RANGE,
	SFE_UBLOX_STATUS_INVALID_ARG,
	SFE_UBLOX_STATUS_INVALID_OPERATION,
	SFE_UBLOX_STATUS_MEM_ERR,
	SFE_UBLOX_STATUS_HW_ERR,
	SFE_UBLOX_STATUS_DATA_SENT,
	SFE_UBLOX_STATUS_DATA_RECEIVED,
	SFE_UBLOX_STATUS_I2C_COMM_FAILURE,
} sfe_ublox_status_e;
/////////////////////////////////// REGISTERS /////////////////////////////////////////

// getPVT will only return data once in each navigation cycle. By default, that is once per second.
// Therefore we should set getPVTmaxWait to slightly longer than that.
// If you change the navigation frequency to (e.g.) 4Hz using setNavigationFrequency(4)
// then you should use a shorter maxWait for getPVT. 300msec would be about right: getPVT(300)
// The same is true for getHPPOSLLH.
#define getPVTmaxWait 1100		// Default maxWait for getPVT and all functions which call it
#define getHPPOSLLHmaxWait 1100 // Default maxWait for getHPPOSLLH and all functions which call it


//Registers
const uint8_t UBX_SYNCH_1 = 0xB5;
const uint8_t UBX_SYNCH_2 = 0x62;

//The following are UBX Class IDs. Descriptions taken from ZED-F9P Interface Description Document page 32, NEO-M8P Interface Description page 145
const uint8_t UBX_CLASS_NAV = 0x01;  //Navigation Results Messages: Position, Speed, Time, Acceleration, Heading, DOP, SVs used
const uint8_t UBX_CLASS_RXM = 0x02;  //Receiver Manager Messages: Satellite Status, RTC Status
const uint8_t UBX_CLASS_INF = 0x04;  //Information Messages: Printf-Style Messages, with IDs such as Error, Warning, Notice
const uint8_t UBX_CLASS_ACK = 0x05;  //Ack/Nak Messages: Acknowledge or Reject messages to UBX-CFG input messages
const uint8_t UBX_CLASS_CFG = 0x06;  //Configuration Input Messages: Configure the receiver.
const uint8_t UBX_CLASS_UPD = 0x09;  //Firmware Update Messages: Memory/Flash erase/write, Reboot, Flash identification, etc.
const uint8_t UBX_CLASS_MON = 0x0A;  //Monitoring Messages: Communication Status, CPU Load, Stack Usage, Task Status
const uint8_t UBX_CLASS_AID = 0x0B;  //(NEO-M8P ONLY!!!) AssistNow Aiding Messages: Ephemeris, Almanac, other A-GPS data input
const uint8_t UBX_CLASS_TIM = 0x0D;  //Timing Messages: Time Pulse Output, Time Mark Results
const uint8_t UBX_CLASS_ESF = 0x10;  //(NEO-M8P ONLY!!!) External Sensor Fusion Messages: External Sensor Measurements and Status Information
const uint8_t UBX_CLASS_MGA = 0x13;  //Multiple GNSS Assistance Messages: Assistance data for various GNSS
const uint8_t UBX_CLASS_LOG = 0x21;  //Logging Messages: Log creation, deletion, info and retrieval
const uint8_t UBX_CLASS_SEC = 0x27;  //Security Feature Messages
const uint8_t UBX_CLASS_HNR = 0x28;  //(NEO-M8P ONLY!!!) High Rate Navigation Results Messages: High rate time, position speed, heading
const uint8_t UBX_CLASS_NMEA = 0xF0; //NMEA Strings: standard NMEA strings

//The following are used for configuration. Descriptions are from the ZED-F9P Interface Description pg 33-34 and NEO-M9N Interface Description pg 47-48
const uint8_t UBX_CFG_ANT = 0x13;		//Antenna Control Settings. Used to configure the antenna control settings
const uint8_t UBX_CFG_BATCH = 0x93;		//Get/set data batching configuration.
const uint8_t UBX_CFG_CFG = 0x09;		//Clear, Save, and Load Configurations. Used to save current configuration
const uint8_t UBX_CFG_DAT = 0x06;		//Set User-defined Datum or The currently defined Datum
const uint8_t UBX_CFG_DGNSS = 0x70;		//DGNSS configuration
const uint8_t UBX_CFG_GEOFENCE = 0x69;  //Geofencing configuration. Used to configure a geofence
const uint8_t UBX_CFG_GNSS = 0x3E;		//GNSS system configuration
const uint8_t UBX_CFG_INF = 0x02;		//Depending on packet length, either: poll configuration for one protocol, or information message configuration
const uint8_t UBX_CFG_ITFM = 0x39;		//Jamming/Interference Monitor configuration
const uint8_t UBX_CFG_LOGFILTER = 0x47; //Data Logger Configuration
const uint8_t UBX_CFG_MSG = 0x01;		//Poll a message configuration, or Set Message Rate(s), or Set Message Rate
const uint8_t UBX_CFG_NAV5 = 0x24;		//Navigation Engine Settings. Used to configure the navigation engine including the dynamic model.
const uint8_t UBX_CFG_NAVX5 = 0x23;		//Navigation Engine Expert Settings
const uint8_t UBX_CFG_NMEA = 0x17;		//Extended NMEA protocol configuration V1
const uint8_t UBX_CFG_ODO = 0x1E;		//Odometer, Low-speed COG Engine Settings
const uint8_t UBX_CFG_PM2 = 0x3B;		//Extended power management configuration
const uint8_t UBX_CFG_PMS = 0x86;		//Power mode setup
const uint8_t UBX_CFG_PRT = 0x00;		//Used to configure port specifics. Polls the configuration for one I/O Port, or Port configuration for UART ports, or Port configuration for USB port, or Port configuration for SPI port, or Port configuration for DDC port
const uint8_t UBX_CFG_PWR = 0x57;		//Put receiver in a defined power state
const uint8_t UBX_CFG_RATE = 0x08;		//Navigation/Measurement Rate Settings. Used to set port baud rates.
const uint8_t UBX_CFG_RINV = 0x34;		//Contents of Remote Inventory
const uint8_t UBX_CFG_RST = 0x04;		//Reset Receiver / Clear Backup Data Structures. Used to reset device.
const uint8_t UBX_CFG_RXM = 0x11;		//RXM configuration
const uint8_t UBX_CFG_SBAS = 0x16;		//SBAS configuration
const uint8_t UBX_CFG_TMODE3 = 0x71;	//Time Mode Settings 3. Used to enable Survey In Mode
const uint8_t UBX_CFG_TP5 = 0x31;		//Time Pulse Parameters
const uint8_t UBX_CFG_USB = 0x1B;		//USB Configuration
const uint8_t UBX_CFG_VALDEL = 0x8C;	//Used for config of higher version Ublox modules (ie protocol v27 and above). Deletes values corresponding to provided keys/ provided keys with a transaction
const uint8_t UBX_CFG_VALGET = 0x8B;	//Used for config of higher version Ublox modules (ie protocol v27 and above). Configuration Items
const uint8_t UBX_CFG_VALSET = 0x8A;	//Used for config of higher version Ublox modules (ie protocol v27 and above). Sets values corresponding to provided key-value pairs/ provided key-value pairs within a transaction.

//The following are used to enable NMEA messages. Descriptions come from the NMEA messages overview in the ZED-F9P Interface Description
const uint8_t UBX_NMEA_MSB = 0xF0;  //All NMEA enable commands have 0xF0 as MSB
const uint8_t UBX_NMEA_DTM = 0x0A;  //GxDTM (datum reference)
const uint8_t UBX_NMEA_GAQ = 0x45;  //GxGAQ (poll a standard message (if the current talker ID is GA))
const uint8_t UBX_NMEA_GBQ = 0x44;  //GxGBQ (poll a standard message (if the current Talker ID is GB))
const uint8_t UBX_NMEA_GBS = 0x09;  //GxGBS (GNSS satellite fault detection)
const uint8_t UBX_NMEA_GGA = 0x00;  //GxGGA (Global positioning system fix data)
const uint8_t UBX_NMEA_GLL = 0x01;  //GxGLL (latitude and long, whith time of position fix and status)
const uint8_t UBX_NMEA_GLQ = 0x43;  //GxGLQ (poll a standard message (if the current Talker ID is GL))
const uint8_t UBX_NMEA_GNQ = 0x42;  //GxGNQ (poll a standard message (if the current Talker ID is GN))
const uint8_t UBX_NMEA_GNS = 0x0D;  //GxGNS (GNSS fix data)
const uint8_t UBX_NMEA_GPQ = 0x040; //GxGPQ (poll a standard message (if the current Talker ID is GP))
const uint8_t UBX_NMEA_GRS = 0x06;  //GxGRS (GNSS range residuals)
const uint8_t UBX_NMEA_GSA = 0x02;  //GxGSA (GNSS DOP and Active satellites)
const uint8_t UBX_NMEA_GST = 0x07;  //GxGST (GNSS Pseudo Range Error Statistics)
const uint8_t UBX_NMEA_GSV = 0x03;  //GxGSV (GNSS satellites in view)
const uint8_t UBX_NMEA_RMC = 0x04;  //GxRMC (Recommended minimum data)
const uint8_t UBX_NMEA_TXT = 0x41;  //GxTXT (text transmission)
const uint8_t UBX_NMEA_VLW = 0x0F;  //GxVLW (dual ground/water distance)
const uint8_t UBX_NMEA_VTG = 0x05;  //GxVTG (course over ground and Ground speed)
const uint8_t UBX_NMEA_ZDA = 0x08;  //GxZDA (Time and Date)

//The following are used to configure the NMEA protocol main talker ID and GSV talker ID
const uint8_t UBX_NMEA_MAINTALKERID_NOTOVERRIDDEN = 0x00; //main talker ID is system dependent
const uint8_t UBX_NMEA_MAINTALKERID_GP = 0x01;			  //main talker ID is GPS
const uint8_t UBX_NMEA_MAINTALKERID_GL = 0x02;			  //main talker ID is GLONASS
const uint8_t UBX_NMEA_MAINTALKERID_GN = 0x03;			  //main talker ID is combined receiver
const uint8_t UBX_NMEA_MAINTALKERID_GA = 0x04;			  //main talker ID is Galileo
const uint8_t UBX_NMEA_MAINTALKERID_GB = 0x05;			  //main talker ID is BeiDou
const uint8_t UBX_NMEA_GSVTALKERID_GNSS = 0x00;			  //GNSS specific Talker ID (as defined by NMEA)
const uint8_t UBX_NMEA_GSVTALKERID_MAIN = 0x01;			  //use the main Talker ID

//The following are used to configure INF UBX messages (information messages).  Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 34)
const uint8_t UBX_INF_CLASS = 0x04;   //All INF messages have 0x04 as the class
const uint8_t UBX_INF_DEBUG = 0x04;   //ASCII output with debug contents
const uint8_t UBX_INF_ERROR = 0x00;   //ASCII output with error contents
const uint8_t UBX_INF_NOTICE = 0x02;  //ASCII output with informational contents
const uint8_t UBX_INF_TEST = 0x03;	//ASCII output with test contents
const uint8_t UBX_INF_WARNING = 0x01; //ASCII output with warning contents

//The following are used to configure LOG UBX messages (loggings messages).  Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 34)
const uint8_t UBX_LOG_CREATE = 0x07;		   //Create Log File
const uint8_t UBX_LOG_ERASE = 0x03;			   //Erase Logged Data
const uint8_t UBX_LOG_FINDTIME = 0x0E;		   //Find index of a log entry based on a given time, or response to FINDTIME requested
const uint8_t UBX_LOG_INFO = 0x08;			   //Poll for log information, or Log information
const uint8_t UBX_LOG_RETRIEVEPOSEXTRA = 0x0F; //Odometer log entry
const uint8_t UBX_LOG_RETRIEVEPOS = 0x0B;	  //Position fix log entry
const uint8_t UBX_LOG_RETRIEVESTRING = 0x0D;   //Byte string log entry
const uint8_t UBX_LOG_RETRIEVE = 0x09;		   //Request log data
const uint8_t UBX_LOG_STRING = 0x04;		   //Store arbitrary string on on-board flash

//The following are used to configure MGA UBX messages (Multiple GNSS Assistance Messages).  Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 34)
const uint8_t UBX_MGA_ACK_DATA0 = 0x60;		 //Multiple GNSS Acknowledge message
const uint8_t UBX_MGA_BDS_EPH = 0x03;		 //BDS Ephemeris Assistance
const uint8_t UBX_MGA_BDS_ALM = 0x03;		 //BDS Almanac Assistance
const uint8_t UBX_MGA_BDS_HEALTH = 0x03;	 //BDS Health Assistance
const uint8_t UBX_MGA_BDS_UTC = 0x03;		 //BDS UTC Assistance
const uint8_t UBX_MGA_BDS_IONO = 0x03;		 //BDS Ionospheric Assistance
const uint8_t UBX_MGA_DBD = 0x80;			 //Either: Poll the Navigation Database, or Navigation Database Dump Entry
const uint8_t UBX_MGA_GAL_EPH = 0x02;		 //Galileo Ephemeris Assistance
const uint8_t UBX_MGA_GAL_ALM = 0x02;		 //Galileo Almanac Assitance
const uint8_t UBX_MGA_GAL_TIMOFFSET = 0x02;  //Galileo GPS time offset assistance
const uint8_t UBX_MGA_GAL_UTC = 0x02;		 //Galileo UTC Assistance
const uint8_t UBX_MGA_GLO_EPH = 0x06;		 //GLONASS Ephemeris Assistance
const uint8_t UBX_MGA_GLO_ALM = 0x06;		 //GLONASS Almanac Assistance
const uint8_t UBX_MGA_GLO_TIMEOFFSET = 0x06; //GLONASS Auxiliary Time Offset Assistance
const uint8_t UBX_MGA_GPS_EPH = 0x00;		 //GPS Ephemeris Assistance
const uint8_t UBX_MGA_GPS_ALM = 0x00;		 //GPS Almanac Assistance
const uint8_t UBX_MGA_GPS_HEALTH = 0x00;	 //GPS Health Assistance
const uint8_t UBX_MGA_GPS_UTC = 0x00;		 //GPS UTC Assistance
const uint8_t UBX_MGA_GPS_IONO = 0x00;		 //GPS Ionosphere Assistance
const uint8_t UBX_MGA_INI_POS_XYZ = 0x40;	//Initial Position Assistance
const uint8_t UBX_MGA_INI_POS_LLH = 0x40;	//Initial Position Assitance
const uint8_t UBX_MGA_INI_TIME_UTC = 0x40;   //Initial Time Assistance
const uint8_t UBX_MGA_INI_TIME_GNSS = 0x40;  //Initial Time Assistance
const uint8_t UBX_MGA_INI_CLKD = 0x40;		 //Initial Clock Drift Assitance
const uint8_t UBX_MGA_INI_FREQ = 0x40;		 //Initial Frequency Assistance
const uint8_t UBX_MGA_INI_EOP = 0x40;		 //Earth Orientation Parameters Assistance
const uint8_t UBX_MGA_QZSS_EPH = 0x05;		 //QZSS Ephemeris Assistance
const uint8_t UBX_MGA_QZSS_ALM = 0x05;		 //QZSS Almanac Assistance
const uint8_t UBX_MGA_QZAA_HEALTH = 0x05;	//QZSS Health Assistance

//The following are used to configure the MON UBX messages (monitoring messages). Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 35)
const uint8_t UBX_MON_COMMS = 0x36; //Comm port information
const uint8_t UBX_MON_GNSS = 0x28;  //Information message major GNSS selection
const uint8_t UBX_MON_HW2 = 0x0B;   //Extended Hardware Status
const uint8_t UBX_MON_HW3 = 0x37;   //HW I/O pin information
const uint8_t UBX_MON_HW = 0x09;	//Hardware Status
const uint8_t UBX_MON_IO = 0x02;	//I/O Subsystem Status
const uint8_t UBX_MON_MSGPP = 0x06; //Message Parse and Process Status
const uint8_t UBX_MON_PATCH = 0x27; //Output information about installed patches
const uint8_t UBX_MON_RF = 0x38;	//RF information
const uint8_t UBX_MON_RXBUF = 0x07; //Receiver Buffer Status
const uint8_t UBX_MON_RXR = 0x21;   //Receiver Status Information
const uint8_t UBX_MON_TXBUF = 0x08; //Transmitter Buffer Status. Used for query tx buffer size/state.
const uint8_t UBX_MON_VER = 0x04;   //Receiver/Software Version. Used for obtaining Protocol Version.

//The following are used to configure the NAV UBX messages (navigation results messages). Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 35-36)
const uint8_t UBX_NAV_CLOCK = 0x22;		//Clock Solution
const uint8_t UBX_NAV_DOP = 0x04;		//Dilution of precision
const uint8_t UBX_NAV_EOE = 0x61;		//End of Epoch
const uint8_t UBX_NAV_GEOFENCE = 0x39;  //Geofencing status. Used to poll the geofence status
const uint8_t UBX_NAV_HPPOSECEF = 0x13; //High Precision Position Solution in ECEF. Used to find our positional accuracy (high precision).
const uint8_t UBX_NAV_HPPOSLLH = 0x14;  //High Precision Geodetic Position Solution. Used for obtaining lat/long/alt in high precision
const uint8_t UBX_NAV_ODO = 0x09;		//Odometer Solution
const uint8_t UBX_NAV_ORB = 0x34;		//GNSS Orbit Database Info
const uint8_t UBX_NAV_POSECEF = 0x01;   //Position Solution in ECEF
const uint8_t UBX_NAV_POSLLH = 0x02;	//Geodetic Position Solution
const uint8_t UBX_NAV_PVT = 0x07;		//All the things! Position, velocity, time, PDOP, height, h/v accuracies, number of satellites. Navigation Position Velocity Time Solution.
const uint8_t UBX_NAV_RELPOSNED = 0x3C; //Relative Positioning Information in NED frame
const uint8_t UBX_NAV_RESETODO = 0x10;  //Reset odometer
const uint8_t UBX_NAV_SAT = 0x35;		//Satellite Information
const uint8_t UBX_NAV_SIG = 0x43;		//Signal Information
const uint8_t UBX_NAV_STATUS = 0x03;	//Receiver Navigation Status
const uint8_t UBX_NAV_SVIN = 0x3B;		//Survey-in data. Used for checking Survey In status
const uint8_t UBX_NAV_TIMEBDS = 0x24;   //BDS Time Solution
const uint8_t UBX_NAV_TIMEGAL = 0x25;   //Galileo Time Solution
const uint8_t UBX_NAV_TIMEGLO = 0x23;   //GLO Time Solution
const uint8_t UBX_NAV_TIMEGPS = 0x20;   //GPS Time Solution
const uint8_t UBX_NAV_TIMELS = 0x26;	//Leap second event information
const uint8_t UBX_NAV_TIMEUTC = 0x21;   //UTC Time Solution
const uint8_t UBX_NAV_VELECEF = 0x11;   //Velocity Solution in ECEF
const uint8_t UBX_NAV_VELNED = 0x12;	//Velocity Solution in NED

//The following are used to configure the RXM UBX messages (receiver manager messages). Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 36)
const uint8_t UBX_RXM_MEASX = 0x14; //Satellite Measurements for RRLP
const uint8_t UBX_RXM_PMREQ = 0x41; //Requests a Power Management task (two differenent packet sizes)
const uint8_t UBX_RXM_RAWX = 0x15;  //Multi-GNSS Raw Measurement Data
const uint8_t UBX_RXM_RLM = 0x59;   //Galileo SAR Short-RLM report (two different packet sizes)
const uint8_t UBX_RXM_RTCM = 0x32;  //RTCM input status
const uint8_t UBX_RXM_SFRBX = 0x13; //Boradcast Navigation Data Subframe

//The following are used to configure the SEC UBX messages (security feature messages). Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 36)
const uint8_t UBX_SEC_UNIQID = 0x03; //Unique chip ID

//The following are used to configure the TIM UBX messages (timing messages). Descriptions from UBX messages overview (ZED_F9P Interface Description Document page 36)
const uint8_t UBX_TIM_TM2 = 0x03;  //Time mark data
const uint8_t UBX_TIM_TP = 0x01;   //Time Pulse Timedata
const uint8_t UBX_TIM_VRFY = 0x06; //Sourced Time Verification

//The following are used to configure the UPD UBX messages (firmware update messages). Descriptions from UBX messages overview (ZED-F9P Interface Description Document page 36)
const uint8_t UBX_UPD_SOS = 0x14; //Poll Backup Fil Restore Status, Create Backup File in Flash, Clear Backup File in Flash, Backup File Creation Acknowledge, System Restored from Backup

//The following are used to enable RTCM messages
const uint8_t UBX_RTCM_MSB = 0xF5;  //All RTCM enable commands have 0xF5 as MSB
const uint8_t UBX_RTCM_1005 = 0x05; //Stationary RTK reference ARP
const uint8_t UBX_RTCM_1074 = 0x4A; //GPS MSM4
const uint8_t UBX_RTCM_1077 = 0x4D; //GPS MSM7
const uint8_t UBX_RTCM_1084 = 0x54; //GLONASS MSM4
const uint8_t UBX_RTCM_1087 = 0x57; //GLONASS MSM7
const uint8_t UBX_RTCM_1094 = 0x5E; //Galileo MSM4
const uint8_t UBX_RTCM_1124 = 0x7C; //BeiDou MSM4
const uint8_t UBX_RTCM_1230 = 0xE6; //GLONASS code-phase biases, set to once every 10 seconds

const uint8_t UBX_ACK_NACK = 0x00;
const uint8_t UBX_ACK_ACK = 0x01;
const uint8_t UBX_ACK_NONE = 0x02; //Not a real value

const uint8_t SVIN_MODE_DISABLE = 0x00;
const uint8_t SVIN_MODE_ENABLE = 0x01;

//The following consts are used to configure the various ports and streams for those ports. See -CFG-PRT.
const uint8_t COM_PORT_I2C = 0;
const uint8_t COM_PORT_UART1 = 1;
const uint8_t COM_PORT_UART2 = 2;
const uint8_t COM_PORT_USB = 3;
const uint8_t COM_PORT_SPI = 4;

const uint8_t COM_TYPE_UBX = (1 << 0);
const uint8_t COM_TYPE_NMEA = (1 << 1);
const uint8_t COM_TYPE_RTCM3 = (1 << 5);

//The following consts are used to generate KEY values for the advanced protocol functions of VELGET/SET/DEL
const uint8_t VAL_SIZE_1 = 0x01;  //One bit
const uint8_t VAL_SIZE_8 = 0x02;  //One byte
const uint8_t VAL_SIZE_16 = 0x03; //Two bytes
const uint8_t VAL_SIZE_32 = 0x04; //Four bytes
const uint8_t VAL_SIZE_64 = 0x05; //Eight bytes

//These are the Bitfield layers definitions for the UBX-CFG-VALSET message (not to be confused with Bitfield deviceMask in UBX-CFG-CFG)
const uint8_t VAL_LAYER_RAM = (1 << 0);
const uint8_t VAL_LAYER_BBR = (1 << 1);
const uint8_t VAL_LAYER_FLASH = (1 << 2);

//Below are various Groups, IDs, and sizes for various settings
//These can be used to call getVal/setVal/delVal
const uint8_t VAL_GROUP_I2COUTPROT = 0x72;
const uint8_t VAL_GROUP_I2COUTPROT_SIZE = VAL_SIZE_1; //All fields in I2C group are currently 1 bit

const uint8_t VAL_ID_I2COUTPROT_UBX = 0x01;
const uint8_t VAL_ID_I2COUTPROT_NMEA = 0x02;
const uint8_t VAL_ID_I2COUTPROT_RTCM3 = 0x03;

const uint8_t VAL_GROUP_I2C = 0x51;
const uint8_t VAL_GROUP_I2C_SIZE = VAL_SIZE_8; //All fields in I2C group are currently 1 byte

const uint8_t VAL_ID_I2C_ADDRESS = 0x01;

// Configuration Sub-Section mask definitions for saveConfigSelective (UBX-CFG-CFG)
const uint32_t VAL_CFG_SUBSEC_IOPORT = 0x00000001;   // ioPort - communications port settings (causes IO system reset!)
const uint32_t VAL_CFG_SUBSEC_MSGCONF = 0x00000002;  // msgConf - message configuration
const uint32_t VAL_CFG_SUBSEC_INFMSG = 0x00000004;   // infMsg - INF message configuration
const uint32_t VAL_CFG_SUBSEC_NAVCONF = 0x00000008;  // navConf - navigation configuration
const uint32_t VAL_CFG_SUBSEC_RXMCONF = 0x00000010;  // rxmConf - receiver manager configuration
const uint32_t VAL_CFG_SUBSEC_SENCONF = 0x00000100;  // senConf - sensor interface configuration (requires protocol 19+)
const uint32_t VAL_CFG_SUBSEC_RINVCONF = 0x00000200; // rinvConf - remove inventory configuration
const uint32_t VAL_CFG_SUBSEC_ANTCONF = 0x00000400;  // antConf - antenna configuration
const uint32_t VAL_CFG_SUBSEC_LOGCONF = 0x00000800;  // logConf - logging configuration
const uint32_t VAL_CFG_SUBSEC_FTSCONF = 0x00001000;  // ftsConf - FTS configuration (FTS products only)



	//These are pointed at from within the ubxPacket
	uint8_t payloadAck[2];
	uint8_t payloadCfg[MAX_PAYLOAD_SIZE];

	//Init the packet structures and init them with pointers to the payloadAck and payloadCfg arrays
	ubxPacket packetAck = {0, 0, 0, 0, 0, payloadAck, 0, 0, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED};
	ubxPacket packetCfg = {0, 0, 0, 0, 0, payloadCfg, 0, 0, SFE_UBLOX_PACKET_VALIDITY_NOT_DEFINED};
	//Limit checking of new data to every X ms
	//If we are expecting an update every X Hz then we should check every half that amount of time
	//Otherwise we may block ourselves from seeing new data
	uint8_t i2cPollingWait = 100; //Default to 100ms. Adjusted when user calls setNavigationFrequency()

	unsigned long lastCheck = 0;
	bool autoPVT = false;			  //Whether autoPVT is enabled or not
	bool autoPVTImplicitUpdate = true; // Whether autoPVT is triggered by accessing stale data (=true) or by a call to checkUblox (=false)
	uint8_t commandAck = UBX_ACK_NONE;	//This goes to UBX_ACK_ACK after we send a command and it's ack'd
	uint16_t ubxFrameCounter;			  //It counts all UBX frame. [Fixed header(2bytes), CLS(1byte), ID(1byte), length(2bytes), payload(x bytes), checksums(2bytes)]

	uint8_t rollingChecksumA; //Rolls forward as we receive incoming bytes. Checked against the last two A/B checksum bytes
	uint8_t rollingChecksumB; //Rolls forward as we receive incoming bytes. Checked against the last two A/B checksum bytes

#endif