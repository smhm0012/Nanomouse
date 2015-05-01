/**
  DB.h
  Database library for Arduino 
  Written by Madhusudana das
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DB_PROM
#define DB_PROM

#include "EEPROM.h"
/**Declaring database header for number and size*/
struct DB_Header
{
  /**Declaring n_recs as number*/
  byte n_recs;
  /**Declaring n_size as size*/
  byte rec_size;
};

/// slightly padded for the time being
#define DB_HEAD_SIZE 4

/// DB_error values
#define DB_OK 0
#define DB_RECNO_OUT_OF_RANGE 1

#define DB_REC (byte*)(void*)&

typedef byte* DB_Rec;
/**Manage the Database and connection*/  
class DB {
  public:
    /**Creates a table*/
    void    create(int head_ptr, byte recsize); 
    /**Opens the table*/
    void    open(int head_ptr);
    boolean write(byte recno, const DB_Rec rec);
	/**Reads and collect information from the table inside the EEPROM*/
    boolean read(byte recno, DB_Rec rec);
	/**Delete a record from the table*/
    boolean deleteRec(byte recno);	                /// delete is a reserved word
    boolean insert(byte recno, const DB_Rec rec);
	/**Store the table inside EEPROM*/
    void    append(DB_Rec rec);
	/**Number of records*/
    byte    nRecs();
	/**Number and size information*/
    DB_Header DB_head;
	/**declaring a database error*/
    byte DB_error;
  private:
    int writeHead();
    int readHead();
    int EEPROM_dbWrite(int ee, const byte* p);
    int EEPROM_dbRead(int ee, byte* p);
    int DB_head_ptr;
    int DB_tbl_ptr;
};

extern DB db;

#endif