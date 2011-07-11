/**************
FILE          : config.h
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
#ifndef LIBVM_CONFIG_H
#define LIBVM_CONFIG_H 1

//====================================
// Includes...
//------------------------------------
#include <stdio.h> 
#include <qstring.h>
#include <qbytearray.h>
#include <qtextstream.h>

namespace LibVM {
//====================================
// Defines...
//------------------------------------
#define VM_BOOT "/home/ms/Project/libvm/boot.iso"

//====================================
// Defines (macros)...
//------------------------------------
#define qError(msg)  handle_error(__FILE__, __LINE__, msg)

//====================================
// Functions...
//------------------------------------
void handle_error (const char*, int, const char*);
QString* read_line (int);
void write_line (const QString&,int);

}
#endif
