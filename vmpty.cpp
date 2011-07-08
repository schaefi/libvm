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

void VMPty::setOperationMode ( int m ) {
	mode = m;
	if (mode == VM_READ) {
		fp = new QFile (pty);
		fp -> open ( QIODevice::ReadOnly );
	} else {
		fp = new QFile (pty);
		fp -> open ( QIODevice::WriteOnly );	
	}
}

void VMPty::run (void) {
	if (mode == VM_READ) {
		read();
	}
}

void VMPty::read  (void) {
	QTextStream in(fp);
	QString line = in.readLine();
	while (! line.isNull()) {
		//printf ("'%s'\n",line.toLatin1().data());
		if (line.compare ("(none):/ # ") == 0) {
			// printf ("UNLOCK\n");
			mutex.unlock();
		}
		if (recording) {
			if (line.compare (done) == 0) {
				recording = false;
				mutex.unlock();
			}
			QTextStream s(storage);
			s << line << endl;
		}
		line = in.readLine();
	}
	fp -> close();
}

void VMPty::write (const QString& data) {
	mutex.lock();
	QTextStream out(fp);
	out << data << endl << endl;
}

void VMPty::startRecordingUntil (const QString& flag) {
	recording = true;
	storage   = new QString ();
	done     = flag;
}

QString* VMPty::readRecorded (void) {
	return storage;
}

VMPty::~VMPty (void) {
	fp -> close();
}

} // end namespace
