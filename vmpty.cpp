/**************
FILE          : vmpty.cpp
***************
PROJECT       : libvm - library interface
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
#include "vmpty.h"
    
namespace LibVM {

QMutex mutex;

//====================================
// Constructor...
//------------------------------------
VMPty::VMPty ( const QString& device ) {
	pty = device;
	storage = new QString();
	recording = false;
}

//====================================
// setOperationsMode
//------------------------------------
void VMPty::setOperationMode ( int m ) {
	mode = m;
	if (mode == VM_READ) {
		fp = open (pty.toLatin1().data(),O_RDONLY);
	} else {
		fp = open (pty.toLatin1().data(),O_WRONLY | O_NDELAY);
	}
}

//====================================
// run thread
//------------------------------------
void VMPty::run (void) {
	if (mode == VM_READ) {
		read();
	}
	if (mode == VM_INIT) {
		checkBoot();
	}
}

//====================================
// checkBoot
//------------------------------------
void VMPty::checkBoot (void) {
	while (true) {
		sleep (1);
		writeNoLock ("BOOT DONE");
	}
}

//====================================
// read
//------------------------------------
void VMPty::read  (void) {
	QString *line = 0;
	while ( line = read_line (fp) ) {
		//printf ("'%s'\n",line->toLatin1().data());
		if (line->compare ("(none):/ #") == 0) {
			//printf ("**************** UNLOCK\n");
			mutex.unlock();
		}
		if (recording) {
			if (line->compare (done) == 0) {
				recording = false;
				mutex.unlock();
			}
			QTextStream s(storage);
			s << *line << endl;
		}
	}
	close (fp);
}

//====================================
// write
//------------------------------------
void VMPty::write (const QString& data) {
	mutex.lock();
	write_line (data,fp);
}

//====================================
// writeNoLock
//------------------------------------
void VMPty::writeNoLock (const QString& data) {
	write_line (data,fp);
}

//====================================
// startRecordingUntil
//------------------------------------
void VMPty::startRecordingUntil (const QString& flag) {
	recording = true;
	storage   = new QString ();
	done     = flag;
}

//====================================
// readRecorded
//------------------------------------
QString* VMPty::readRecorded (void) {
	return storage;
}

//====================================
// Destructor
//------------------------------------
VMPty::~VMPty (void) {
	close (fp);
}

} // end namespace
