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

// DLL function signature
typedef void (*FuncDevInit)();
typedef void (*FuncDevClose)();
typedef int (*FuncSetDataSize)(uint32_t);
typedef int (*FuncSetTxData)(uint8_t *, uint32_t, int);
typedef int (*FuncSetSend)();
typedef int (*FuncGetBusyFlag)(uint32_t *);
typedef int (*FuncGetRxData)(uint8_t *, uint32_t, int);

extern FuncDevInit InitialDev;
extern FuncDevClose CloseDev;
extern FuncSetDataSize SetDataSize;
extern FuncSetTxData SetTxData;
extern FuncSetSend SetSend;
extern FuncGetBusyFlag GetBusyFlag;
extern FuncGetRxData GetRxData;
