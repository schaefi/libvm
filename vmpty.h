/**************
FILE          : vmpty.h
***************
PROJECT       : libvm - library interface [ header ]
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : libvm - OS image dev. tools
              :
DESCRIPTION   : native C++ class library to provide
              : read / write access to the vm pty
              : via the KVM infrastructure
              :
STATUS        : Status: Development
**************/
#ifndef LIBVM_PTY_H
#define LIBVM_PTY_H 1

//====================================
// Includes...
//------------------------------------
#include <qlist.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qprocess.h>
#include <qthread.h>
#include <qfile.h>
#include <qtimer.h>
#include <qmutex.h>
#include <signal.h>
#include <stdio.h>  // printf
#include <unistd.h> // read
#include <fcntl.h>  // flags
#include <string.h> // memset

#include "config.h"

//====================================
// Defines...
//------------------------------------
#define VM_READ  0
#define VM_WRITE 1
#define VM_INIT  2

namespace LibVM {
//====================================
// Class VMPty...
//------------------------------------
/*! \brief LibVM -  Linux pty access class.
*
* The VMPty class provides public methods for
* reading and writing from/to the specified pty
*/
class VMPty : public QThread {
	Q_OBJECT

	public:
	void read  (void);
	int write (const QString&);
	int writeNoLock (const QString&);
	virtual void run ( void );
	QString* readRecorded (void);
	void startRecordingUntil (const QString&);
	void setOperationMode (int);
	void checkBoot (void);

	private:
	int mode;
	QTimer* mTimer;
	bool recording;
	QString done;
	QString* storage;
	QString pty;
	int fp;

	public:
	VMPty ( const QString&);
	~VMPty ( void );
};
} // end namespace
#endif
