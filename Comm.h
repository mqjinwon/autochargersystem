//////////////////////////////////////////////////////////////////////////////////////////////////////////
// File		: Comm.h
// Version	: 1.0.1
// Date		: 2019.08.21
// Writer	: Lee, Seungmin (CDSL)
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef _CCOMM_H__
#define _CCOMM_H__


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name	: CComm
// Summury		: Port Handler
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class CComm{

// Define ////////////////////////////////////////////////////////
public:

protected:

private:
	class _CPortHandler;


// Method ////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: CComm
	// Input	: None
	// Output	: None
	// Summury	: Standard constructor
	////////////////////////////////////////////////////////////////////////////////////////////
	CComm();



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: ~CComm
	// Input	: None
	// Output	: None
	// Summury	: Standard destructor
	////////////////////////////////////////////////////////////////////////////////////////////
	~CComm();



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: Open
	// Input	: port name(const char), baudrate(int)
	// Output	: Result(bool)
	// Summury	: Open port handler.
	////////////////////////////////////////////////////////////////////////////////////////////
	bool Open(const char* port, int baudrate);



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: Close
	// Input	: None
	// Output	: Result(bool)
	// Summury	: Close port handler.
	////////////////////////////////////////////////////////////////////////////////////////////
	bool Close();



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: isOpen
	// Input	: None
	// Output	: isopen(bool)
	// Summury	: The port is open or not
	////////////////////////////////////////////////////////////////////////////////////////////
	bool isOpen();



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: Write
	// Input	: data buffer(const char*), data length(int)
	// Output	: length of sended data(int)
	// Summury	: Write data to port.
	////////////////////////////////////////////////////////////////////////////////////////////
	int Write(const char* buf, int length);



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: Read
	// Input	: data buffer(char*), data length(int)
	// Output	: length of recved data(int)
	// Summury	: Read data from recv buffer.
	////////////////////////////////////////////////////////////////////////////////////////////
	int Read(char* buf, int length);



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: SetBufSize
	// Input	: buffer size(int)
	// Output	: None
	// Summury	: Set buffer size.
	////////////////////////////////////////////////////////////////////////////////////////////
	void SetBufSize(int size);



	////////////////////////////////////////////////////////////////////////////////////////////
	// Method	: SetTimeOut
	// Input	: time(double)
	// Output	: None
	// Summury	: Set Time Out.
	////////////////////////////////////////////////////////////////////////////////////////////
	void SetTimeOut(double t);

protected:

private:


// Member ////////////////////////////////////////////////////////
public:

protected:

private:

	_CPortHandler *_poHandler;
};

#endif