/**************
FILE          : vmattach.cpp
***************
PROJECT       : libvm - library interface
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
#include "vmattach.h"

namespace LibVM {
//====================================
// Constructor...
//------------------------------------
VMAttach::VMAttach ( void ) {
}

//====================================
// addDisk
//------------------------------------
void VMAttach::addDisk (const QString& name) {
	QFile* f = new QFile (name);
	if (! f->exists()) {
		QString error;
		QTextStream (&error)
			<< "Disk " << name << " doesn't exist";
		qError (error.toLatin1().data());
		return;
	}
	QTextStream(&disk) << " -drive file=" << name << ",if=virtio";
}

//====================================
// addISO
//------------------------------------
void VMAttach::addISO (const QString& name) {
	QFile* f = new QFile (name);
	if (! f->exists()) {
		QString error;
		QTextStream (&error)
			<< "ISO file " << name << " doesn't exist";
		qError (error.toLatin1().data());
		return;
	}
	QTextStream(&iso) << " -cdrom " << name;
}

//====================================
// init
//------------------------------------
bool VMAttach::init (void) {
	QString program = "/bin/bash";
	QString params;
	//====================================
	// prepare VM params
	//------------------------------------
	QTextStream(&params)
		<< "qemu-kvm"
		<< " --kernel " << VM_KERNEL
		<< " --initrd " << VM_INITRD
		<< " --append " << "\"console=ttyS0 kiwistderr=/dev/hvc0\""
		<< " -net " << "nic,model=virtio,macaddr=52:54:00:12:34:51"
		<< " -virtioconsole " << "pty";
	if (! disk.isEmpty()) {
		QTextStream(&params) << disk;
	}
	if (! iso.isEmpty()) {
		QTextStream(&params) << iso;
	}
	QTextStream(&params) << " -nographic ";
	args << "-c" << params;
	//====================================
	// run KVM
	//------------------------------------
	p = new QProcess (this);
	p -> start (program,args);
	instancePID = p -> pid();
	//====================================
	// fetch pty device
	//------------------------------------
	p -> waitForReadyRead (500);
	QByteArray in = p -> readAllStandardError();
	//printf ("%s\n",in.data());
	QRegExp modeExp ("(/dev/pts/\\d+)");
	int rpos = modeExp.indexIn (in);
	if (rpos >= 0) {
		pty = modeExp.cap(1);
	} else {
		qError ("Didn't get a terminal device");
		return false;
	}
	QFile* fp = new QFile (pty);
	if (! fp->exists()) {
		QString error;
		QTextStream (&error)
			<< "Terminal device " << pty << " did not appear";
		qError (error.toLatin1().data());
		return false;
	}

	VMPty* w = new VMPty (pty);
	w -> setOperationMode (VM_INIT);
	w -> start();

	//====================================
	// wait until boot shell is there
	//------------------------------------
	int mFD = open (pty.toLatin1().data(),O_RDONLY);
	QString *line = 0;
	while ( line = read_line (mFD) ) {
		//printf ("___%s\n",line->toLatin1().data());
		QRegExp bootDoneExp ("BOOT DONE");
		int rpos = bootDoneExp.indexIn (*line);
		if (rpos >= 0) {
			//printf ("BOOTING DONE\n");
			w->exit();
			close(mFD);
			return true;
		}
	}
	w->exit();
	close (mFD);
	return false;
}

//====================================
// getPTY
//------------------------------------
QString VMAttach::getPTY (void) {
	return pty;
}

//====================================
// getPID
//------------------------------------
int VMAttach::getPID (void) {
	return instancePID;
}

//====================================
// exit
//------------------------------------
void VMAttach::exit (void) {
	p -> close();
}

} // end namespace
