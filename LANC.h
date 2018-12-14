
/* Some basic defaults */
#define cmdPin 5
#define lancPin 4
#define PWMinRec 2
#define PWMinZoom 3

//#define cmdPin    Out[0]    //   o1 port D8  = bit 1 from PORTB
//#define lancPin   Out[1]    //   o2 port D9  = bit 2 from PORTB
//#define PWMinRec  Out[2]    //   o3 port D10 = bit 3 from PORTB
//#define PWMinZoom Out[3]    //   o4 port D4  = bit 5 from PORTD

#define LedPin 13     // Heartbeat LED if any

#define EN  1         // Enable value
#define DI  0         // Disable value
#define IN  0
#define OUT 1

#define MaxPWM 2300
#define MinPWM 700



//#define LedPin 6    // LED Pin

int Out[] = {8,9,10,4,3,2};  /* Output I/O pin array */

//Defines for Throttle settings, Rear Flash when nudging in PosHold


/* Global variables */

int LANCReady = 0;

static uint32_t p_cMillis = 0;
static int counter = 0;      /* General counter */
static int stripCounter =0;
byte ioCounter = 0;          /* Loop counter */
int LedStatus = 0;

/*RC*/
static uint16_t RCOnOff = 0;                  // RC Value
static uint16_t RCZoom = 0;                  //  RC Value


/* time */
static uint8_t minutes;
static uint8_t seconds;
static uint16_t milliseconds;

static uint32_t TimerStart = 0;
static uint32_t TimerEnd = 0;



/*Status*/
int ZoomSpeed = 0;  //0-8
int ZoomPW    = 0; 
int CamStat   = 0; //0=off, 1=on, 2=rec 
int SwitchPos = 0;


//LANC commands byte 0 + byte 1
//Tested with Canon XF300

//Start-stop video recording
boolean REC[] = {LOW,LOW,LOW,HIGH,HIGH,LOW,LOW,LOW,   LOW,LOW,HIGH,HIGH,LOW,LOW,HIGH,HIGH}; //18 33

//Zoom in from slowest to fastest speed
boolean ZOOM_IN[8][16] = {
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,LOW,LOW,LOW,LOW}, //28 00
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,LOW,LOW,HIGH,LOW}, //28 02
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,LOW,HIGH,LOW,LOW}, //28 04
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,LOW,HIGH,HIGH,LOW}, //28 06
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,HIGH,LOW,LOW,LOW}, //28 08
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,HIGH,LOW,HIGH,LOW}, //28 0A
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,HIGH,HIGH,LOW,LOW}, //28 0C
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,LOW}//28 0E
};

//Zoom out from slowest to fastest speed
boolean ZOOM_OUT[8][16] = {
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,LOW,LOW,LOW,LOW}, //28 10
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,LOW,LOW,HIGH,LOW}, //28 12
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,LOW,HIGH,LOW,LOW}, //28 14
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,LOW,HIGH,HIGH,LOW}, //28 16
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,HIGH,LOW,LOW,LOW}, //28 18
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,HIGH,LOW,HIGH,LOW}, //28 1A
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,HIGH,HIGH,LOW,LOW}, //28 1C
{LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH,LOW} //28 1E
};

//Focus control. Camera must be switched to manual focus
boolean FOCUS_NEAR[] = {LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,HIGH,LOW,LOW,LOW,HIGH,HIGH,HIGH}; //28 47
boolean FOCUS_FAR[] = {LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,HIGH,LOW,LOW,LOW,HIGH,LOW,HIGH}; //28 45

boolean FOCUS_AUTO[] = {LOW,LOW,HIGH,LOW,HIGH,LOW,LOW,LOW,   LOW,HIGH,LOW,LOW,LOW,LOW,LOW,HIGH}; //28 41

boolean POWER_OFF[] = {LOW,LOW,LOW,HIGH,HIGH,LOW,LOW,LOW,   LOW,HIGH,LOW,HIGH,HIGH,HIGH,HIGH,LOW}; //18 5E
boolean POWER_ON[] = {LOW,LOW,LOW,HIGH,HIGH,LOW,LOW,LOW,   LOW,HIGH,LOW,HIGH,HIGH,HIGH,LOW,LOW}; //18 5C  Doesn't work because there's no power supply from the LANC port when the camera is off
//boolean POWER_OFF2[] = {LOW,LOW,LOW,HIGH,HIGH,LOW,LOW,LOW,   LOW,LOW,HIGH,LOW,HIGH,LOW,HIGH,LOW}; //18 2A Turns the XF300 off and then on again
//boolean POWER_SAVE[] = {LOW,LOW,LOW,HIGH,HIGH,LOW,LOW,LOW,   LOW,HIGH,HIGH,LOW,HIGH,HIGH,LOW,LOW}; //18 6C Didn't work





