#include <qapplication.h>
#include <qlist.h>
#include <qstring.h>
#include <qregexp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../vmattach.h"
#include "../vmpty.h"

using namespace LibVM;

int main(int argc,char*argv[]) {

	QApplication app (argc,argv);
	

	VMAttach* p = new VMAttach();

	p -> addDisk ("/tmp/mytest/LimeJeOS-openSUSE-11.3.i686-1.11.3.raw");
	p -> init();

	QString device = p -> getPTY();
	printf ("%s\n",device.toLatin1().data());

	//return (0);

	VMPty* r = new VMPty (device);
	r -> setOperationMode (VM_READ);
	r -> start();

	VMPty* w = new VMPty (device);
	w -> setOperationMode (VM_WRITE);
	w -> start();

	w->write ("mount /dev/vda1 /mnt");
	w->write ("find /mnt -mtime -5 -print");

	r->startRecordingUntil ("***done***");

	w->write ("echo ***done***");
	w->write ("umount /mnt");

	QString* out = r->readRecorded();
	printf ("%s\n",out->toLatin1().data());

	//return app.exec(); // event loop required for timer

	p -> exit();
}
