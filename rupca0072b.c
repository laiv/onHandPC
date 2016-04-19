

/************************************************* **********

                                                 1998/12/23
Screen Capture

                        Version 0.07
                      Programed by SeaGate support
                      Ported / translated for onHand by Levi Horowitz

v0.06
Added the ability to capture their
************************************************** **********/
#include <string.h>
#include"rupsys.h"
#include"wbios.h"
#include"psdos.h"
#include"rupmenu.h"

void	syoki_gamen( void );
void	time_disp_and_wait( void );
void	get_exfpath( void );
void	setting_capture( void );
int		*dummy( void );
void	capture_set( void );
int		main( void );

static char	*Mess3[] = {"Date has changed," "Do Not."};
char	Buf[512];
char	Exf_path[64]; /* Exf of where the file PATH */

int After_min = 1; /* minutes after you copy stuff */
int Get_num = 1; /* You can copy many pieces */
int After_sec = 0; /* every second, or copy stuff */

/**********************************************
*
* Initial Screen *
*
**********************************************/
void	syoki_gamen( void )
{
	cls (4);
	gv_kput (3, 5, "Time Now:", 0, 0, 0);
	gv_kput (60, 5, "   secs", 0, 0, 0);
	gv_kput (3, 14, "MENU to set timer", 0, 0, 0);
	gv_kput (3, 23, "  min   sec   snaps", 0, 0, 0);
	gv_kput (3, 39, "By Hazime Hiraku", 0, 0, 0);
	gv_kput (30, 51, "ENTER key", 0, 0, 0);
}

/**********************************************
*
* While the second view, ENTER key * wait
*
**********************************************/
void	time_disp_and_wait( void )
{
char	p[10];
int		i, s;
int		btn, ret;

	syoki_gamen();
	bi_clrbtn();
	pSystemCtl->EventManager->eventReq |= EVENT_SEC; /* Keyin exit set */

	while( 1 ){
		btn = Keyin( 0, -1 );
		ret = EventCall( btn );
		if( ret==-1 ){
			pSystemCtl->EventManager->eventReq &= ~EVENT_SEC;
			cls( 4 );
			bi_clrbtn();
			dos_exit(0);
		}
		if( ret==1 ) {
			pSystemCtl->EventManager->eventReq &= ~EVENT_SEC;
			ret = EventExec();
			if( ret==-1 ){
				cls( 4 );
				bi_clrbtn();
				dos_exit(0);
			}
			syoki_gamen();
			pSystemCtl->EventManager->eventReq |= EVENT_SEC;
		}

		if( (btn&0x000F)==Bkey_B )	break;
		if( (btn&0x000F)==Bkey_D ){
			pSystemCtl->EventManager->eventReq &= ~EVENT_SEC;
			setting_capture();
			syoki_gamen();
			pSystemCtl->EventManager->eventReq |= EVENT_SEC;
		}
		s = pSystemCtl->CPUram->NOW_Sec;
		p[0] = s/10 + 0x30;
		p[1] = s%10 + 0x30;
		p[2] = 0;
		gv_kput( 48, 5, p, 0, 0, 0 );
		s = After_min;
		p[0] = s/10 + 0x30;
		p[1] = s%10 + 0x30;
		gv_kput( 3, 23, p, 0, 0, 0 );
		s = After_sec;
		p[0] = s/10 + 0x30;
		p[1] = s%10 + 0x30;
		gv_kput( 39, 23, p, 0, 0, 0 );
		s = Get_num;
		p[0] = s/10 + 0x30;
		p[1] = s%10 + 0x30;
		gv_kput( 75, 23, p, 0, 0, 0 );
	}
	pSystemCtl->EventManager->eventReq &= ~EVENT_SEC;
}


/**********************************************
*
* EXF PATH station to get a file *
*
**********************************************/
void	get_exfpath( void )
{
int		i, j, k;

	strcpy( Exf_path, getArg() );
	Exf_path[63] = 0;
	k = strlen( Exf_path );
	for( i=0; i<k; i++ ){
		if( Exf_path[i]==0x5c )	j = i;	/* j values, \ mark Position	*/
	}
	Exf_path[j+1] = 0;
}

