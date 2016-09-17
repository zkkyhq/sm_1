typedef unsigned char BYTE;

struct RMSTREAM_HEADER
{
	unsigned long IFrameType;	
	unsigned long IFrameLen:24; /*位域*/
	unsigned long ISreamExam:8;
	unsigned long IExtendLen:24;
	unsigned long IExtendCount:8;
};

struct RMS_INFOTYPE
{
	unsigned long LInfoType:8;
	unsigned long LInfoLength:24;
};

struct RMS_DATETIME
{
	BYTE cYear;
	BYTE cMonth;
	BYTE cDay;
	BYTE cHour;
	BYTE cMinute;
	BYTE cSecond;
	unsigned short usMilliSecond:10;
	unsigned short usWeek:3;
	unsigned short usReserved:2;
	unsigned short usMilliValidate:1;
};

struct RMFI2_RTCTIME
{
	struct RMS_INFOTYPE stuInfoTYpe;
	struct RMS_DATETIME stuRtcTime;
};

struct PACKET
{
	unsigned char flag;	/*同步标志*/
	struct RMSTREAM_HEADER head; /*封装头*/
	struct RMFI2_RTCTIME rtc; /*扩展数据：RTC*/
	unsigned char frame[1024]; /*音视频数据*/
};

struct PACKET_I
{
	struct RMSTREAM_HEADER head; /*封装头*/
	struct RMFI2_RTCTIME rtc; /*扩展数据：RTC*/
	unsigned char frame[1024]; /*音视频数据*/
};

struct INDEX_INFO
{
	BYTE cHour;
	BYTE cMinute;
	BYTE cSecond;
	unsigned int offset;
};

