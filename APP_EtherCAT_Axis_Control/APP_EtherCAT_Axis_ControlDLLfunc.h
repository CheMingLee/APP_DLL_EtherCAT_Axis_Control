#include "stdio.h"
#include "math.h"

#ifdef _MSC_VER  
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;
#else 
#include <stdint.h>  // for integer typedefs
#endif 

// mode define
#define MODE_IDLE 0
#define MODE_JOG 1
#define MODE_MOTION 2
#define MODE_HOME 3
#define MODE_JOGEND 4

// digital input define
#define DIGINPUT_LIMIT_LEFT 0x01
#define DIGINPUT_LIMIT_RIGHT 0x02
#define DIGINPUT_HMOE 0x04

// Params structure
typedef struct motion_params{
	double m_dJogSpeed;
	double m_dMotionSpeed;
	double m_dComeHomeSpeed;
	double m_dLeftHomeSpeed;
	double m_dJogAcc;
	double m_dMotionAcc;
	double m_dHomeAcc;
	double m_dAxisUnit;
} MOTION_PARAMS;

extern MOTION_PARAMS g_MotionParms[2];
extern double g_dJogSpeed[2];
extern double g_dJogAcc[2];
extern double g_dMotionSpeed[2];
extern double g_dMotionAcc[2];
extern double g_dComeHomeSpeed[2];
extern double g_dLeftHomeSpeed[2];
extern double g_dHomeAcc[2];

// DLL function signature
typedef void (*FuncDevInit)();
typedef void (*FuncDevClose)();
typedef int (*FuncSetDataSize)(uint32_t);
typedef int (*FuncSetTxData)(uint8_t *, uint32_t, int);
typedef int (*FuncSetSend)();
typedef int (*FuncGetBusyFlag)(uint32_t *);
typedef int (*FuncGetRxData)(uint8_t *, uint32_t, int);
typedef int (*FuncSetParams)(int, MOTION_PARAMS);
typedef int (*FuncSetJog)(int, int);
typedef int (*FuncSetMotion)(int, double);
typedef int (*FuncSetHome)(int);
typedef int (*FuncSetStop)(int);
typedef int (*FuncSetJogEnd)(int);
typedef int (*FuncSetIntrFlag)();
typedef int (*FuncSetCurPos)(int, int);
typedef int (*FuncSetServoCnt)(int8_t);
typedef int (*FuncGetCurPos)(int, int *);
typedef int (*FuncGetServoMode)(int, uint32_t *);
typedef int (*FuncGetDigInput)(int, uint32_t *);
typedef int (*FuncSetIntrFlagFalse)();
typedef int (*FuncGetCmdPos)(int, int *);

extern FuncDevInit InitialDev;
extern FuncDevClose CloseDev;
extern FuncSetDataSize SetDataSize;
extern FuncSetTxData SetTxData;
extern FuncSetSend SetSend;
extern FuncGetBusyFlag GetBusyFlag;
extern FuncGetRxData GetRxData;
extern FuncSetParams SetParams;
extern FuncSetJog SetJog;
extern FuncSetMotion SetMotion;
extern FuncSetHome SetHome;
extern FuncSetStop SetStop;
extern FuncSetJogEnd SetJogEnd;
extern FuncSetIntrFlag SetIntrFlag;
extern FuncSetCurPos SetCurPos;
extern FuncSetServoCnt SetServoCnt;
extern FuncGetCurPos GetCurPos;
extern FuncGetServoMode GetServoMode;
extern FuncGetDigInput GetDigInput;
extern FuncSetIntrFlagFalse SetIntrFlagFalse;
extern FuncGetCmdPos GetCmdPos;

extern CString g_strIniPath;
