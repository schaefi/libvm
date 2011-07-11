/**************
FILE          : vmattach.h
***************
PROJECT       : libvm - library interface [ header ]
              :
AUTHOR        : Marcus Schäfer <ms@suse.de>
              :
BELONGS TO    : libvm - OS image dev. tools
              :
DESCRIPTION   : native C++ class library to provide
              : encapsulated access to virtual hardware
              : via the KVM infrastructure
              :
STATUS        : Status: Development
**************/
#ifndef LIBVM_ATTACH_H
#define LIBVM_ATTACH_H 1

//====================================
// Includes...
//------------------------------------
#include <qlist.h>
#include <qstring.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qprocess.h>
#include <qfile.h>
#include <stdio.h>  // printf
#include <unistd.h> // read
#include <fcntl.h>  // flags
#include <string.h> // memset

//====================================
// Defines...
//------------------------------------
#include "config.h"
#include "vmpty.h"

namespace LibVM {
//====================================
// Class VMAttach...
//------------------------------------
/*! \brief LibVM -  Linux process execution class.
*
* The VMAttach class provides public methods to run
* the libvm initrd and gain access to the attached
* image virtual hardware devices
*/
class VMAttach : public QObject {
	Q_OBJECT

	public:
	void addDisk (const QString&);
	void addISO  (const QString&);
	bool init ( void );

	public:
	QString getPTY (void);
	int getPID(void);
	void exit (void);

	private:
	QString disk;
	QString iso;
	QStringList args;
	QString pty;
	QProcess* p;
	int instancePID;

	public:
	VMAttach ( void );
};
} // end namespace
#endif
