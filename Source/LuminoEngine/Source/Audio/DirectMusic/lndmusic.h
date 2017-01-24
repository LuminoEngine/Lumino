#pragma once

#include <windows.h>
#define COM_NO_WINDOWS_H
#include <objbase.h>
#include <mmsystem.h>
#include <dsound.h>
#include <pshpack8.h>

//-------------------------------------------------------------

typedef WORD            TRANSITION_TYPE;
typedef __int64         REFERENCE_TIME;
typedef long            MUSIC_TIME;

interface IDirectMusic;
interface IDirectMusic8;
interface IDirectMusicBuffer;
interface IDirectMusicPort;
interface IDirectMusicThru;
interface IReferenceClock;

typedef IDirectMusic *LPDIRECTMUSIC;
typedef IDirectMusic8 *LPDIRECTMUSIC8;
typedef IDirectMusicPort *LPDIRECTMUSICPORT;
typedef IDirectMusicBuffer *LPDIRECTMUSICBUFFER;

interface IDirectMusicTrack;
interface IDirectMusicPerformance;
interface IDirectMusicPerformance8;
interface IDirectMusicTool;
interface IDirectMusicSegment;
interface IDirectMusicSegment8;
interface IDirectMusicSegmentState;
interface IDirectMusicSegmentState8;
interface IDirectMusicGraph;
interface IDirectMusicBuffer;
interface IDirectMusicInstrument;
interface IDirectMusicDownloadedInstrument;
interface IDirectMusicBand;
interface IDirectMusicChordMap;
interface IDirectMusicLoader;
interface IDirectMusicLoader8;
interface IDirectMusicScript;
interface IDirectMusicObject;
interface IDirectMusicStyle8;
interface IDirectMusicPatternTrack;
interface IDirectMusicContainer;
interface IDirectMusicTool8;
interface IDirectMusicTrack8;
interface IDirectMusicAudioPath;

//-------------------------------------------------------------

#define DMUS_PCHANNEL_ALL           0xFFFFFFFB

#define DMUS_APATH_SHARED_STEREOPLUSREVERB   1       /* A standard music set up with stereo outs and reverb. */
#define DMUS_APATH_DYNAMIC_3D                6       /* An audio path with one dynamic bus from the synth feeding to a dynamic 3d buffer. Does not send to env reverb. */
#define DMUS_APATH_DYNAMIC_MONO              7       /* An audio path with one dynamic bus from the synth feeding to a dynamic mono buffer. */
#define DMUS_APATH_DYNAMIC_STEREO            8       /* An audio path with two dynamic buses from the synth feeding to a dynamic stereo buffer. */

#define DMUS_PATH_SEGMENT          0x1000      /* Get the segment itself (from a segment state.) */
#define DMUS_PATH_SEGMENT_TRACK    0x1100      /* Look in Track List of Segment. */
#define DMUS_PATH_SEGMENT_GRAPH    0x1200      /* Get the segment's tool graph. */
#define DMUS_PATH_SEGMENT_TOOL     0x1300      /* Look in Tool Graph of Segment. */
#define DMUS_PATH_AUDIOPATH        0x2000      /* Get the audiopath itself (from a segment state.) */
#define DMUS_PATH_AUDIOPATH_GRAPH  0x2200      /* Get the audiopath's tool graph. */
#define DMUS_PATH_AUDIOPATH_TOOL   0x2300      /* Look in Tool Graph of Audio Path. */
#define DMUS_PATH_PERFORMANCE      0x3000      /* Access the performance. */
#define DMUS_PATH_PERFORMANCE_GRAPH 0x3200     /* Get the performance's tool graph. */
#define DMUS_PATH_PERFORMANCE_TOOL 0x3300      /* Look in Tool Graph of Performance. */
#define DMUS_PATH_PORT             0x4000      /* Access the synth. */
#define DMUS_PATH_BUFFER           0x6000      /* Look in DirectSoundBuffer. */
#define DMUS_PATH_BUFFER_DMO       0x6100      /* Access a DMO in the buffer. */
#define DMUS_PATH_MIXIN_BUFFER     0x7000      /* Look in a global mixin buffer. */
#define DMUS_PATH_MIXIN_BUFFER_DMO 0x7100      /* Access a DMO in a global mixin buffer. */
#define DMUS_PATH_PRIMARY_BUFFER   0x8000      /* Access the primary buffer. */

#define DMUS_MAX_NAME           64         /* Maximum object name length. */
#define DMUS_MAX_CATEGORY       64         /* Maximum object category name length. */
#define DMUS_MAX_FILENAME       MAX_PATH

#define DMUS_OBJ_OBJECT         (1 << 0)     /* Object GUID is valid. */
#define DMUS_OBJ_CLASS          (1 << 1)     /* Class GUID is valid. */
#define DMUS_OBJ_NAME           (1 << 2)     /* Name is valid. */
#define DMUS_OBJ_CATEGORY       (1 << 3)     /* Category is valid. */
#define DMUS_OBJ_FILENAME       (1 << 4)     /* File path is valid. */
#define DMUS_OBJ_FULLPATH       (1 << 5)     /* Path is full path. */
#define DMUS_OBJ_URL            (1 << 6)     /* Path is URL. */
#define DMUS_OBJ_VERSION        (1 << 7)     /* Version is valid. */
#define DMUS_OBJ_DATE           (1 << 8)     /* Date is valid. */
#define DMUS_OBJ_LOADED         (1 << 9)     /* Object is currently loaded in memory. */
#define DMUS_OBJ_MEMORY         (1 << 10)    /* Object is pointed to by pbMemData. */
#define DMUS_OBJ_STREAM         (1 << 11)    /* Object is stored in pStream. */

#define DMUS_SEG_REPEAT_INFINITE    0xFFFFFFFF  /* For IDirectMusicSegment::SetRepeat*/
#define DMUS_SEG_ALLTRACKS          0x80000000  /* For IDirectMusicSegment::SetParam() and SetTrackConfig() - selects all tracks instead on nth index. */
#define DMUS_SEG_ANYTRACK           0x80000000  /* For IDirectMusicSegment::GetParam() - checks each track until it finds one that returns data (not DMUS_E_NOT_FOUND.) */

#define DMUS_AUDIOF_3D          0x1   /* Require 3D buffers. */
#define DMUS_AUDIOF_ENVIRON     0x2   /* Require environmental modeling. */
#define DMUS_AUDIOF_EAX         0x4   /* Require use of EAX effects. */
#define DMUS_AUDIOF_DMOS        0x8   /* Require use of additional DMOs. */
#define DMUS_AUDIOF_STREAMING   0x10  /* Require support for streaming waves. */
#define DMUS_AUDIOF_BUFFERS     0x20  /* Require support for multiple buffers (all above cases need this.) */
#define DMUS_AUDIOF_ALL         0x3F  /* Requires everything. */

