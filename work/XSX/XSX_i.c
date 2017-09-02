/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sun Dec 21 14:14:16 2008
 */
/* Compiler settings for C:\develop\xraynotes\work\XSX\XSX.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_IXFX = {0xCA507F13,0x9839,0x4466,{0x8E,0xF0,0x5C,0xAE,0xA1,0xE6,0x6A,0x1C}};


const IID LIBID_XSXLib = {0xBA63C474,0xF386,0x4542,{0xB7,0x79,0xCB,0x31,0x5A,0xC6,0x6F,0xF6}};


const CLSID CLSID_XFX = {0x9580EBAF,0xB0FF,0x4F0C,{0xA2,0xDF,0x2D,0xFD,0x46,0xB9,0xCB,0x4C}};


#ifdef __cplusplus
}
#endif

