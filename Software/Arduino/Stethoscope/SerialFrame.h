/*
 */

#include <Arduino.h>
#ifndef SERIALFRAME_H
#define SERIALFRAME_H

typedef unsigned int	uint;
typedef uint8_t			byte;

typedef enum{ GENERIC, STETHOSCOPE, PULSEOX, OSCOPE, THERM }	deviceType;
typedef enum{ GENERAL, HEARTRATE }								dataType;
typedef enum{ STRING, FRAME }									txType;

typedef struct
{
	byte		DevID;
	deviceType	DevType;
	dataType	DatType;
	String 		DataString;
} Frame;
	
class SerialFrame
{
public:
	// members
	Frame frame;

	// constructors
	SerialFrame( void );
	SerialFrame( byte deviceid, deviceType devtype, dataType datatype, String datastring );
	SerialFrame( Frame f );

	// methods
	Frame Get( void );
	void  Set( byte deviceid, deviceType devtype, dataType datatype, String datastring );
	void  Set( Frame f );
	void  Send( txType tx );
	void  StartSend( txType tx, long interval );
	void  StopSend( txType tx );

private:
	static void frameOut( void );
	static void stringOut( void );
};

#endif