#define DMUS_PMSG_PART                                                                              \
    DWORD               dwSize;                                                                     \
    REFERENCE_TIME      rtTime;             /* real time (in 100 nanosecond increments) */          \
    MUSIC_TIME          mtTime;             /* music time */                                        \
    DWORD               dwFlags;            /* various bits (see DMUS_PMSGF_FLAGS enumeration) */    \
    DWORD               dwPChannel;         /* Performance Channel. The Performance can */          \
                                            /* use this to determine the port/channel. */           \
    DWORD               dwVirtualTrackID;   /* virtual track ID */                                  \
    IDirectMusicTool*   pTool;              /* tool interface pointer */                            \
    IDirectMusicGraph*  pGraph;             /* tool graph interface pointer */                      \
    DWORD               dwType;             /* PMSG type (see DMUS_PMSGT_TYPES defines) */              \
    DWORD               dwVoiceID;          /* unique voice id which allows synthesizers to */      \
                                            /* identify a specific event. For DirectX 6.0, */       \
                                            /* this field should always be 0. */                    \
    DWORD               dwGroupID;          /* Track group id */                                 \
    IUnknown*           punkUser;           /* user com pointer, auto released upon PMSG free */


//-------------------------------------------------------------

#define DMUS_MAX_DESCRIPTION 128
#define DMUS_MAX_DRIVER 128

typedef enum
{
    DMUS_CLOCK_SYSTEM = 0,
    DMUS_CLOCK_WAVE = 1
} DMUS_CLOCKTYPE;

typedef struct _DMUS_BUFFERDESC *LPDMUS_BUFFERDESC;
typedef struct _DMUS_BUFFERDESC
{
    DWORD dwSize;
    DWORD dwFlags;
    GUID guidBufferFormat;
    DWORD cbBuffer;
} DMUS_BUFFERDESC;

typedef struct _DMUS_PORTCAPS
{
    DWORD   dwSize;
    DWORD   dwFlags;
    GUID    guidPort;
    DWORD   dwClass;
    DWORD   dwType;
    DWORD   dwMemorySize;
    DWORD   dwMaxChannelGroups;
    DWORD   dwMaxVoices;
    DWORD   dwMaxAudioChannels;
    DWORD   dwEffectFlags;
    WCHAR   wszDescription[DMUS_MAX_DESCRIPTION];
} DMUS_PORTCAPS;

typedef DMUS_PORTCAPS *LPDMUS_PORTCAPS;

typedef struct _DMUS_PORTPARAMS8
{
    DWORD   dwSize;
    DWORD   dwValidParams;
    DWORD   dwVoices;
    DWORD   dwChannelGroups;
    DWORD   dwAudioChannels;
    DWORD   dwSampleRate;
    DWORD   dwEffectFlags;
    BOOL    fShare;
    DWORD   dwFeatures;
} DMUS_PORTPARAMS8;

#define DMUS_PORT_FEATURE_AUDIOPATH     0x00000001	/* Supports audiopath connection to DirectSound buffers. */
#define DMUS_PORT_FEATURE_STREAMING     0x00000002	/* Supports streaming waves through the synth. */

typedef DMUS_PORTPARAMS8 DMUS_PORTPARAMS;
typedef DMUS_PORTPARAMS *LPDMUS_PORTPARAMS;

typedef struct _DMUS_CLOCKINFO8 *LPDMUS_CLOCKINFO8;
typedef struct _DMUS_CLOCKINFO8
{
    DWORD           dwSize;
    DMUS_CLOCKTYPE  ctType;
    GUID            guidClock;          /* Identifies this time source */
    WCHAR           wszDescription[DMUS_MAX_DESCRIPTION];
    DWORD           dwFlags;
} DMUS_CLOCKINFO8;

typedef DMUS_CLOCKINFO8 DMUS_CLOCKINFO;
typedef DMUS_CLOCKINFO *LPDMUS_CLOCKINFO;

typedef struct _DMUS_PMSG
{
    /* begin DMUS_PMSG_PART */
    DMUS_PMSG_PART
    /* end DMUS_PMSG_PART */

} DMUS_PMSG;

typedef struct _DMUS_NOTIFICATION_PMSG
{
    /* begin DMUS_PMSG_PART */
    DMUS_PMSG_PART
    /* end DMUS_PMSG_PART */

    GUID    guidNotificationType;
    DWORD   dwNotificationOption;
    DWORD   dwField1;
    DWORD   dwField2;
} DMUS_NOTIFICATION_PMSG;

typedef struct _DMUS_VERSION {
  DWORD    dwVersionMS;
  DWORD    dwVersionLS;
}DMUS_VERSION, FAR *LPDMUS_VERSION;

typedef struct _DMUS_OBJECTDESC
{
    DWORD          dwSize;                 /* Size of this structure. */
    DWORD          dwValidData;            /* Flags indicating which fields below are valid. */
    GUID           guidObject;             /* Unique ID for this object. */
    GUID           guidClass;              /* GUID for the class of object. */
    FILETIME       ftDate;                 /* Last edited date of object. */
    DMUS_VERSION   vVersion;               /* Version. */
    WCHAR          wszName[DMUS_MAX_NAME]; /* Name of object. */
    WCHAR          wszCategory[DMUS_MAX_CATEGORY]; /* Category for object (optional). */
    WCHAR          wszFileName[DMUS_MAX_FILENAME]; /* File path. */
    LONGLONG       llMemLength;            /* Size of Memory data. */
    LPBYTE         pbMemData;              /* Memory pointer for data. */
    IStream *      pStream;                /* Stream with data. */
} DMUS_OBJECTDESC;

typedef DMUS_OBJECTDESC *LPDMUS_OBJECTDESC;

//-------------------------------------------------------------

typedef int DMUS_NOTERANGE;
typedef int DMUS_CHORD_KEY;
typedef int DMUS_TIMESIGNATURE;
typedef int DMUS_AUDIOPARAMS;

//-------------------------------------------------------------

