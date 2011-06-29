/*
  SD.h - SD card and FAT fs library.
  Copyright (c) 2011 HATTORI, Hiroki.    All rights reserved.

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
#if !defined(SD_H)
#  define SD_H

#  include <libraries/SerialBus/SerialBus.h>
#  include <pff.h>

template <uint8_t ss>
class SDCard : public SPIDevice<LOW, SPIMaster::InputFirst, SPIMaster::MSBFirst, SPIMaster::DIV2, ss>
{
protected:
	typedef SPIDevice<LOW, SPIMaster::OutputFirst, SPIMaster::MSBFirst, SPIMaster::DIV2, -1> super;

public:

	inline SDCard() {}
	inline ~SDCard() {}

	static FRESULT mount(FATFS* fs)
	{
		extern uint8_t disk_spi_ss;

		disk_spi_ss = ss;
		return pf_mount(fs);
	}

	static inline void unmount()								{ pf_mount(NULL); }
	static inline FRESULT open(char const* fname)				{ return pf_open(fname); }
	static inline FRESULT read(void* buff, uint16_t len, uint16_t* bytes_read)
	{
		return pf_read(buff, len, bytes_read);
	}
	static inline uint8_t read() { uint8_t ch; uint16_t br; read(&ch, 1, &br); }
	static inline bool available() { return pf_available(); }
	static inline FRESULT write(void const* buff, uint16_t len, uint16_t* bytes_wrote)
	{
		return pf_write(buff, len, bytes_wrote);
	}
	static inline FRESULT lseek(uint16_t pos)						{ return pf_lseek(pos); }
	static inline FRESULT openDir(DIR* dir, char const* dirname)	{ return pf_opendir(dir, dirname); }
	static inline FRESULT readDir(DIR* dir, FILINFO* finfo)			{ return pf_readdir(dir, finfo); }
};


#endif