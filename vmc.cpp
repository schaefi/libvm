#include <qapplication.h>
#include <qlist.h>
#include <qstring.h>
#include <qregexp.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <getopt.h>
#include <unistd.h>

#include "vmattach.h"
#include "vmpty.h"

using namespace LibVM;

void usage (int);
void quit  (int);

static int vmpid = 0;

#define PTY_FILE "/var/cache/pty"

int main(int argc,char*argv[]) {
	QString* mDisk = 0;
	QString* mISO  = 0;
	QApplication app (argc,argv,QApplication::Tty);

	signal (SIGINT  ,quit);
	signal (SIGHUP  ,quit);
	signal (SIGTERM ,quit);

	//=====================================
	// get additional options...
	//-------------------------------------
	while (1) {
		int option_index = 0;
		static struct option long_options[] =
		{
			{"add-disk"  , 1 , 0 , 'd'},
			{"add-iso"   , 1 , 0 , 'i'},
			{"help"      , 0 , 0 , 'h'}
		};
		int c = getopt_long (
			argc, argv, "d:i:h",long_options, &option_index
		);
		if (c == -1) {
			break;
		}
		switch (c) {
			case 'i':
				mISO = new QString (optarg);
			break;
			case 'd':
				mDisk = new QString (optarg);
			break;
			case 'h':
				usage (0);
			break;
			default:
				usage (1);
			break;
		}
	}
	//=====================================
	// start VM...
	//-------------------------------------
	VMAttach* p = new VMAttach();
	if (mDisk) {
		p->addDisk (*mDisk);
	}
	if (mISO) {
		p->addISO (*mISO);
	}
	if (! p -> init()) {
		return 1;
	}
	QString device = p -> getPTY();
	vmpid = p->getPID();
	QFile data(PTY_FILE);
	if (! data.open(QFile::WriteOnly)) {
		return 1;
	}
	QTextStream fp (&data);
	fp << device << endl;
	data.close();
	daemon(0,0);
	return app.exec();
}

void quit (int s) {
	//printf ("libvm: got signal: %d\n",s);
	unlink (PTY_FILE);
	kill (vmpid,s);
	_exit (0);
}

void usage (int e) {
	_exit (e);
}