DEFINE_GUID(CLSID_DirectMusic,0x636b9f10,0x0c7d,0x11d1,0x95,0xb2,0x00,0x20,0xaf,0xdc,0x74,0x21);
DEFINE_GUID(CLSID_DirectMusicPerformance,0xd2ac2881, 0xb39b, 0x11d1, 0x87, 0x4, 0x0, 0x60, 0x8, 0x93, 0xb1, 0xbd);
DEFINE_GUID(CLSID_DirectMusicLoader,0xd2ac2892, 0xb39b, 0x11d1, 0x87, 0x4, 0x0, 0x60, 0x8, 0x93, 0xb1, 0xbd);
DEFINE_GUID(CLSID_DirectMusicSegment,0xd2ac2882, 0xb39b, 0x11d1, 0x87, 0x4, 0x0, 0x60, 0x8, 0x93, 0xb1, 0xbd);

DEFINE_GUID(GUID_DirectMusicAllTypes,0xd2ac2893, 0xb39b, 0x11d1, 0x87, 0x4, 0x0, 0x60, 0x8, 0x93, 0xb1, 0xbd);
DEFINE_GUID(GUID_StandardMIDIFile, 0x6621075, 0xe92e, 0x11d1, 0xa8, 0xc5, 0x0, 0xc0, 0x4f, 0xa3, 0x72, 0x6e);
DEFINE_GUID(GUID_PerfMasterVolume,0xd2ac28b1, 0xb39b, 0x11d1, 0x87, 0x4, 0x0, 0x60, 0x8, 0x93, 0xb1, 0xbd);

DEFINE_GUID(IID_IDirectMusic,0x6536115a,0x7b2d,0x11d2,0xba,0x18,0x00,0x00,0xf8,0x75,0xac,0x12);
DEFINE_GUID(IID_IDirectMusicLoader8, 0x19e7c08c, 0xa44, 0x4e6a, 0xa1, 0x16, 0x59, 0x5a, 0x7c, 0xd5, 0xde, 0x8c);
DEFINE_GUID(IID_IDirectMusicPerformance8, 0x679c4137, 0xc62e, 0x4147, 0xb2, 0xb4, 0x9d, 0x56, 0x9a, 0xcb, 0x25, 0x4c);
DEFINE_GUID(IID_IDirectMusicSegment8,0xc6784488, 0x41a3, 0x418f, 0xaa, 0x15, 0xb3, 0x50, 0x93, 0xba, 0x42, 0xd4);
DEFINE_GUID(IID_IDirectMusicSegmentState8, 0xa50e4730, 0xae4, 0x48a7, 0x98, 0x39, 0xbc, 0x4, 0xbf, 0xe0, 0x77, 0x72);


//-------------------------------------------------------------

