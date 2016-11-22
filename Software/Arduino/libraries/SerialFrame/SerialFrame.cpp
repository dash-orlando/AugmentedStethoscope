/*
 */
#include <SerialFrame.h>
#include <inttypes.h>
#include <TimerThree.h>


volatile unsigned long frameCount = 0;
String					saDataString;


// default constructor
SerialFrame::SerialFrame()
{
	frame = (Frame)
	{
		0,
		GENERIC,
		GENERAL,
		"0"
	};
}


// constructor
SerialFrame::SerialFrame( byte deviceid, deviceType devtype, dataType datatype, String datastring )
{
	frame = (Frame)
	{
		deviceid,
		devtype,
		datatype,
		datastring
	};
	saDataString	 = datastring;
}


// constructor
SerialFrame::SerialFrame( Frame f )
{
	frame			 = f;
	saDataString	 = f.DataString;
}


// start of public methods
Frame SerialFrame::Get()
{
	return frame;
}


void SerialFrame::Set( byte deviceid, deviceType devtype, dataType datatype, String datastring )
{
	frame.DevID		 = deviceid;
	frame.DevType	 = devtype;
	frame.DatType	 = datatype;
	frame.DataString = datastring;
	saDataString	 = datastring;
}


void SerialFrame::Set( Frame f )
{
	frame			 = f;
	saDataString	 = f.DataString;
}


void SerialFrame::Send( txType tx )
{
}


void SerialFrame::StartSend( txType tx, long interval )
{
	Timer3.initialize( interval * 1000L );
	if ( tx == FRAME )
	{
		Timer3.attachInterrupt( SerialFrame::frameOut );
		Serial.println( "frame-start" );
	}
	else if ( tx == STRING )
	{
		Timer3.attachInterrupt( SerialFrame::stringOut );
		Serial.println( "string-start" );
	}
	else Serial.println( "neither-started" );
}


void SerialFrame::StopSend( txType tx )
{
	if ( tx == FRAME )
	{
		Timer3.detachInterrupt();
		Serial.println( "frame-stop" );
	}
	else if ( tx == STRING )
	{
		Timer3.detachInterrupt();
		Serial.println( "string-stop" );
	}
	else Serial.println( "neither-stopped" );
}


void SerialFrame::frameOut( void )
{
	unsigned long frameCountCopy;
	
	frameCount++;
	noInterrupts();
    frameCountCopy = frameCount;
    interrupts();

}

void SerialFrame::stringOut( void )
{
	String s = saDataString;

	for ( int i = 0; i < (int)s.length(); i++ )
		Serial.write( (byte)s[i] );
	Serial.write( '\00' );
}