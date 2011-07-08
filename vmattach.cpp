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

void VMAttach::addDisk (const QString& name) {
	QTextStream(&disk) << " -drive file=" << name << ",if=virtio";
}

void VMAttach::addISO (const QString& name) {
	QTextStream(&iso) << " -cdrom " << name;
}

bool VMAttach::init (void) {
	QString program = "/bin/bash";
	QString params;
	QTextStream(&params)
		<< "qemu-kvm"
		<< " --kernel " << VM_KERNEL
		<< " --initrd " << VM_INITRD
		<< " --append " << "\"console=ttyS0 kiwistderr=/dev/console\""
		<< " -net " << "nic,model=virtio,macaddr=52:54:00:12:34:51"
		<< " -nographic "
		<< " -serial "  << "pty"
		<< " -monitor " << "/dev/null";
	if (! disk.isEmpty()) {
		QTextStream(&params) << disk;
	}
	if (! iso.isEmpty()) {
		QTextStream(&params) << iso;
	}
	args << "-c" << params;
	p = new QProcess (this);
	p -> start (program,args);
	instancePID = p -> pid();
	p -> waitForReadyRead (200);
	QByteArray in = p -> readAllStandardError();
	//printf ("%s\n",in.data());
	QRegExp modeExp ("(/dev/pts/\\d+)");
	int rpos = modeExp.indexIn (in);
	if (rpos >= 0) {
		pty = modeExp.cap(1);
	}
	QFile* fp = new QFile (pty);
	while (! fp->exists()) {
		//printf ("Waiting for terminal to appear\n");
	}
	fp -> open ( QIODevice::ReadOnly );
	QTextStream bootData (fp);
	QString line = bootData.readLine();
	while (!line.isNull()) {
		QRegExp bootDoneExp ("shellException: providing shell");
		int rpos = bootDoneExp.indexIn (line);
		if (rpos >= 0) {
			//printf ("BOOTING DONE\n");
			//printf ("%s\n",line.toLatin1().data());
			sleep (1); // let the shell settle
			break;
		}
		line = bootData.readLine();
	}
	fp -> close();
}

QString VMAttach::getPTY (void) {
	return pty;
}

int VMAttach::getPID (void) {
	return instancePID;
}

void VMAttach::exit (void) {
	p -> close();
}

} // end namespace
