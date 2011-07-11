/**************
FILE          : config.cpp
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
#include "config.h"

namespace LibVM {
//====================================
// handle_error
//------------------------------------
void handle_error (const char *file, int lineno, const char *msg) {
	fprintf ( stderr, "*** libvm: %s:%i: error: %s:\n",
		file, lineno , msg
	);
}

//====================================
// read_line
//------------------------------------
QString* read_line (int fd) {
	QByteArray ba;
	while ( true ) {
		char c = 0;
		if ( ! read (fd,&c,1) ) {
			return 0;
		}
		ba.append (c);
		if (c == '\n') {
			break;
		}
	}
	QString* line = new QString (ba);
	*line = line -> trimmed ();
	return line;
}

//====================================
// write_line
//------------------------------------
void write_line (const QString& line,int fd) {
	QString msg;
	QTextStream (&msg) << line << endl << endl;
	write (fd,msg.toLatin1().data(),msg.length());
}

}
