// 
//  Author:
//    seagull seagull.kamome@gmail.com
// 
//  Copyright (c) 2011, seagull
// 
//  All rights reserved.
// 
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
//     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in
//       the documentation and/or other materials provided with the distribution.
//     * Neither the name of the [ORGANIZATION] nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
// 
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 


#include "DataLogger.h"
//#include "FatFs.h"

void setup()
{
	Serial.begin(115200);
	DataLoggerShield::begin(DataLoggerShield::USE_RTC | DataLoggerShield::USE_SD_CARD | DataLoggerShield::USE_FAT_FS);
	
	pf_open("hello.txt");
	char buf[31] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
	WORD x;
	pf_read(buf, 30, &x);
	buf[30] = '\0';
	Serial.println(buf);
}


void loop()
{
	char	buf[14];
	static char rcv[14];
	static char* p = rcv;
	DataLoggerShield::RTC::DateTime_t tm;

	if (Serial.available()) {
		*p++ = Serial.read();
		if (p == rcv + 13) {
			tm.year =(rcv[0] - '0' << 4) & 0xf0 + (rcv[1] - '0') & 0x0f;
			tm.mon =(rcv[2] - '0' << 4) & 0xf0 + (rcv[3] - '0') & 0x0f;
			tm.day =(rcv[4] - '0' << 4) & 0xf0 + (rcv[5] - '0') & 0x0f;
			tm.day =(rcv[6] - '0') & 0x0f;
			tm.hour =(rcv[7] - '0' << 4) & 0xf0 + (rcv[8] - '0') & 0x0f;
			tm.min =(rcv[9] - '0' << 4) & 0xf0 + (rcv[10] - '0') & 0x0f;
			tm.sec =(rcv[11] - '0' << 4) & 0xf0 + (rcv[12] - '0') & 0x0f;
			DataLoggerShield::RTC::write(tm);
		}
	}

#if 1
	DataLoggerShield::RTC::read(&tm);

	tm.format(buf);
	buf[13] = '\0';

	Serial.println(buf);
#endif	
	delay(1000);

}