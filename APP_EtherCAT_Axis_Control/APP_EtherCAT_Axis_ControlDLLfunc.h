#include "stdio.h"
#include "math.h"
#include "PdoDefine.h"
#include "EcmUsrDriver.h"
#include "EcmDriver.h"

#define PCI_DATA_MAX_SIZE 200

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
#define MODE_RUNFILE 5

// digital input define
#define DIGINPUT_LIMIT_LEFT 0x01
#define DIGINPUT_LIMIT_RIGHT 0x02
#define DIGINPUT_HMOE 0x04

// Cmd file define
#define BEGIN 0
#define SPEED 1
#define ACC 2
#define LINEXY 3
#define FLINEXY 4
#define ARCXY 5
#define FARCXY 6
#define END 7

// File cmd structure
typedef struct file_cmd{
	int m_iID;
	double m_dParams[5];
}FILE_CMD;

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

extern uint32_t g_u32mode[TEST_SERVO_CNT];
extern MOTION_PARAMS g_MotionParms[TEST_SERVO_CNT];
extern double g_dJogSpeed[TEST_SERVO_CNT];
extern double g_dJogAcc[TEST_SERVO_CNT];
extern double g_dMotionSpeed[TEST_SERVO_CNT];
extern double g_dMotionAcc[TEST_SERVO_CNT];
extern double g_dComeHomeSpeed[TEST_SERVO_CNT];
extern double g_dLeftHomeSpeed[TEST_SERVO_CNT];
extern double g_dHomeAcc[TEST_SERVO_CNT];

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
typedef int (*FuncSetRunFile)();
typedef int (*FuncGetRunFileBeginPosFlag)(int, bool *);
typedef int (*FuncSetRunFileCmdCnt)(int);
typedef int (*FuncSetRunFileCmd)(int, FILE_CMD);

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
extern FuncSetRunFile SetRunFile;
extern FuncGetRunFileBeginPosFlag GetRunFileBeginPosFlag;
extern FuncSetRunFileCmdCnt SetRunFileCmdCnt;
extern FuncSetRunFileCmd SetRunFileCmd;

extern CString g_strIniPath;