/**********************************************
*
* * Set the capture conditions
*
**********************************************/
void	setting_capture( void )
{

char		*dataAfter_min[][3]={" 0"," 1"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9","10" };
char		*dataAfter_sec[][3]={" 0"," 1"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9",
							"10","11","12","13","14","15","16","17","18","19",
							"20","21","22","23","24","25","26","27","28","29",
							"30","31","32","33","34","35","36","37","38","39",
							"40","41","42","43","44","45","46","47","48","49",
							"50","51","52","53","54","55","56","57","58","59" };
char		*dataSheets[][3]={" 1"," 2"," 3"," 4"," 5"," 6"," 7"," 8"," 9","10" };
int		id_min, id_sec, id_mai, id;
int		ret;


INPUTITEM Item[4];


initInputTable( "RupCapture 0.07", Item );
/* Menu for processing */
id_min = entryItem ("Min til Cap ", (char *)dataAfter_min, 11);
setItemListVal (id_min, After_min-1);

id_sec = entryItem ("Sec til Cap ", (char *)dataAfter_sec, 60);
setItemListVal (id_sec, After_sec);

id_mai = entryItem ("Num of Caps ", (char *)dataSheets, 10);
setItemListVal (id_mai, Get_num-1);

id = entryItemMenu ("Exit Program", (int *)dummy);
setItemMenuNext (id, 0);

/* Run start menu */
ret = runInputProc ();
if (ret == 1) {
if (getLastItemId () == getItemId ("Exit Program")) {
endInputTable ();
cls (4);
bi_clrbtn ();
dos_exit (0);
}
/* Update process */
After_min = getItemListVal (id_min) + 1;
After_sec = getItemListVal (id_sec);
Get_num = getItemListVal (id_mai) + 1;
if (After_sec <10) Get_num = 1;
setItemListVal (id_min, After_min-1);
setItemListVal (id_sec, After_sec);
setItemListVal (id_mai, Get_num-1);
}
cls (4);
/* Release memory */
endInputTable ();
}
int *dummy (void) {return 0;}


/**********************************************
*
Embed RUPCAP2.EXF * * for capture
*
**********************************************/
void capture_set (void)
{
char pdow [1];
char pmonth [1];
char pday [1];
int pyear [1];
int s, h, m;
int l, i;
int fp;

cls (4);
gv_kput (6, 20, "Setting alarm interruption", 0, 0, 0);

s = pSystemCtl-> CPUram-> NOW_Sec;
h = pSystemCtl-> CPUram-> NOW_Hour;
m = pSystemCtl-> CPUram-> NOW_Min;
dos_getdate (pdow, pmonth, pday, pyear);


fp = dos_fopen ("b:\\ system\\ALARM.INI", 2);
dos_fseek (fp, 0, 0x32L);
dos_fread (fp, Buf, 0x32);
dos_fclose (fp);

fp = dos_fcreate ("b:\\system\\ini\\rupcaptu.ini");
dos_fwrite (fp, Buf, 0x32);
l = strlen (Exf_path);
Buf [0] = (char) (l +1);
dos_fwrite (fp, Buf, 1); /* save the path name length */
dos_fwrite (fp, Exf_path, l +1); /* save the path name */
Buf [0] = (char) (Get_num);
dos_fwrite (fp, Buf, 1); /* You can copy many pieces */
Buf [0] = (char) (After_sec);
dos_fwrite (fp, Buf, 1); /* every second, or copy stuff */
dos_fclose (fp);

	m += After_min;
	if( m>=60 ){
		m -= 60;
		h += 1;
	}
	if( h>=24 ){
		dispMess( Mess3, 2 );
		cls( 4 );
		bi_clrbtn();
		dos_exit( 0 );
	}

	Buf[0] = (char)(pyear[0] & 0xff);
	Buf[1] = (char)(pyear[0] / 0x100);
	Buf[2] = pmonth[0];
	Buf[3] = 0;
	Buf[4] = pday[0];
	Buf[5] = 0;
	Buf[6] = h & 0xff;
	Buf[7] = h >> 8;
	Buf[8] = m & 0xff;
	Buf[9] = m >> 8;

	fp = dos_fopen( "b:\\system\\ALARM.INI", 2 );
	dos_fseek( fp, 0, 0x32L );
	dos_fwrite( fp, Buf, 10 );

	for( i=0; i<8; i++ )	Buf[i] = 0;
	dos_fwrite( fp, Buf, 8 );
	for( i=0; i<32; i++ )	Buf[i] = 0;
	strcpy( Buf, Exf_path );
	strcat( Buf, "rupcap2.EXF" );
	dos_fwrite( fp, Buf, 32 );
	dos_fclose( fp );
	bi_vwrflush();
	setAlarm();
}

/**********************************************
*
* Main Function *
*
**********************************************/
int main (void)
{
screen (1);
gv_place (0, 0);
endWaiting ();
cls (4);
get_exfpath ();
time_disp_and_wait ();
capture_set ();
cls (4);
bi_clrbtn ();
}