#undef  INTERFACE
#define INTERFACE  IDirectMusic
DECLARE_INTERFACE_(IDirectMusic, IUnknown)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusic */
    STDMETHOD(EnumPort)             (THIS_ DWORD dwIndex,
                                           LPDMUS_PORTCAPS pPortCaps) PURE;
    STDMETHOD(CreateMusicBuffer)    (THIS_ LPDMUS_BUFFERDESC pBufferDesc,
                                           LPDIRECTMUSICBUFFER *ppBuffer,
                                           LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(CreatePort)           (THIS_ REFCLSID rclsidPort,
                                           LPDMUS_PORTPARAMS pPortParams,
                                           LPDIRECTMUSICPORT *ppPort,
                                           LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(EnumMasterClock)      (THIS_ DWORD dwIndex,
                                           LPDMUS_CLOCKINFO lpClockInfo) PURE;
    STDMETHOD(GetMasterClock)       (THIS_ LPGUID pguidClock,
                                           IReferenceClock **ppReferenceClock) PURE;
    STDMETHOD(SetMasterClock)       (THIS_ REFGUID rguidClock) PURE;
    STDMETHOD(Activate)             (THIS_ BOOL fEnable) PURE;
    STDMETHOD(GetDefaultPort)       (THIS_ LPGUID pguidPort) PURE;
    STDMETHOD(SetDirectSound)       (THIS_ LPDIRECTSOUND pDirectSound,
                                           HWND hWnd) PURE;
};

#undef  INTERFACE
#define INTERFACE  IDirectMusic8
DECLARE_INTERFACE_(IDirectMusic8, IDirectMusic)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusic */
    STDMETHOD(EnumPort)             (THIS_ DWORD dwIndex,
                                           LPDMUS_PORTCAPS pPortCaps) PURE;
    STDMETHOD(CreateMusicBuffer)    (THIS_ LPDMUS_BUFFERDESC pBufferDesc,
                                           LPDIRECTMUSICBUFFER *ppBuffer,
                                           LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(CreatePort)           (THIS_ REFCLSID rclsidPort,
                                           LPDMUS_PORTPARAMS pPortParams,
                                           LPDIRECTMUSICPORT *ppPort,
                                           LPUNKNOWN pUnkOuter) PURE;
    STDMETHOD(EnumMasterClock)      (THIS_ DWORD dwIndex,
                                           LPDMUS_CLOCKINFO lpClockInfo) PURE;
    STDMETHOD(GetMasterClock)       (THIS_ LPGUID pguidClock,
                                           IReferenceClock **ppReferenceClock) PURE;
    STDMETHOD(SetMasterClock)       (THIS_ REFGUID rguidClock) PURE;
    STDMETHOD(Activate)             (THIS_ BOOL fEnable) PURE;
    STDMETHOD(GetDefaultPort)       (THIS_ LPGUID pguidPort) PURE;
    STDMETHOD(SetDirectSound)       (THIS_ LPDIRECTSOUND pDirectSound,
                                           HWND hWnd) PURE;
    /*  IDirectMusic8 */
    STDMETHOD(SetExternalMasterClock)
                                    (THIS_ IReferenceClock *pClock) PURE;
};

#undef  INTERFACE
#define INTERFACE  IDirectMusicSegment
DECLARE_INTERFACE_(IDirectMusicSegment, IUnknown)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicSegment */
    STDMETHOD(GetLength)                (THIS_ MUSIC_TIME* pmtLength) PURE;
    STDMETHOD(SetLength)                (THIS_ MUSIC_TIME mtLength) PURE;
    STDMETHOD(GetRepeats)               (THIS_ DWORD* pdwRepeats) PURE;
    STDMETHOD(SetRepeats)               (THIS_ DWORD  dwRepeats) PURE;
    STDMETHOD(GetDefaultResolution)     (THIS_ DWORD* pdwResolution) PURE;
    STDMETHOD(SetDefaultResolution)     (THIS_ DWORD  dwResolution) PURE;
    STDMETHOD(GetTrack)                 (THIS_ REFGUID rguidType,
                                               DWORD dwGroupBits,
                                               DWORD dwIndex,
                                               IDirectMusicTrack** ppTrack) PURE;
    STDMETHOD(GetTrackGroup)            (THIS_ IDirectMusicTrack* pTrack,
                                               DWORD* pdwGroupBits) PURE;
    STDMETHOD(InsertTrack)              (THIS_ IDirectMusicTrack* pTrack,
                                               DWORD dwGroupBits) PURE;
    STDMETHOD(RemoveTrack)              (THIS_ IDirectMusicTrack* pTrack) PURE;
    STDMETHOD(InitPlay)                 (THIS_ IDirectMusicSegmentState** ppSegState,
                                               IDirectMusicPerformance* pPerformance,
                                               DWORD dwFlags) PURE;
    STDMETHOD(GetGraph)                 (THIS_ IDirectMusicGraph** ppGraph) PURE;
    STDMETHOD(SetGraph)                 (THIS_ IDirectMusicGraph* pGraph) PURE;
    STDMETHOD(AddNotificationType)      (THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(RemoveNotificationType)   (THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(GetParam)                 (THIS_ REFGUID rguidType,
                                               DWORD dwGroupBits,
                                               DWORD dwIndex,
                                               MUSIC_TIME mtTime,
                                               MUSIC_TIME* pmtNext,
                                               void* pParam) PURE;
    STDMETHOD(SetParam)                 (THIS_ REFGUID rguidType,
                                               DWORD dwGroupBits,
                                               DWORD dwIndex,
                                               MUSIC_TIME mtTime,
                                               void* pParam) PURE;
    STDMETHOD(Clone)                    (THIS_ MUSIC_TIME mtStart,
                                               MUSIC_TIME mtEnd,
                                               IDirectMusicSegment** ppSegment) PURE;
    STDMETHOD(SetStartPoint)            (THIS_ MUSIC_TIME mtStart) PURE;
    STDMETHOD(GetStartPoint)            (THIS_ MUSIC_TIME* pmtStart) PURE;
    STDMETHOD(SetLoopPoints)            (THIS_ MUSIC_TIME mtStart,
                                               MUSIC_TIME mtEnd) PURE;
    STDMETHOD(GetLoopPoints)            (THIS_ MUSIC_TIME* pmtStart,
                                               MUSIC_TIME* pmtEnd) PURE;
    STDMETHOD(SetPChannelsUsed)         (THIS_ DWORD dwNumPChannels,
                                               DWORD* paPChannels) PURE;
};

#undef  INTERFACE
#define INTERFACE  IDirectMusicSegment8
DECLARE_INTERFACE_(IDirectMusicSegment8, IDirectMusicSegment)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicSegment */
    STDMETHOD(GetLength)                (THIS_ MUSIC_TIME* pmtLength) PURE;
    STDMETHOD(SetLength)                (THIS_ MUSIC_TIME mtLength) PURE;
    STDMETHOD(GetRepeats)               (THIS_ DWORD* pdwRepeats) PURE;
    STDMETHOD(SetRepeats)               (THIS_ DWORD  dwRepeats) PURE;
    STDMETHOD(GetDefaultResolution)     (THIS_ DWORD* pdwResolution) PURE;
    STDMETHOD(SetDefaultResolution)     (THIS_ DWORD  dwResolution) PURE;
    STDMETHOD(GetTrack)                 (THIS_ REFGUID rguidType,
                                               DWORD dwGroupBits,
                                               DWORD dwIndex,
                                               IDirectMusicTrack** ppTrack) PURE;
    STDMETHOD(GetTrackGroup)            (THIS_ IDirectMusicTrack* pTrack,
                                               DWORD* pdwGroupBits) PURE;
    STDMETHOD(InsertTrack)              (THIS_ IDirectMusicTrack* pTrack,
                                               DWORD dwGroupBits) PURE;
    STDMETHOD(RemoveTrack)              (THIS_ IDirectMusicTrack* pTrack) PURE;
    STDMETHOD(InitPlay)                 (THIS_ IDirectMusicSegmentState** ppSegState,
                                               IDirectMusicPerformance* pPerformance,
                                               DWORD dwFlags) PURE;
    STDMETHOD(GetGraph)                 (THIS_ IDirectMusicGraph** ppGraph) PURE;
    STDMETHOD(SetGraph)                 (THIS_ IDirectMusicGraph* pGraph) PURE;
    STDMETHOD(AddNotificationType)      (THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(RemoveNotificationType)   (THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(GetParam)                 (THIS_ REFGUID rguidType,
                                               DWORD dwGroupBits,
                                               DWORD dwIndex,
                                               MUSIC_TIME mtTime,
                                               MUSIC_TIME* pmtNext,
                                               void* pParam) PURE;
    STDMETHOD(SetParam)                 (THIS_ REFGUID rguidType,
                                               DWORD dwGroupBits,
                                               DWORD dwIndex,
                                               MUSIC_TIME mtTime,
                                               void* pParam) PURE;
    STDMETHOD(Clone)                    (THIS_ MUSIC_TIME mtStart,
                                               MUSIC_TIME mtEnd,
                                               IDirectMusicSegment** ppSegment) PURE;
    STDMETHOD(SetStartPoint)            (THIS_ MUSIC_TIME mtStart) PURE;
    STDMETHOD(GetStartPoint)            (THIS_ MUSIC_TIME* pmtStart) PURE;
    STDMETHOD(SetLoopPoints)            (THIS_ MUSIC_TIME mtStart,
                                               MUSIC_TIME mtEnd) PURE;
    STDMETHOD(GetLoopPoints)            (THIS_ MUSIC_TIME* pmtStart,
                                               MUSIC_TIME* pmtEnd) PURE;
    STDMETHOD(SetPChannelsUsed)         (THIS_ DWORD dwNumPChannels,
                                               DWORD* paPChannels) PURE;
    /*  IDirectMusicSegment8 */
    STDMETHOD(SetTrackConfig)           (THIS_ REFGUID rguidTrackClassID,   /* Class ID of the type of track on which to set the configuration flags. */
                                               DWORD dwGroupBits,           /* Group bits. */
                                               DWORD dwIndex,               /* Nth track (or DMUS_SEG_ALLTRACKS) that matches class id and group id. */
                                               DWORD dwFlagsOn,             /* DMUS_TRACKCONFIG_ flags to enable. */
                                               DWORD dwFlagsOff) PURE;      /* DMUS_TRACKCONFIG_ flags to disable. */
    STDMETHOD(GetAudioPathConfig)       (THIS_ IUnknown ** ppAudioPathConfig) PURE;
    STDMETHOD(Compose)                  (THIS_ MUSIC_TIME mtTime,
                                               IDirectMusicSegment* pFromSegment,
                                               IDirectMusicSegment* pToSegment,
                                               IDirectMusicSegment** ppComposedSegment) PURE;
    STDMETHOD(Download)                 (THIS_ IUnknown *pAudioPath) PURE;
    STDMETHOD(Unload)                   (THIS_ IUnknown *pAudioPath) PURE;
};

#undef  INTERFACE
#define INTERFACE  IDirectMusicSegmentState
DECLARE_INTERFACE_(IDirectMusicSegmentState, IUnknown)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicSegmentState */
    STDMETHOD(GetRepeats)           (THIS_ DWORD* pdwRepeats) PURE;
    STDMETHOD(GetSegment )          (THIS_ IDirectMusicSegment** ppSegment) PURE;
    STDMETHOD(GetStartTime)         (THIS_ MUSIC_TIME* pmtStart) PURE;
    STDMETHOD(GetSeek)              (THIS_ MUSIC_TIME* pmtSeek) PURE;
    STDMETHOD(GetStartPoint)        (THIS_ MUSIC_TIME* pmtStart) PURE;
};

#undef  INTERFACE
#define INTERFACE  IDirectMusicSegmentState8
DECLARE_INTERFACE_(IDirectMusicSegmentState8, IDirectMusicSegmentState)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicSegmentState */
    STDMETHOD(GetRepeats)           (THIS_ DWORD* pdwRepeats) PURE;
    STDMETHOD(GetSegment )          (THIS_ IDirectMusicSegment** ppSegment) PURE;
    STDMETHOD(GetStartTime)         (THIS_ MUSIC_TIME* pmtStart) PURE;
    STDMETHOD(GetSeek)              (THIS_ MUSIC_TIME* pmtSeek) PURE;
    STDMETHOD(GetStartPoint)        (THIS_ MUSIC_TIME* pmtStart) PURE;

    /* IDirectMusicSegmentState8 */
    STDMETHOD(SetTrackConfig)       (THIS_ REFGUID rguidTrackClassID,   /* Class ID of the type of track on which to set the configuration flags. */
                                           DWORD dwGroupBits,           /* Group bits. */
                                           DWORD dwIndex,               /* Nth track (or DMUS_SEG_ALLTRACKS) that matches class id and group id. */
                                           DWORD dwFlagsOn,             /* DMUS_TRACKCONFIG_ flags to enable. */
                                           DWORD dwFlagsOff) PURE;      /* DMUS_TRACKCONFIG_ flags to disable. */
    STDMETHOD(GetObjectInPath)      (THIS_ DWORD dwPChannel,    /* PChannel to search. */
                                           DWORD dwStage,       /* Which stage in the path. */
                                           DWORD dwBuffer,      /* Which buffer to address, if more than one. */
                                           REFGUID guidObject,  /* ClassID of object. */
                                           DWORD dwIndex,       /* Which object of that class. */
                                           REFGUID iidInterface,/* Requested COM interface. */
                                           void ** ppObject) PURE; /* Pointer to interface. */
};


#undef  INTERFACE
#define INTERFACE  IDirectMusicPerformance
DECLARE_INTERFACE_(IDirectMusicPerformance, IUnknown)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicPerformance */
    STDMETHOD(Init)                 (THIS_ IDirectMusic** ppDirectMusic,
                                           LPDIRECTSOUND pDirectSound,
                                           HWND hWnd) PURE;
    STDMETHOD(PlaySegment)          (THIS_ IDirectMusicSegment* pSegment,
                                           DWORD dwFlags,
                                           __int64 i64StartTime,
                                           IDirectMusicSegmentState** ppSegmentState) PURE;
    STDMETHOD(Stop)                 (THIS_ IDirectMusicSegment* pSegment,
                                           IDirectMusicSegmentState* pSegmentState,
                                           MUSIC_TIME mtTime,
                                           DWORD dwFlags) PURE;
    STDMETHOD(GetSegmentState)      (THIS_ IDirectMusicSegmentState** ppSegmentState,
                                           MUSIC_TIME mtTime) PURE;
    STDMETHOD(SetPrepareTime)       (THIS_ DWORD dwMilliSeconds) PURE;
    STDMETHOD(GetPrepareTime)       (THIS_ DWORD* pdwMilliSeconds) PURE;
    STDMETHOD(SetBumperLength)      (THIS_ DWORD dwMilliSeconds) PURE;
    STDMETHOD(GetBumperLength)      (THIS_ DWORD* pdwMilliSeconds) PURE;
    STDMETHOD(SendPMsg)             (THIS_ DMUS_PMSG* pPMSG) PURE;
    STDMETHOD(MusicToReferenceTime) (THIS_ MUSIC_TIME mtTime,
                                           REFERENCE_TIME* prtTime) PURE;
    STDMETHOD(ReferenceToMusicTime) (THIS_ REFERENCE_TIME rtTime,
                                           MUSIC_TIME* pmtTime) PURE;
    STDMETHOD(IsPlaying)            (THIS_ IDirectMusicSegment* pSegment,
                                           IDirectMusicSegmentState* pSegState) PURE;
    STDMETHOD(GetTime)              (THIS_ REFERENCE_TIME* prtNow,
                                           MUSIC_TIME* pmtNow) PURE;
    STDMETHOD(AllocPMsg)            (THIS_ ULONG cb,
                                           DMUS_PMSG** ppPMSG) PURE;
    STDMETHOD(FreePMsg)             (THIS_ DMUS_PMSG* pPMSG) PURE;
    STDMETHOD(GetGraph)             (THIS_ IDirectMusicGraph** ppGraph) PURE;
    STDMETHOD(SetGraph)             (THIS_ IDirectMusicGraph* pGraph) PURE;
    STDMETHOD(SetNotificationHandle)(THIS_ HANDLE hNotification,
                                           REFERENCE_TIME rtMinimum) PURE;
    STDMETHOD(GetNotificationPMsg)  (THIS_ DMUS_NOTIFICATION_PMSG** ppNotificationPMsg) PURE;
    STDMETHOD(AddNotificationType)  (THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(RemoveNotificationType)(THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(AddPort)              (THIS_ IDirectMusicPort* pPort) PURE;
    STDMETHOD(RemovePort)           (THIS_ IDirectMusicPort* pPort ) PURE;
    STDMETHOD(AssignPChannelBlock)  (THIS_ DWORD dwBlockNum,
                                           IDirectMusicPort* pPort,
                                           DWORD dwGroup ) PURE;
    STDMETHOD(AssignPChannel)       (THIS_ DWORD dwPChannel,
                                           IDirectMusicPort* pPort,
                                           DWORD dwGroup,
                                           DWORD dwMChannel ) PURE;
    STDMETHOD(PChannelInfo)         (THIS_ DWORD dwPChannel,
                                           IDirectMusicPort** ppPort,
                                           DWORD* pdwGroup,
                                           DWORD* pdwMChannel ) PURE;
    STDMETHOD(DownloadInstrument)   (THIS_ IDirectMusicInstrument* pInst,
                                           DWORD dwPChannel,
                                           IDirectMusicDownloadedInstrument** ppDownInst,
                                           DMUS_NOTERANGE* pNoteRanges,
                                           DWORD dwNumNoteRanges,
                                           IDirectMusicPort** ppPort,
                                           DWORD* pdwGroup,
                                           DWORD* pdwMChannel ) PURE;
    STDMETHOD(Invalidate)           (THIS_ MUSIC_TIME mtTime,
                                           DWORD dwFlags) PURE;
    STDMETHOD(GetParam)             (THIS_ REFGUID rguidType,
                                           DWORD dwGroupBits,
                                           DWORD dwIndex,
                                           MUSIC_TIME mtTime,
                                           MUSIC_TIME* pmtNext,
                                           void* pParam) PURE;
    STDMETHOD(SetParam)             (THIS_ REFGUID rguidType,
                                           DWORD dwGroupBits,
                                           DWORD dwIndex,
                                           MUSIC_TIME mtTime,
                                           void* pParam) PURE;
    STDMETHOD(GetGlobalParam)       (THIS_ REFGUID rguidType,
                                           void* pParam,
                                           DWORD dwSize) PURE;
    STDMETHOD(SetGlobalParam)       (THIS_ REFGUID rguidType,
                                           void* pParam,
                                           DWORD dwSize) PURE;
    STDMETHOD(GetLatencyTime)       (THIS_ REFERENCE_TIME* prtTime) PURE;
    STDMETHOD(GetQueueTime)         (THIS_ REFERENCE_TIME* prtTime) PURE;
    STDMETHOD(AdjustTime)           (THIS_ REFERENCE_TIME rtAmount) PURE;
    STDMETHOD(CloseDown)            (THIS) PURE;
    STDMETHOD(GetResolvedTime)      (THIS_ REFERENCE_TIME rtTime,
                                           REFERENCE_TIME* prtResolved,
                                           DWORD dwTimeResolveFlags) PURE;
    STDMETHOD(MIDIToMusic)          (THIS_ BYTE bMIDIValue,
                                           DMUS_CHORD_KEY* pChord,
                                           BYTE bPlayMode,
                                           BYTE bChordLevel,
                                           WORD *pwMusicValue) PURE;
    STDMETHOD(MusicToMIDI)          (THIS_ WORD wMusicValue,
                                           DMUS_CHORD_KEY* pChord,
                                           BYTE bPlayMode,
                                           BYTE bChordLevel,
                                           BYTE *pbMIDIValue) PURE;
    STDMETHOD(TimeToRhythm)         (THIS_ MUSIC_TIME mtTime,
                                           DMUS_TIMESIGNATURE *pTimeSig,
                                           WORD *pwMeasure,
                                           BYTE *pbBeat,
                                           BYTE *pbGrid,
                                           short *pnOffset) PURE;
    STDMETHOD(RhythmToTime)         (THIS_ WORD wMeasure,
                                           BYTE bBeat,
                                           BYTE bGrid,
                                           short nOffset,
                                           DMUS_TIMESIGNATURE *pTimeSig,
                                           MUSIC_TIME *pmtTime) PURE;
};


#undef  INTERFACE
#define INTERFACE  IDirectMusicPerformance8
DECLARE_INTERFACE_(IDirectMusicPerformance8, IDirectMusicPerformance)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicPerformance */
    STDMETHOD(Init)                 (THIS_ IDirectMusic** ppDirectMusic,
                                           LPDIRECTSOUND pDirectSound,
                                           HWND hWnd) PURE;
    STDMETHOD(PlaySegment)          (THIS_ IDirectMusicSegment* pSegment,
                                           DWORD dwFlags,
                                           __int64 i64StartTime,
                                           IDirectMusicSegmentState** ppSegmentState) PURE;
    STDMETHOD(Stop)                 (THIS_ IDirectMusicSegment* pSegment,
                                           IDirectMusicSegmentState* pSegmentState,
                                           MUSIC_TIME mtTime,
                                           DWORD dwFlags) PURE;
    STDMETHOD(GetSegmentState)      (THIS_ IDirectMusicSegmentState** ppSegmentState,
                                           MUSIC_TIME mtTime) PURE;
    STDMETHOD(SetPrepareTime)       (THIS_ DWORD dwMilliSeconds) PURE;
    STDMETHOD(GetPrepareTime)       (THIS_ DWORD* pdwMilliSeconds) PURE;
    STDMETHOD(SetBumperLength)      (THIS_ DWORD dwMilliSeconds) PURE;
    STDMETHOD(GetBumperLength)      (THIS_ DWORD* pdwMilliSeconds) PURE;
    STDMETHOD(SendPMsg)             (THIS_ DMUS_PMSG* pPMSG) PURE;
    STDMETHOD(MusicToReferenceTime) (THIS_ MUSIC_TIME mtTime,
                                           REFERENCE_TIME* prtTime) PURE;
    STDMETHOD(ReferenceToMusicTime) (THIS_ REFERENCE_TIME rtTime,
                                           MUSIC_TIME* pmtTime) PURE;
    STDMETHOD(IsPlaying)            (THIS_ IDirectMusicSegment* pSegment,
                                           IDirectMusicSegmentState* pSegState) PURE;
    STDMETHOD(GetTime)              (THIS_ REFERENCE_TIME* prtNow,
                                           MUSIC_TIME* pmtNow) PURE;
    STDMETHOD(AllocPMsg)            (THIS_ ULONG cb,
                                           DMUS_PMSG** ppPMSG) PURE;
    STDMETHOD(FreePMsg)             (THIS_ DMUS_PMSG* pPMSG) PURE;
    STDMETHOD(GetGraph)             (THIS_ IDirectMusicGraph** ppGraph) PURE;
    STDMETHOD(SetGraph)             (THIS_ IDirectMusicGraph* pGraph) PURE;
    STDMETHOD(SetNotificationHandle)(THIS_ HANDLE hNotification,
                                           REFERENCE_TIME rtMinimum) PURE;
    STDMETHOD(GetNotificationPMsg)  (THIS_ DMUS_NOTIFICATION_PMSG** ppNotificationPMsg) PURE;
    STDMETHOD(AddNotificationType)  (THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(RemoveNotificationType)(THIS_ REFGUID rguidNotificationType) PURE;
    STDMETHOD(AddPort)              (THIS_ IDirectMusicPort* pPort) PURE;
    STDMETHOD(RemovePort)           (THIS_ IDirectMusicPort* pPort ) PURE;
    STDMETHOD(AssignPChannelBlock)  (THIS_ DWORD dwBlockNum,
                                           IDirectMusicPort* pPort,
                                           DWORD dwGroup ) PURE;
    STDMETHOD(AssignPChannel)       (THIS_ DWORD dwPChannel,
                                           IDirectMusicPort* pPort,
                                           DWORD dwGroup,
                                           DWORD dwMChannel ) PURE;
    STDMETHOD(PChannelInfo)         (THIS_ DWORD dwPChannel,
                                           IDirectMusicPort** ppPort,
                                           DWORD* pdwGroup,
                                           DWORD* pdwMChannel ) PURE;
    STDMETHOD(DownloadInstrument)   (THIS_ IDirectMusicInstrument* pInst,
                                           DWORD dwPChannel,
                                           IDirectMusicDownloadedInstrument** ppDownInst,
                                           DMUS_NOTERANGE* pNoteRanges,
                                           DWORD dwNumNoteRanges,
                                           IDirectMusicPort** ppPort,
                                           DWORD* pdwGroup,
                                           DWORD* pdwMChannel ) PURE;
    STDMETHOD(Invalidate)           (THIS_ MUSIC_TIME mtTime,
                                           DWORD dwFlags) PURE;
    STDMETHOD(GetParam)             (THIS_ REFGUID rguidType,
                                           DWORD dwGroupBits,
                                           DWORD dwIndex,
                                           MUSIC_TIME mtTime,
                                           MUSIC_TIME* pmtNext,
                                           void* pParam) PURE;
    STDMETHOD(SetParam)             (THIS_ REFGUID rguidType,
                                           DWORD dwGroupBits,
                                           DWORD dwIndex,
                                           MUSIC_TIME mtTime,
                                           void* pParam) PURE;
    STDMETHOD(GetGlobalParam)       (THIS_ REFGUID rguidType,
                                           void* pParam,
                                           DWORD dwSize) PURE;
    STDMETHOD(SetGlobalParam)       (THIS_ REFGUID rguidType,
                                           void* pParam,
                                           DWORD dwSize) PURE;
    STDMETHOD(GetLatencyTime)       (THIS_ REFERENCE_TIME* prtTime) PURE;
    STDMETHOD(GetQueueTime)         (THIS_ REFERENCE_TIME* prtTime) PURE;
    STDMETHOD(AdjustTime)           (THIS_ REFERENCE_TIME rtAmount) PURE;
    STDMETHOD(CloseDown)            (THIS) PURE;
    STDMETHOD(GetResolvedTime)      (THIS_ REFERENCE_TIME rtTime,
                                           REFERENCE_TIME* prtResolved,
                                           DWORD dwTimeResolveFlags) PURE;
    STDMETHOD(MIDIToMusic)          (THIS_ BYTE bMIDIValue,
                                           DMUS_CHORD_KEY* pChord,
                                           BYTE bPlayMode,
                                           BYTE bChordLevel,
                                           WORD *pwMusicValue) PURE;
    STDMETHOD(MusicToMIDI)          (THIS_ WORD wMusicValue,
                                           DMUS_CHORD_KEY* pChord,
                                           BYTE bPlayMode,
                                           BYTE bChordLevel,
                                           BYTE *pbMIDIValue) PURE;
    STDMETHOD(TimeToRhythm)         (THIS_ MUSIC_TIME mtTime,
                                           DMUS_TIMESIGNATURE *pTimeSig,
                                           WORD *pwMeasure,
                                           BYTE *pbBeat,
                                           BYTE *pbGrid,
                                           short *pnOffset) PURE;
    STDMETHOD(RhythmToTime)         (THIS_ WORD wMeasure,
                                           BYTE bBeat,
                                           BYTE bGrid,
                                           short nOffset,
                                           DMUS_TIMESIGNATURE *pTimeSig,
                                           MUSIC_TIME *pmtTime) PURE;
    /*  IDirectMusicPerformance8 */
    STDMETHOD(InitAudio)            (THIS_ IDirectMusic** ppDirectMusic,            /* Optional DirectMusic pointer. */
                                           IDirectSound** ppDirectSound,            /* Optional DirectSound pointer. */
                                           HWND hWnd,                               /* HWND for DirectSound. */
                                           DWORD dwDefaultPathType,                 /* Requested default audio path type, also optional. */
                                           DWORD dwPChannelCount,                   /* Number of PChannels, if default audio path to be created. */
                                           DWORD dwFlags,                           /* DMUS_AUDIOF flags, if no pParams structure. */
                                           DMUS_AUDIOPARAMS *pParams) PURE;         /* Optional initialization structure, defining required voices, buffers, etc. */
    STDMETHOD(PlaySegmentEx)        (THIS_ IUnknown* pSource,                       /* Segment to play. */
                                           WCHAR *pwzSegmentName,                   /* Not supported in DX8. */
                                           IUnknown* pTransition,                   /* Optional template segment to compose transition with. */
                                           DWORD dwFlags,                           /* DMUS_SEGF_ flags. */
                                           __int64 i64StartTime,                    /* Time to start playback. */
                                           IDirectMusicSegmentState** ppSegmentState, /* Returned Segment State. */
                                           IUnknown *pFrom,                         /* Optional segmentstate or audiopath to replace. */
                                           IUnknown *pAudioPath) PURE;              /* Optional audioPath to play on. */
    STDMETHOD(StopEx)               (THIS_ IUnknown *pObjectToStop,                 /* Segstate, AudioPath, or Segment. */
                                           __int64 i64StopTime,
                                           DWORD dwFlags) PURE;
    STDMETHOD(ClonePMsg)            (THIS_ DMUS_PMSG* pSourcePMSG,
                                           DMUS_PMSG** ppCopyPMSG) PURE;
    STDMETHOD(CreateAudioPath)      (THIS_ IUnknown *pSourceConfig,                 /* Source configuration, from AudioPathConfig file. */
                                           BOOL fActivate,                          /* TRUE to activate on creation. */
                                           IDirectMusicAudioPath **ppNewPath) PURE; /* Returns created audiopath. */
    STDMETHOD(CreateStandardAudioPath)(THIS_ DWORD dwType,                          /* Type of path to create. */
                                           DWORD dwPChannelCount,                   /* How many PChannels to allocate for it. */
                                           BOOL fActivate,                          /* TRUE to activate on creation. */
                                           IDirectMusicAudioPath **ppNewPath) PURE; /* Returns created audiopath. */
    STDMETHOD(SetDefaultAudioPath)  (THIS_ IDirectMusicAudioPath *pAudioPath) PURE;
    STDMETHOD(GetDefaultAudioPath)  (THIS_ IDirectMusicAudioPath **ppAudioPath) PURE;
    STDMETHOD(GetParamEx)           (THIS_ REFGUID rguidType,                       /* GetParam command ID. */
                                           DWORD dwTrackID,                         /* Virtual track ID of caller. */
                                           DWORD dwGroupBits,                       /* Group bits of caller. */
                                           DWORD dwIndex,                           /* Index to Nth parameter. */
                                           MUSIC_TIME mtTime,                       /* Time of requested parameter. */
                                           MUSIC_TIME* pmtNext,                     /* Returned delta to next parameter. */
                                           void* pParam) PURE;                      /* Data structure to fill with parameter. */
};

#undef  INTERFACE
#define INTERFACE  IDirectMusicAudioPath
DECLARE_INTERFACE_(IDirectMusicAudioPath, IUnknown)
{
    /*  IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /*  IDirectMusicAudioPath */
    STDMETHOD(GetObjectInPath)      (THIS_ DWORD dwPChannel,    /* PChannel to search. */
                                           DWORD dwStage,       /* Which stage in the path. */
                                           DWORD dwBuffer,      /* Which buffer to address, if more than one. */
                                           REFGUID guidObject,  /* ClassID of object. */
                                           DWORD dwIndex,       /* Which object of that class. */
                                           REFGUID iidInterface,/* Requested COM interface. */
                                           void ** ppObject) PURE; /* Pointer to interface. */
    STDMETHOD(Activate)             (THIS_ BOOL fActivate) PURE;/* True to activate, False to deactivate. */
    STDMETHOD(SetVolume)            (THIS_ long lVolume,        /* Gain, in 100ths of a dB. This must be negative (0 represents full volume.) */
                                           DWORD dwDuration) PURE;/* Duration of volume ramp in  milliseconds. Note that 0 is more efficient. */
    STDMETHOD(ConvertPChannel)      (THIS_ DWORD dwPChannelIn,   /* Pchannel of source. */
                                           DWORD *pdwPChannelOut) PURE; /* Equivalent pchannel on performance. */
};

typedef IDirectMusicAudioPath IDirectMusicAudioPath8;

#undef  INTERFACE
#define INTERFACE  IDirectMusicLoader
DECLARE_INTERFACE_(IDirectMusicLoader, IUnknown)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /* IDirectMusicLoader */
    STDMETHOD(GetObject)            (THIS_ LPDMUS_OBJECTDESC pDesc,
                                           REFIID riid,
                                           LPVOID FAR *ppv) PURE;
    STDMETHOD(SetObject)            (THIS_ LPDMUS_OBJECTDESC pDesc) PURE;
    STDMETHOD(SetSearchDirectory)   (THIS_ REFGUID rguidClass,
                                           WCHAR *pwzPath,
                                           BOOL fClear) PURE;
    STDMETHOD(ScanDirectory)        (THIS_ REFGUID rguidClass,
                                           WCHAR *pwzFileExtension,
                                           WCHAR *pwzScanFileName) PURE;
    STDMETHOD(CacheObject)          (THIS_ IDirectMusicObject * pObject) PURE;
    STDMETHOD(ReleaseObject)        (THIS_ IDirectMusicObject * pObject) PURE;
    STDMETHOD(ClearCache)           (THIS_ REFGUID rguidClass) PURE;
    STDMETHOD(EnableCache)          (THIS_ REFGUID rguidClass,
                                           BOOL fEnable) PURE;
    STDMETHOD(EnumObject)           (THIS_ REFGUID rguidClass,
                                           DWORD dwIndex,
                                           LPDMUS_OBJECTDESC pDesc) PURE;
};

#undef  INTERFACE
#define INTERFACE  IDirectMusicLoader8
DECLARE_INTERFACE_(IDirectMusicLoader8, IDirectMusicLoader)
{
    /* IUnknown */
    STDMETHOD(QueryInterface)       (THIS_ REFIID, LPVOID FAR *) PURE;
    STDMETHOD_(ULONG,AddRef)        (THIS) PURE;
    STDMETHOD_(ULONG,Release)       (THIS) PURE;

    /* IDirectMusicLoader */
    STDMETHOD(GetObject)            (THIS_ LPDMUS_OBJECTDESC pDesc,
                                           REFIID riid,
                                           LPVOID FAR *ppv) PURE;
    STDMETHOD(SetObject)            (THIS_ LPDMUS_OBJECTDESC pDesc) PURE;
    STDMETHOD(SetSearchDirectory)   (THIS_ REFGUID rguidClass,
                                           WCHAR *pwzPath,
                                           BOOL fClear) PURE;
    STDMETHOD(ScanDirectory)        (THIS_ REFGUID rguidClass,
                                           WCHAR *pwzFileExtension,
                                           WCHAR *pwzScanFileName) PURE;
    STDMETHOD(CacheObject)          (THIS_ IDirectMusicObject * pObject) PURE;
    STDMETHOD(ReleaseObject)        (THIS_ IDirectMusicObject * pObject) PURE;
    STDMETHOD(ClearCache)           (THIS_ REFGUID rguidClass) PURE;
    STDMETHOD(EnableCache)          (THIS_ REFGUID rguidClass,
                                           BOOL fEnable) PURE;
    STDMETHOD(EnumObject)           (THIS_ REFGUID rguidClass,
                                           DWORD dwIndex,
                                           LPDMUS_OBJECTDESC pDesc) PURE;

    /* IDirectMusicLoader8 */
    STDMETHOD_(void, CollectGarbage)                (THIS) PURE;
    STDMETHOD(ReleaseObjectByUnknown)               (THIS_ IUnknown *pObject) PURE;
    STDMETHOD(LoadObjectFromFile)                   (THIS_ REFGUID rguidClassID,
                                                           REFIID iidInterfaceID,
                                                           WCHAR *pwzFilePath,
                                                           void ** ppObject) PURE;
};

