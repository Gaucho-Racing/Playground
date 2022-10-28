// Receive filter demo for Teensy 4.x

// This demo runs on Teensy 4.x, using CAN1 in looppback mode: no external hardware required.
// It shows how to define reception primary filters (up to 32)

//-----------------------------------------------------------------

#ifndef __IMXRT1062__
  #error "This sketch should be compiled for Teensy 4.x"
#endif

//-----------------------------------------------------------------

#include <ACAN_T4.h>

//-----------------------------------------------------------------

void setup () {
  pinMode (LED_BUILTIN, OUTPUT) ;
  Serial.begin (9600) ;
  while (!Serial) {
    delay (50) ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
  }
  Serial.println ("Receive filters demo") ;
  ACAN_T4_Settings settings (1000 * 1000) ;
  settings.mLoopBackMode = true ; // Loop back mode
  settings.mSelfReceptionMode = true ; // Required for loop back mode
  const ACANPrimaryFilter primaryFilters [32] = {
    ACANPrimaryFilter (kData,   kStandard, 0x00, handlePrimaryFilterReception), //  0
    ACANPrimaryFilter (kRemote, kStandard, 0x00, handlePrimaryFilterReception), //  1
    ACANPrimaryFilter (kData,   kExtended, 0x00, handlePrimaryFilterReception), //  2
    ACANPrimaryFilter (kRemote, kExtended, 0x00, handlePrimaryFilterReception), //  3
    ACANPrimaryFilter (kData,   kStandard, 0x01, handlePrimaryFilterReception), //  4
    ACANPrimaryFilter (kRemote, kStandard, 0x01, handlePrimaryFilterReception), //  5
    ACANPrimaryFilter (kData,   kExtended, 0x01, handlePrimaryFilterReception), //  6
    ACANPrimaryFilter (kRemote, kExtended, 0x01, handlePrimaryFilterReception), //  7
    ACANPrimaryFilter (kData,   kStandard, 0x02, handlePrimaryFilterReception), //  8
    ACANPrimaryFilter (kRemote, kStandard, 0x02, handlePrimaryFilterReception), //  9
    ACANPrimaryFilter (kData,   kExtended, 0x02, handlePrimaryFilterReception), // 10
    ACANPrimaryFilter (kRemote, kExtended, 0x02, handlePrimaryFilterReception), // 11
    ACANPrimaryFilter (kData,   kStandard, 0x04, handlePrimaryFilterReception), // 12
    ACANPrimaryFilter (kRemote, kStandard, 0x04, handlePrimaryFilterReception), // 13
    ACANPrimaryFilter (kData,   kExtended, 0x04, handlePrimaryFilterReception), // 14
    ACANPrimaryFilter (kRemote, kExtended, 0x04, handlePrimaryFilterReception), // 15
    ACANPrimaryFilter (kData,   kStandard, 0x08, handlePrimaryFilterReception), // 16
    ACANPrimaryFilter (kRemote, kStandard, 0x08, handlePrimaryFilterReception), // 17
    ACANPrimaryFilter (kData,   kExtended, 0x08, handlePrimaryFilterReception), // 18
    ACANPrimaryFilter (kRemote, kExtended, 0x08, handlePrimaryFilterReception), // 19
    ACANPrimaryFilter (kData,   kStandard, 0x10, handlePrimaryFilterReception), // 20
    ACANPrimaryFilter (kRemote, kStandard, 0x10, handlePrimaryFilterReception), // 21
    ACANPrimaryFilter (kData,   kExtended, 0x10, handlePrimaryFilterReception), // 22
    ACANPrimaryFilter (kRemote, kExtended, 0x10, handlePrimaryFilterReception), // 23
    ACANPrimaryFilter (kData,   kStandard, 0x20, handlePrimaryFilterReception), // 24
    ACANPrimaryFilter (kRemote, kStandard, 0x20, handlePrimaryFilterReception), // 25
    ACANPrimaryFilter (kData,   kExtended, 0x20, handlePrimaryFilterReception), // 26
    ACANPrimaryFilter (kRemote, kExtended, 0x20, handlePrimaryFilterReception), // 27
    ACANPrimaryFilter (kData,   kStandard, 0x40, handlePrimaryFilterReception), // 28
    ACANPrimaryFilter (kRemote, kStandard, 0x40, handlePrimaryFilterReception), // 29
    ACANPrimaryFilter (kData,   kExtended, 0x40, handlePrimaryFilterReception), // 30
    ACANPrimaryFilter (handlePrimaryFilterReception) // 31, catch every other frame (8161)
 //   ACANPrimaryFilter (kRemote, kExtended, 0x40, handlePrimaryFilterReception), // 31
  } ;
  const uint32_t errorCode = ACAN_T4::can1.begin (settings, primaryFilters, 32) ;
  if (0 == errorCode) {
    Serial.println ("can0 ok") ;
  }else{
    Serial.print ("Error can0: 0x") ;
    Serial.println (errorCode, HEX) ;
  }
}

//-----------------------------------------------------------------
//   PRIMARY FILTERS
//-----------------------------------------------------------------

static uint32_t gPrimaryFilterReceptionCount [32] = { 0 } ;
static uint32_t gPrimaryFilterReceptionErrorCount = 0 ;

static void handlePrimaryFilterReception (const CANMessage & inMessage) {
  if (inMessage.idx < 32) {
    gPrimaryFilterReceptionCount [inMessage.idx] += 1 ;
  }else{
    Serial.print ("Filter index error (") ;
    Serial.print (inMessage.idx) ;
    Serial.println (")") ;
    gPrimaryFilterReceptionErrorCount += 1 ;
  }
}

//-----------------------------------------------------------------

static uint32_t gBlinkDate = 0 ;
static uint32_t gSendIndex = 0 ;
static const uint32_t SEND_COUNT = 2048 * 4 ;

//-----------------------------------------------------------------

void loop () {
  if (gBlinkDate <= millis ()) {
    gBlinkDate += 2000 ;
    digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    Serial.print (gSendIndex) ;
    for (uint32_t i = 0 ; i<32 ; i++) {
      Serial.print (" ") ;
      Serial.print (gPrimaryFilterReceptionCount [i]) ;
    }
    Serial.println ("") ;
  }
  CANMessage message ;
//--- Send message
  if (gSendIndex < SEND_COUNT) {
    message.id = gSendIndex / 4 ;
    message.rtr = (gSendIndex & 1) != 0 ;
    message.ext = (gSendIndex & 2) != 0 ;
    const bool ok = ACAN_T4::can1.tryToSend (message) ;
    if (ok) {
      gSendIndex += 1 ;
    }
  }
//--- Received message handling
  ACAN_T4::can1.dispatchReceivedMessage () ;
}
