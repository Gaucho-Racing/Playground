#include <Arduino.h>
#include <../lib/acan-t4/src/ACAN_T4.h>

//-----------------------------------------------------------------

static const uint32_t QUARTZ_FREQUENCY = 8UL * 1000UL * 1000UL ; // 16 MHz

void setup () {
    pinMode (LED_BUILTIN, OUTPUT) ;
    Serial.println ("CAN1 loopback test") ;
    ACAN_T4_Settings settings (QUARTZ_FREQUENCY, 125 * 1000) ; // 125 kbit/s
    const uint32_t errorCode = ACAN_T4::can1.begin (settings) ;
    Serial.print ("Bitrate prescaler: ") ;
    Serial.println (settings.mBitRatePrescaler) ;
    Serial.print ("Propagation Segment: ") ;
    Serial.println (settings.mPropagationSegment) ;
    Serial.print ("Phase segment 1: ") ;
    Serial.println (settings.mPhaseSegment1) ;
    Serial.print ("Phase segment 2: ") ;
    Serial.println (settings.mPhaseSegment2) ;
    Serial.print ("RJW: ") ;
    Serial.println (settings.mRJW) ;
    Serial.print ("Triple Sampling: ") ;
    Serial.println (settings.mTripleSampling ? "yes" : "no") ;
    Serial.print ("Actual bitrate: ") ;
    Serial.print (settings.actualBitRate ()) ;
    Serial.println (" bit/s") ;
    Serial.print ("Exact bitrate ? ") ;
    Serial.println (settings.exactBitRate () ? "yes" : "no") ;
    Serial.print ("Distance from wished bitrate: ") ;
    Serial.print (settings.ppmFromWishedBitRate ()) ;
    Serial.println (" ppm") ;
    Serial.print ("Sample point: ") ;
    Serial.print (settings.samplePointFromBitStart ()) ;
    Serial.println ("%") ;
    if (0 == errorCode) {
        Serial.println ("can1 ok") ;
    }else{
        Serial.print ("Error can1: 0x") ;
        Serial.println (errorCode, HEX) ;
        while (1) {
            delay (100) ;
            Serial.println ("Invalid setting") ;
            digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
        }
    }
}

//-----------------------------------------------------------------

static uint32_t gBlinkDate = 0 ;
static uint32_t gSendDate = 0 ;
static uint32_t gSentCount = 0 ;
static uint32_t gReceivedCount = 0 ;

//-----------------------------------------------------------------

void loop () {
    if (gBlinkDate <= millis ()) {
        gBlinkDate += 500 ;
        digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN)) ;
    }
    CANMessage message ;
    if (gSendDate <= millis ()) {
        message.id = 0x542 ;
        const bool ok = ACAN_T4::can1.tryToSend (message) ;
        if (ok) {
            gSendDate += 2000 ;
            gSentCount += 1 ;
            Serial.print ("Sent: ") ;
            Serial.println (gSentCount) ;
        }
    }
    if (ACAN_T4::can1.receive (message)) {
        gReceivedCount += 1 ;
        Serial.print ("Received: ") ;
        Serial.println (gReceivedCount) ;
    }
}
