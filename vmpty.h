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

//====================================
// Defines...
//------------------------------------
#define VM_READ  0
#define VM_WRITE 1
#define IDLE     1

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
	void write (const QString&);
	virtual void run ( void );
	QString* readRecorded (void);
	void startRecordingUntil (const QString&);
	void setOperationMode (int);

	private:
	int mode;
	QTimer* mTimer;
	bool recording;
	QString done;
	QString* storage;
	QString pty;
	QFile* fp;

	public:
	VMPty ( const QString&);
	~VMPty ( void );
};
} // end namespace
#endif
