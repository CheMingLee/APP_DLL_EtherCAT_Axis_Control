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

// Params structure
typedef struct motion_params{
	double m_dJogSpeed;
	double m_dMotionSpeed;
	double m_dComeHomeSpeed;
	double m_dLeftHomeSpeed;
	double m_dJogAcc;
	double m_dMotionAcc;
	double m_dHomeAcc;
	double m_dRatio;
} MOTION_PARAMS;

extern MOTION_PARAMS g_MotionParms[2];

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

extern CString g_strIniPath;
