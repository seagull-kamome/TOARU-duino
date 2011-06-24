//
//
//
#ifndef DATALOGGER_H
#  define DATALOGGER_H

#  include "RTC4543.h"
#  include "SerialBUS.h"
#  include "pff.h"


class DataLoggerShield
{
protected:
	inline DataLoggerShield() {}
	inline ~DataLoggerShield() {}

public:

	enum { SD_CARD_SS = 10 };
	enum { USE_RTC = 1 << 0, USE_SD_CARD = 1 << 1, USE_FAT_FS = 1 << 2 };

	typedef SPIMaster SBUS;
	typedef RTC4543<15, 14, SBUS> RTC;

    typedef SPIDevice<SPIMaster::Low, SPIMaster::OutputFirst, SPIMaster::MSBFirst, SPIMaster::DIV2> sd_card_t;
    static sd_card_t sd_card;

	static inline void begin(uint8_t flags)
	{
		if (flags & (USE_RTC | USE_SD_CARD))
			SBUS::begin();
		if (flags & USE_RTC)		RTC::begin();
		if (flags & USE_SD_CARD) {
			sd_card.begin(10);
			
			if (flags & USE_FAT_FS) {
				static ::FATFS fs;
				::pf_mount(&fs);
			}
		}
	}
};

#endif
