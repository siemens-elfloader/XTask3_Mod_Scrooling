#include <swilib.h>
#include <sys/stat.h>
#include <xtask_ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "swaper.h"
#include "gui.h"
#include "main.h"
#include "conf_loader.h"
#include <cfg_items.h>
#include "strings2.h"

char background_img[64];
char cursor_img[64];
char icon_list_path[64];
char bokmark_list_path[64];
char selection_img[64];
char tasks_img[64];
char bookmarks_img[64];
char shutdown1[128];
char shutdown2[128];
char reboot1[128];
char reboot2[128];
int book_show;
int time_not=1;
extern const int style;
GBSTMR start_tmr;
extern const char ctasks[4];
extern const char cdaemons[4];
extern const char cinfo[4];
extern const char turnoff_path[128];
extern const int cleseri;
extern const char path[128];

extern int SaveConfig(const char *);

void init_turnoff()
{
    sprintf(shutdown1, "%s%s", turnoff_path, "shutdown1.png");
    sprintf(shutdown2, "%s%s", turnoff_path, "shutdown2.png");
    sprintf(reboot1, "%s%s", turnoff_path, "reboot1.png");
    sprintf(reboot2, "%s%s", turnoff_path, "reboot2.png");
}

//=================================bookmarks
char BM1NAME[32];
char BM1FILE[128];
char BM2NAME[32];
char BM2FILE[128];
char BM3NAME[32];
char BM3FILE[128];
char BM4NAME[32];
char BM4FILE[128];
char BM5NAME[32];
char BM5FILE[128];
char BM6NAME[32];
char BM6FILE[128];
char BM7NAME[32];
char BM7FILE[128];
char BM8NAME[32];
char BM8FILE[128];
char BM9NAME[32];
char BM9FILE[128];
char BM0NAME[32];
char BM0FILE[128];

void Start_bookmark(int casa)
{
    switch(casa)
    {
    case 1:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM1FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 2:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM2FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 3:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM3FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 4:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM4FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 5:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM5FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 6:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM6FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 7:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM7FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 8:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM8FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 9:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM9FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    case 10:
    {
        WSHDR *elfname=AllocWS(256);
        wsprintf(elfname, BM0FILE);
        ExecuteFile(elfname,NULL,NULL);
        FreeWS(elfname);
    }
    break;
    }
}
//=================================bookmarks


IMGHDR screen2= {0,0,8,(unsigned char* )""};
int tasks;
char icon_text[32768];
int task_list=1;

void DrwImg3(IMGHDR *img, int x, int y, char *pen, char *brush)
{
    RECT rc;
    DRWOBJ drwobj;
    StoreXYWHtoRECT(&rc,x,y,img->w,img->h);
    SetPropTo_Obj5(&drwobj,&rc,0,img);
    SetColor(&drwobj,pen,brush);
    DrawObject(&drwobj);
}

void DoScreen2()
{
    int ScrH=ScreenH();
    int ScrW=ScreenW();
    char *ms=RamScreenBuffer();
    screen2.w=ScrW;
    screen2.h=ScrH;
    screen2.bitmap=malloc(ScrW*ScrH*2);
    memcpy(screen2.bitmap, ms, ScrW*ScrH*2);
}


const int minus11=-11; // стремная константа =)
unsigned short maincsm_name_body[140];
extern unsigned long  strtoul (const char *nptr,char **endptr,int base);

unsigned int MAINCSM_ID = 0;

typedef struct
{
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

typedef struct
{
    GUI gui;
    WSHDR *ws1;
    WSHDR *ws2;
    int i1;
} MAIN_GUI;

extern NAMELIST * volatile nltop;
int pos=1;
int total_pos=1;
int total_d;
int move_skip;
void redrit();
int sm;
int forward;
int halt;
int shutd;
int logout;


WSHDR *data4;
WSHDR *data5;
int draw_rec;
int this_rec;

void DrawProgress(WSHDR *dat, int casi)
{
    if (casi)
    {
        this_rec++;
        wsprintf(data4, "Closing...");
        DrawScrollString(data4,17,97,100,97+GetFontYSIZE(11),1,11,TEXT_ALIGNLEFT,ctasks,GetPaletteAdrByColorIndex(23));
        wsprintf(data4,"...............[OK]");
        DrawString(data4,115-Get_WS_width(data4,11),109,115+3,109+GetFontYSIZE(11)+1,11,0,ctasks,GetPaletteAdrByColorIndex(23));
        void *canv = BuildCanvas();
        if(Get_WS_width(dat,11)<70)
            DrawCanvas(canv,17,109,17+Get_WS_width(dat,11),109+GetFontYSIZE(11),1);
        else DrawCanvas(canv,17,109,87,109+GetFontYSIZE(11),1);
        DrawScrollString(dat,17,109,87,109+GetFontYSIZE(11),1,11,TEXT_ALIGNLEFT,ctasks,GetPaletteAdrByColorIndex(23));
    }
    else
    {
        if(shutd) wsprintf(data4,"Shut down......");
        else wsprintf(data4,"Restarting......");
        DrawString(data4,17,109,115,109+GetFontYSIZE(11)+1,11,0,ctasks,GetPaletteAdrByColorIndex(23));
    }
    DrawRectangle(18,125,(115 * this_rec)/draw_rec,135,0,GetPaletteAdrByColorIndex(23),GetPaletteAdrByColorIndex(4));
}

int zin;
int init_rec=1;

void task_list3()
{
    DrwImg3(&screen2, 0, 0, NULL, NULL);
    sprintf(background_img,"%s%s",path,"background.png");
    sprintf(cursor_img,"%s%s",path,"cursor.png");
    sprintf(selection_img,"%s%s",path,"selection.png");
    sprintf(tasks_img,"%s%s",path,"tasks.png");
    sprintf(bookmarks_img,"%s%s",path,"bookmarks.png");

    DrawImg(12, 26,(int)background_img);  //12,37
    DrawImg(83,40,(int)tasks_img);
    DrawImg(101,40,(int)bookmarks_img);
    total_d=GetNumberOfDialogs()-2;
    if(init_rec)
    {
        draw_rec=total_d;
        init_rec=0;
    }
    NAMELIST *nl=nltop;
    if(total_d>0)
    {
        NAMELIST *p=nl;
        nl=p->next;
        zin=((CSM_RAM *)(nl->p))->id;
        if (zin==CSM_root()->idle_id)
        {
            NAMELIST *p=nl;
            nl=p->next;
            zin=((CSM_RAM *)(nl->p))->id;
        }
        DrawProgress(nl->name, 1);
        CloseCSM(zin);

        GBS_StartTimerProc(&start_tmr, 216, (void (*)(GBSTMR *))task_list3);
    }
    else
    {
        DrawProgress(NULL, 0);
        if (shutd==1) SwitchPhoneOff();
        else RebootPhone();
    }
}

void SearchIcon(WSHDR *ramed, int w, int show)
{
    char buffer[256];
    //ws_2str(ramed, buffer, 64);
    convWS_to_ANSI(ramed, buffer);
    if(strstr(buffer, "NATICQ")>0) strcpy(buffer, "NATICQ");
    if(strstr(buffer, "SieJC")>0) strcpy(buffer, "SieJC");
    if(strstr(buffer, "MC:")>0) strcpy(buffer, "MC");
    if(strstr(buffer, "BM:")>0) strcpy(buffer, "BM:");
    char p2[]="||";
    char *s,*s1,*s2;
    int len;
    if((s1=strstr(icon_text,buffer))>0)
    {
        s1+=strlen(buffer);
        if((s2=strstr(s1,p2))>0)
        {
            len=s2-s1-1;
            s=malloc(len+1);
            strncpy(s,s1+1,len);
            s[len]='\0';
            if(show) DrawImg(17,w+1,(int)s);
            DrawImg(102,w+1,(int)s);
            mfree(s);
        }
    }
}

int refresh;

void OnRedraw(MAIN_GUI *data)
{
    sprintf(background_img,"%s%s",path,"background.png");
    sprintf(cursor_img,"%s%s",path,"cursor.png");
    sprintf(selection_img,"%s%s",path,"selection.png");
    sprintf(tasks_img,"%s%s",path,"tasks.png");
    sprintf(bookmarks_img,"%s%s",path,"bookmarks.png");
    refresh=216;
    if(task_list==1)
    {
        total_d=GetNumberOfDialogs()-1;
#ifdef NEWSGOLD
        DrwImg3(&screen2, 0, 0, NULL, NULL);
#endif
        DrawImg(12, 26,(int)background_img);  //12,37
        DrawImg(16,48+(pos*12),(int)cursor_img);
        DrawImg(82,39,(int)selection_img);
        DrawImg(83,40,(int)tasks_img);
        DrawImg(101,40,(int)bookmarks_img);
        int show=1;
        NAMELIST *nl=nltop;
        int i=60;
        WSHDR *ws=AllocWS(64);
        int skip=move_skip+1;
        while((nl)&&(show<=5))
        {
            NAMELIST *p=nl;
            if(!skip)
            {
                /*      if(nl->is_daemon)
                      {
                        DrawString(nl->name,17,i+1,115,i+GetFontYSIZE(11)+2,11,0,cdaemons,GetPaletteAdrByColorIndex(23));
                        SearchIcon(nl->name, i);
                      }
                      else
                      {*/
                if(Get_WS_width(nl->name,11)<85)
                {
                    if(nl->is_daemon) DrawString(nl->name,17,i+1,115,i+GetFontYSIZE(11),11,0,cdaemons,GetPaletteAdrByColorIndex(23));
                    else
                        DrawString(nl->name,17,i+1,115,i+GetFontYSIZE(11),11,0,ctasks,GetPaletteAdrByColorIndex(23));
                }
                else
                {
                    if(pos!=show)
                    {
                        if(nl->is_daemon) DrawScrollString(nl->name,17,i+1,100,i+GetFontYSIZE(11),1,11,TEXT_ALIGNLEFT,cdaemons,GetPaletteAdrByColorIndex(23));
                        else
                            DrawScrollString(nl->name,17,i+1,100,i+GetFontYSIZE(11),1,11,TEXT_ALIGNLEFT,ctasks,GetPaletteAdrByColorIndex(23));
                    }
                    else
                    {
                        if(nl->is_daemon) DrawScrollString(nl->name,17,i+1,100,i+GetFontYSIZE(11),sm+1,11,TEXT_ALIGNLEFT,cdaemons,GetPaletteAdrByColorIndex(23));
                        else
                            DrawScrollString(nl->name,17,i+1,100,i+GetFontYSIZE(11),sm+1,11,TEXT_ALIGNLEFT,ctasks,GetPaletteAdrByColorIndex(23));
                        refresh=30;
                        if(forward==1) sm=sm+3;
                        if(forward==0) sm=sm-3;
                        if(forward==2)
                        {
                            if(halt) halt--;
                            else forward=3;
                        }
                        if ((sm>Get_WS_width(nl->name,11)-85)&&(forward==1))
                        {
                            forward=2;
                            halt=5;
                        }
                        if (forward==3)
                        {
                            if (sm==0)
                            {
                                forward=4;
                                halt=5;
                            }
                            else sm=sm-3;
                        }
                        if(forward==4)
                        {
                            if(halt) halt--;
                            else forward=1;
                        }
                        // if ((sm==0)&&(forward==3) forward=1;
                    }
                }
                SearchIcon(nl->name, i, 0);
                //    }
                show++;
                i=i+12;
            }
            else skip--;
            nl=p->next;
        }
        if(time_not)
        {
            if(!style) wsprintf(ws, "Tasks:%d", tasks);
            else wsprintf(ws, "T:%d", tasks);
            DrawString(ws,16,124,16+Get_WS_width(ws,11)+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
            int ram = GetFreeRamAvail();
            ram=ram/1024;
            wsprintf(ws,"Ram:%dKb", ram);
            DrawString(ws,115-Get_WS_width(ws,11),124,115+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
        }
        else
        {
            int battery = *RamCap();
            wsprintf(ws,"%d%", battery);
            DrawString(ws,16,124,16+Get_WS_width(ws,11)+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
            TTime time;
            TDate date;
            GetDateTime(&date, &time);
            wsprintf(ws, "%02d:%02d:%02d", time.hour, time.min, time.sec);
            DrawString(ws,115-Get_WS_width(ws,11),124,115+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
//      GBS_StartTimerProc(&start_tmr, 30, redrit);
        }
        FreeWS(ws);
    }
    else if(task_list==0)
    {
        //===============================do the links
        DrwImg3(&screen2, 0, 0, NULL, NULL);
        DrawImg(12, 26,(int)background_img);  //12,37
        DrawImg(16,48+(pos*12),(int)cursor_img);
        DrawImg(100,39,(int)selection_img);
        DrawImg(83,40,(int)tasks_img);
        DrawImg(101,40,(int)bookmarks_img);
        int show=0;
        int i=60;
        int len;
        int skip=move_skip;
        WSHDR *ws=AllocWS(32);
        if((!skip)&&(book_show>=1)&&(show<5))
        {
            wsprintf(ws, BM1NAME);
            //  DrawString(ws,17,i+1,115,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=2)&&(show<5))
        {
            wsprintf(ws, BM2NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=3)&&(show<5))
        {
            wsprintf(ws, BM3NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=4)&&(show<5))
        {
            wsprintf(ws, BM4NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i,1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=5)&&(show<5))
        {
            wsprintf(ws, BM5NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=6)&&(show<5))
        {
            wsprintf(ws, BM6NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=7)&&(show<5))
        {
            wsprintf(ws, BM7NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=8)&&(show<5))
        {
            wsprintf(ws, BM8NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=9)&&(show<5))
        {
            wsprintf(ws, BM9NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if((!skip)&&(book_show>=10)&&(show<5))
        {
            wsprintf(ws, BM0NAME);
            len=Get_WS_width(ws,11)/2;
            DrawString(ws,66-len,i+1,66+len,i+GetFontYSIZE(11)+2,11,0,ctasks,GetPaletteAdrByColorIndex(23));
            SearchIcon(ws, i, 1);
            show++;
            i=i+12;
        }
        else
            skip--;
        if(time_not)
        {
            if(!style) wsprintf(ws, "Tasks:%d", tasks);
            else wsprintf(ws, "T:%d", tasks);
            DrawString(ws,16,124,16+Get_WS_width(ws,11)+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
            int ram = GetFreeRamAvail();
            ram=ram/1024;
            wsprintf(ws,"Ram:%dKb", ram);
            DrawString(ws,115-Get_WS_width(ws,11),124,115+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
        }
        else
        {
            int battery = *RamCap();
            wsprintf(ws,"%d%", battery);
            DrawString(ws,16,124,16+Get_WS_width(ws,11)+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
            TTime time;
            TDate date;
            GetDateTime(&date, &time);
            wsprintf(ws, "%02d:%02d:%02d", time.hour, time.min, time.sec);
            DrawString(ws,115-Get_WS_width(ws,11),124,115+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));

        }
        FreeWS(ws);
    }//===============================end do links
    else if(task_list==2)
    {
        DrwImg3(&screen2, 0, 0, NULL, NULL);
        DrawImg(12, 26,(int)background_img);  //12,37
//      DrawImg(16,48+(pos*12),(int)cursor_img);
//      DrawImg(100,39,(int)selection_img);
        DrawImg(83,40,(int)tasks_img);
        DrawImg(101,40,(int)bookmarks_img);
        if(shutd)
        {
            DrawImg(16,60,(int)shutdown1);
            DrawImg(16,91,(int)reboot2);
        }
        else
        {
            DrawImg(16,60,(int)shutdown2);
            DrawImg(16,91,(int)reboot1);
        }
        WSHDR *ws=AllocWS(32);
        if(time_not)
        {
            if(!style) wsprintf(ws, "Tasks:%d", tasks);
            else wsprintf(ws, "T:%d", tasks);
            DrawString(ws,16,124,16+Get_WS_width(ws,11)+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
            int ram = GetFreeRamAvail();
            ram=ram/1024;
            wsprintf(ws,"Ram:%dKb", ram);
            DrawString(ws,115-Get_WS_width(ws,11),124,115+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
        }
        else
        {
            int battery = *RamCap();
            wsprintf(ws,"%d%", battery);
            DrawString(ws,16,124,16+Get_WS_width(ws,11)+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
            TTime time;
            TDate date;
            GetDateTime(&date, &time);
            wsprintf(ws, "%02d:%02d:%02d", time.hour, time.min, time.sec);
            DrawString(ws,115-Get_WS_width(ws,11),124,115+3,124+GetFontYSIZE(11)+1,11,0,cinfo,GetPaletteAdrByColorIndex(23));
        }
        FreeWS(ws);
    }
    if(task_list!=3) GBS_StartTimerProc(&start_tmr, refresh, (void (*)(GBSTMR *))redrit);
}

void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int)) //Create
{
    data->gui.state=1;
}

void onClose(MAIN_GUI *data, void (*mfree_adr)(void *)) //Close
{
#ifdef NEWSGOLD
    mfree(screen2.bitmap);
#endif
    ClearNL();
    MAINCSM_ID = 0;
    data->gui.state=0;
}

void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))//Focus
{
    data->gui.state=2;
//  DisableIDLETMR();
}

void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *)) //Unfocus
{
    if (data->gui.state!=2) return;
    data->gui.state=1;
}

int closing;
int show_daemons;


void redrit()
{
    DirectRedrawGUI();
}

int OnKey(MAIN_GUI *data, GUI_MSG *msg) //OnKey
{
    if (msg->gbsmsg->msg==KEY_DOWN)
    {
        if(task_list==1)
        {
            switch(msg->gbsmsg->submess)
            {
            case RIGHT_BUTTON:
                pos=1;
                total_pos=1;
                move_skip=0;
                task_list=0;
                DirectRedrawGUI();
                break;
            case DOWN_BUTTON:
                total_pos++;
                if(total_pos<=total_d)
                {
                    if((total_pos<=3)||(total_pos>total_d-2)) pos++;
                    if((total_pos>3)&&(total_pos<=total_d-2)) move_skip++;
                }
                else
                    total_pos--;
                sm=0;
                forward=1;
                DirectRedrawGUI();
                break;
            case UP_BUTTON:
                total_pos--;
                if(total_pos)
                {
                    if((total_pos<3)||(total_pos>=total_d-2)) pos--;
                    if((total_pos>=3)&&(total_pos<total_d-2)) move_skip--;
                }
                else
                    total_pos++;
                sm=0;
                forward=1;
                DirectRedrawGUI();
                break;
            case '*':
                show_daemons=!show_daemons;
                pos=1;
                total_pos=1;
                move_skip=0;
                sm=0;
                forward=1;
                DirectRedrawGUI();
                break;
            case 0x30:
                time_not=!time_not;
                DirectRedrawGUI();
                break;
            case 0x31:
                if(book_show>=1)
                {
                    Start_bookmark(1);
                    return(1);
                }
                break;
            case 0x32:
                if(book_show>=2)
                {
                    Start_bookmark(2);
                    return(1);
                }
                break;
            case 0x33:
                if(book_show>=3)
                {
                    Start_bookmark(3);
                    return(1);
                }
                break;
            case 0x34:
                if(book_show>=4)
                {
                    Start_bookmark(4);
                    return(1);
                }
                break;
            case 0x35:
                if(book_show>=5)
                {
                    Start_bookmark(5);
                    return(1);
                }
                break;
            case 0x36:
                if(book_show>=6)
                {
                    Start_bookmark(6);
                    return(1);
                }
                break;
            case 0x37:
                if(book_show>=7)
                {
                    Start_bookmark(7);
                    return(1);
                }
                break;
            case 0x38:
                if(book_show>=8)
                {
                    Start_bookmark(8);
                    return(1);
                }
                break;
            case 0x39:
                if(book_show>=9)
                {
                    Start_bookmark(9);
                    return(1);
                }
                break;
            case LEFT_BUTTON:
                if(!cleseri) break;
            case '#':
                closing=1;
            case ENTER_BUTTON:
            {
minine:
                {
                    NAMELIST *nl=nltop;
                    int skip_run=total_pos;
                    while(nl)
                    {
                        NAMELIST *p=nl;
                        if(skip_run)
                        {
                            skip_run--;
                            nl=p->next;
                        }
                        else
                            goto don;
                    }
don:
                    if ((!nl->is_daemon)||((closing)&&(nl->is_daemon)))
                    {
                        if(closing)
                        {
                            int i;
                            closing=0;
                            i=((CSM_RAM *)(nl->p))->id;
                            if (i!=CSM_root()->idle_id)
                            {
                                CloseCSM(i);
                                int rem_pso=total_pos-1;
                                pos=1;
                                total_pos=1;
                                move_skip=0;
                                while(rem_pso!=0)
                                {
                                    total_pos++;
                                    if(total_pos<=total_d-1)
                                    {
                                        if((total_pos<=3)||(total_pos>total_d-3)) pos++;
                                        if((total_pos>3)&&(total_pos<=total_d-3)) move_skip++;
                                    }
                                    else
                                    {
                                        total_pos--;
                                    }
                                    rem_pso--;
                                }
                                sm=0;
                                forward=1;
                                GBS_StartTimerProc(&start_tmr, 30, redrit);
                            }
                        }
                        else
                        {
                            CSMtoTop(((CSM_RAM *)(nl->p))->id,-1);
                            return(1);
                        }
                    }
                    else
                        closing=0;
                }
            }
            break;
            case LEFT_SOFT:
                CSMtoTop(CSM_root()->idle_id,-1);
                return(1);
            case RIGHT_SOFT:
                total_pos=1;
                goto minine;
            case GREEN_BUTTON:
                task_list=2;
                shutd=1;
                DirectRedrawGUI();
                break;
//     return(1);
            }
        }
        else//=======================================do the links
            if(task_list==0)
            {
                switch(msg->gbsmsg->submess)
                {
                case LEFT_BUTTON:
                    pos=1;
                    total_pos=1;
                    move_skip=0;
                    task_list=1;
                    sm=0;
                    forward=1;
                    DirectRedrawGUI();
                    break;
                case DOWN_BUTTON:
                    total_pos++;
                    if(total_pos<=book_show)
                    {
                        if((total_pos<=3)||(total_pos>book_show-2)) pos++;
                        if((total_pos>3)&&(total_pos<=book_show-2)) move_skip++;
                    }
                    else
                        total_pos--;
                    DirectRedrawGUI();
                    break;
                case UP_BUTTON:
                    total_pos--;
                    if(total_pos)
                    {
                        if((total_pos<3)||(total_pos>=book_show-2)) pos--;
                        if((total_pos>=3)&&(total_pos<book_show-2)) move_skip--;
                    }
                    else
                        total_pos++;
                    DirectRedrawGUI();
                    break;
                case 0x30:
                    time_not=!time_not;
                    DirectRedrawGUI();
                    break;
                case LEFT_SOFT:
                    CSMtoTop(CSM_root()->idle_id,-1);
                    return(1);
                case RIGHT_SOFT:
                    total_pos=1;
                    goto minine;
                case ENTER_BUTTON:
                    Start_bookmark(total_pos);
                    return(1);
                case GREEN_BUTTON:
                    task_list=2;
                    shutd=1;
                    DirectRedrawGUI();
                    break;
                }
            }//==========================================end links
            else if(task_list==2)
            {
                switch(msg->gbsmsg->submess)
                {
                case GREEN_BUTTON:
                    pos=1;
                    total_pos=1;
                    move_skip=0;
                    task_list=1;
                    sm=0;
                    forward=1;
                    task_list=1;
                    DirectRedrawGUI();
                    break;
                case 0x30:
                    time_not=!time_not;
                    DirectRedrawGUI();
                    break;
                case DOWN_BUTTON:
                case UP_BUTTON:
                    shutd=!shutd;
                    break;
                case ENTER_BUTTON:
                    task_list=3;
                    data4=AllocWS(128);
                    data5=AllocWS(128);
                    show_daemons=1;
                    task_list3();
                    break;
                }
            }
    }
    return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));
void ElfKiller(void)
{
    kill_elf();
}

int method8(void)
{
    return(0);
}

int method9(void)
{
    return(0);
}

const void * const gui_methods[11]=
{
    (void *)OnRedraw,	//Redraw
    (void *)onCreate,	//Create
    (void *)onClose,	//Close
    (void *)onFocus,	//Focus
    (void *)onUnfocus,	//Unfocus
    (void *)OnKey,	//OnKey
    0,
    (void *)kill_data,	//Destroy
    (void *)method8,
    (void *)method9,
    0
};

const RECT Canvas= {0,0,132,176};

inline void patch_rect(const RECT*rcc,int x,int y, int x2, int y2)
{
    RECT *rc=(RECT *)rcc;
    rc->x=x;
    rc->y=y;
    rc->x2=x2;
    rc->y2=y2;
}

void maincsm_oncreate(CSM_RAM *data)
{
#ifdef NEWSGOLD
    DoScreen2();
#endif
    MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
    MAIN_CSM*csm=(MAIN_CSM*)data;
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(void *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    patch_rect((RECT*)&Canvas,0,YDISP,ScreenW()-1,ScreenH()-1);
    csm->csm.state=0;
    csm->csm.unk1=0;
    csm->gui_id=CreateGUI(main_gui);
}


void maincsm_onclose(CSM_RAM *csm)
{
    //SUBPROC((void *)ElfKiller);
}

int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        csm->csm.state=-3;
    }
    if (msg->msg==MSG_INCOMMING_CALL)
    {
        csm->csm.state=-3;
    }
    return(1);
}

const struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM =
{
    {
        maincsm_onmessage,
        maincsm_oncreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        maincsm_onclose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139
    }
};

void UpdateCSMname(void)
{
    WSHDR *ws=AllocWS(256);
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"xtask 3 mod");
    FreeWS(ws);
}
extern const char UNDER_IDLE_CONSTR[];
extern const char *successed_config_filename;

CSM_RAM *GetUnderIdleCSM(void)
{
    CSM_RAM *u;
    CSM_DESC *UnderIdleDesc;
    if (strlen((char *)UNDER_IDLE_CONSTR)==8)
    {
        UnderIdleDesc=(CSM_DESC *)strtoul((char *)UNDER_IDLE_CONSTR,0,0x10);
    }
    else
    {
        UnderIdleDesc=((CSM_RAM *)(FindCSMbyID(CSM_root()->idle_id))->prev)->constr;
        sprintf((char *)UNDER_IDLE_CONSTR,"%08X",UnderIdleDesc);

        SaveConfig(successed_config_filename);
    }
    LockSched();
    u=CSM_root()->csm_q->csm.first;
    while(u && u->constr!=UnderIdleDesc) u=u->next;
    UnlockSched();
    return u;
}

//------------add on---------------------------//
#ifdef NEWSGOLD
#define USE_ONE_KEY
#endif

#define TMR_SECOND(A) (1300L*A/6)

volatile int SHOW_LOCK;

CSM_DESC icsmd;
int (*old_icsm_onMessage)(CSM_RAM*,GBS_MSG*);
void (*old_icsm_onClose)(CSM_RAM*);


CSM_RAM *under_idle;

extern const int ACTIVE_KEY;
extern const int ACTIVE_KEY_STYLE;
extern const int RED_BUT_MODE;
extern const int ENA_LONG_PRESS;
extern const int ENA_LOCK;

extern const char *successed_config_filename;


int mode;

int mode_red;

int mode_enter;


void ShowMenu()
{
    if(!IsGuiOnTop(MAINCSM_ID))
    {
        pos=1;
        total_pos=1;
        move_skip=0;
        task_list=1;
        time_not=1;
        sm=0;
        forward=1;
        logout=0;
        total_d=GetNumberOfDialogs()-1;
        char dummy[sizeof(MAIN_CSM)];
        MAINCSM_ID = CreateCSM(&MAINCSM.maincsm,dummy,2);
    }
}


int my_keyhook(int submsg, int msg)
{
#ifdef NEWSGOLD
    void *icsm=FindCSMbyID(CSM_root()->idle_id);
    if ((submsg==RED_BUTTON)&&(RED_BUT_MODE))
    {
        if ((CSM_root()->csm_q->csm.last==icsm)||(IsCalling())) //(IsGuiOnTop(((int *)icsm)[DISPLACE_OF_IDLEGUI_ID/4]))
        {
            if (msg==KEY_UP)
            {
                if (mode_red!=2)
                {
                    mode_red=0;
                    return KEYHOOK_BREAK;
                }
            }
            mode_red=2; //????? ?? ??????????
        }
        else
        {
            if (msg==KEY_DOWN)
            {
                if (mode_red==1)
                {
                    mode_red=0;
                    return KEYHOOK_NEXT; //Long press, continue with REDB PRESS
                }
            }
            if (msg==KEY_UP)
            {
                if (mode_red)
                {
                    mode_red=0; //Release after longpress
                    return KEYHOOK_NEXT;
                }
                else
                    //Release after short press
                {
                    if (RED_BUT_MODE==1)
                    {
                        if((task_list!=3)&&(task_list!=2)) GBS_SendMessage(MMI_CEPID,KEY_DOWN,RIGHT_SOFT);
                    }
                    else
                    {
                        if (!MAINCSM_ID)
                        {
                            CSMtoTop(CSM_root()->idle_id,-1);
                        }
                    }
                }
            }
            if (msg==LONG_PRESS)
            {
                mode_red=1;
                if((task_list!=3)&&(task_list!=2)) GBS_SendMessage(MMI_CEPID,KEY_DOWN,RED_BUTTON);
            }
            return KEYHOOK_BREAK;
        }
    }
#endif
//#ifndef NEWSGOLD
    if (ACTIVE_KEY_STYLE==3)
    {
        if (submsg!=ENTER_BUTTON) return KEYHOOK_NEXT;
        /*    if (my_csm_id)
            {
              if (((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=my_csm_id)
              {
                CloseCSM(my_csm_id);
              }
              return KEYHOOK_NEXT;
            }*/
        switch(msg)
        {
        case KEY_DOWN:
            if (mode_enter==2)
            {
                GBS_SendMessage(MMI_CEPID,KEY_UP,ENTER_BUTTON);
                return KEYHOOK_NEXT;
            }
            mode_enter=0;
            return KEYHOOK_BREAK;
        case KEY_UP:
            if (mode_enter==0)
            {
                mode_enter=2;
                if((task_list!=3)&&(task_list!=2)) GBS_SendMessage(MMI_CEPID,KEY_DOWN,ENTER_BUTTON);
                return KEYHOOK_BREAK;
            }
            if (mode_enter==2)
            {
                mode_enter=0;
                return KEYHOOK_NEXT;
            }
            mode_enter=0;
            return KEYHOOK_BREAK;
        case LONG_PRESS:
            mode_enter=1;
            if (IsUnlocked()||ENA_LOCK)
            {
                ShowMenu();
            }
            mode=0;
            return KEYHOOK_BREAK;
        }
    }
    // * + # implementation
    if ((ACTIVE_KEY_STYLE==2) && !(MAINCSM_ID))
    {
        if (msg==KEY_UP)
        {
            mode=0;
            return KEYHOOK_NEXT;
        }
        if (msg==KEY_DOWN)
        {
            switch (submsg)
            {
            case '*':
                mode=1;
                return (0);
            case '#':
                if (mode==1)
                {
                    if (IsUnlocked()||ENA_LOCK)
                        ShowMenu();
                    else mode=0;
                }
                else return KEYHOOK_NEXT;
            }
        }
    }
//#endif
    if (ACTIVE_KEY_STYLE<2)
    {
        if (submsg!=ACTIVE_KEY) return KEYHOOK_NEXT;
        if (MAINCSM_ID)
        {
            /*      if (((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id!=my_csm_id)
                  {
            	CloseCSM(my_csm_id);
                  }*/
            if (msg==KEY_UP)
            {
                total_pos=2;
                if(total_pos>total_d) total_pos--;
                if((task_list!=3)&&(task_list!=2)) GBS_SendMessage(MMI_CEPID,KEY_DOWN,ENTER_BUTTON);
            }
            return KEYHOOK_BREAK;
        }
        switch(msg)
        {
        case KEY_DOWN:
            mode=0;
            if (ACTIVE_KEY_STYLE==0)
                return KEYHOOK_BREAK;
            else
                return KEYHOOK_NEXT;
        case KEY_UP:
            if (mode==1)
            {
                //Release after longpress
                mode=0;
                if ((ACTIVE_KEY_STYLE==1) || (ENA_LONG_PRESS==3))
                {
                    //Launch on LongPress or Extra on LP - Launch
                    if (IsUnlocked()||ENA_LOCK)
                    {
                        ShowMenu();
                    }
                    return KEYHOOK_BREAK;
                }
                if (ENA_LONG_PRESS==1) return KEYHOOK_BREAK;
                if (ENA_LONG_PRESS==2)
                {
                    if ((task_list!=2)&&(task_list!=3)) CSMtoTop(CSM_root()->idle_id,-1);
                    return KEYHOOK_BREAK;
                }
                if (ENA_LONG_PRESS==4)
                {
                    if ((task_list!=2)&&(task_list!=3))
                    {
                        CSMtoTop(CSM_root()->idle_id,-1);
                        KbdLock();
                    }
                    return KEYHOOK_BREAK;
                }
                break;
            }
            if (ACTIVE_KEY_STYLE==0)
            {
                if (IsUnlocked()||ENA_LOCK)
                {
                    ShowMenu();
                }
                return KEYHOOK_BREAK;
            }
            break;
        case LONG_PRESS:
            mode=1;
#ifndef NEWSGOLD
            if (ACTIVE_KEY_STYLE==1)
            {
                if (ENA_LONG_PRESS)
                    return KEYHOOK_NEXT;
                else
                    return KEYHOOK_BREAK;
            }
#else
            return KEYHOOK_BREAK;
#endif
        }
    }
    return KEYHOOK_NEXT;
}

volatile int callhide_mode=0;

static inline int toupper(int c)
{
    if ((c>='a')&&(c<='z')) c+='A'-'a';
    return(c);
}

int strcmp_nocase(const char *s1,const char *s2)
{
    int i;
    int c;
    while(!(i=(c=toupper(*s1++))-toupper(*s2++))) if (!c) break;
    return(i);
}

const char my_ipc_name[]=IPC_XTASK_NAME;

int MyIDLECSM_onMessage(CSM_RAM* data,GBS_MSG* msg)
{
    int csm_result;
    int icgui_id;
    //  int ocgui_id;
    int idlegui_id;

#ifndef NEWSGOLD
#define EXT_BUTTON 0x63
    if ((ACTIVE_KEY_STYLE!=2)&&(ACTIVE_KEY_STYLE!=3)) //?? "* + #" ? ?? "Enter Button"
    {
        //???? ????? ???? ??????? ??????? ??? ???? ???????
        if (ACTIVE_KEY==EXT_BUTTON) //?????? ?????? ????????
        {
            if (msg->msg==439) //?????? ???????
            {
                switch (msg->submess)
                {
                case 1:
                    if((task_list!=3)&&(task_list!=2)) GBS_SendMessage(MMI_CEPID,LONG_PRESS,EXT_BUTTON);
                    break;
                case 2:
                    if((task_list!=3)&&(task_list!=2)) GBS_SendMessage(MMI_CEPID,KEY_UP,EXT_BUTTON);
                    break; // ???????? default!!!
                }
            }
            else //??????? ?? ?????????
                goto L1;
        }
        else //?????? ?????? ?? ???????? ?????? ??????? ?????? ????????
            goto L1;
    }
    else
L1:
        csm_result=old_icsm_onMessage(data,msg);
#else
    csm_result = old_icsm_onMessage(data, msg); //???????? ?????? ?????????? ???????
#endif

    icgui_id=((int *)data)[DISPLACE_OF_INCOMMINGGUI/4];
    idlegui_id=((int *)data)[DISPLACE_OF_IDLEGUI_ID/4];
    //  ocgui_id=((int *)data)[DISPLACE_OF_OUTGOINGGUI/4];
    if (!icgui_id) callhide_mode=0;
    //  if (!ocgui_id) callhide_mode=0;

    if(msg->msg == MSG_RECONFIGURE_REQ)
    {
        if (strcmp_nocase(successed_config_filename,(char *)msg->data0)==0)
        {
            ShowMSG(1,(int)"XTask config updated!");
            InitConfig();
            init_turnoff();
        }
    }
    //IPC
    if (msg->msg==MSG_IPC)
    {
        IPC_REQ *ipc;
        if ((ipc=(IPC_REQ*)msg->data0))
        {
            if (strcmp(ipc->name_to,my_ipc_name)==0)
            {
                switch (msg->submess)
                {
                case IPC_XTASK_SHOW_CSM:
                    if (MAINCSM_ID) break;
                    if ((!IsCalling())&&(!SHOW_LOCK))
                    {
                        if ((CSM_root()->csm_q->csm.last!=data)||IsGuiOnTop(idlegui_id))
                        {
                            CSMtoTop((int)(ipc->data),-1);
                        }
                    }
                    break;
                case IPC_XTASK_IDLE:
                    if (MAINCSM_ID) break;
                    if ((!IsCalling())/*&&(!SHOW_LOCK)*/)
                        CSMtoTop(CSM_root()->idle_id,-1);
                    break;
                case IPC_XTASK_LOCK_SHOW:
                    SHOW_LOCK++;
                    break;
                case IPC_XTASK_UNLOCK_SHOW:
                    if (SHOW_LOCK) SHOW_LOCK--;
                    break;
                }
            }
        }
    }
    if (msg->msg==MSG_INCOMMING_CALL)
    {
        callhide_mode=1;
    }
    //  #ifdef NEWSGOLD
    //  if ((msg->msg==MSG_STATE_OF_CALL)&&(msg->submess==0)&&((int)msg->data0==3)) callhide_mode=1;
    //  #else
    //if ((msg->msg==MSG_STATE_OF_CALL)&&(msg->submess==0)&&((int)msg->data0==0)) callhide_mode=1;
    //    ?????!!!! ??????!!!
    //  #endif
    if (callhide_mode)
    {
        if ((IsGuiOnTop(icgui_id))/*||(IsGuiOnTop(ocgui_id))*/)
        {
            CSMtoTop(CSM_root()->idle_id,((CSM_RAM *)(CSM_root()->csm_q->csm.last))->id);
            callhide_mode=0;
        }
    }
    return csm_result;
}

void MyIDLECSM_onClose(CSM_RAM *data)
{



	extern void *__ex;
//  extern void seqkill(void *data, void(*next_in_seq)(CSM_RAM *), void *data_to_kill, void *seqkiller);
//  extern void *ELF_BEGIN;
  GBS_DelTimer(&start_tmr);
  RemoveKeybMsgHook((void *)my_keyhook);
//  seqkill(data,old_icsm_onClose,&ELF_BEGIN,SEQKILLER_ADR());
  elfclose(__ex);
#warning "Testo'.."
}



void load_icon_list()
{
    FILE *fi;
    struct stat buff;
    int fsizei;
    sprintf(icon_list_path,"%s%s",path,"icon_list.txt");
    if ((fi=fopen(icon_list_path,"rb"))!=NULL)
    {
        fstat (fileno (fi), &buff);

        if(buff.st_size>0)
        {
            fsizei=buff.st_size;
            char *pi=malloc(fsizei);
            fread(pi,fsizei,1,fi);
            fclose(fi);
            strcpy(icon_text, pi);
            icon_text[fsizei]='\0';
            free(pi);
        }
    }
}
int serti;

void get_bookmark(char *name, char *path, int w, char *search)
{
    int q=0;
    while(search[w]!='|')
    {
        name[q]=search[w];
        q++;
        w++;
    }
    name[q]='\0';
    w++;
    q=0;
    while(search[w]!='|')
    {
        path[q]=search[w];
        q++;
        w++;
    }
    path[q]='\0';
    serti=w+3;
    book_show++;
}

void load_bookmark_list()
{
    FILE *fi;
    struct stat buff;

    int fsize;
    sprintf(bokmark_list_path,"%s%s",path,"bookmark_list.txt");
    if ((fi=fopen(bokmark_list_path,"rb"))!=NULL)
    {
        fstat (fileno (fi), &buff);

        if(buff.st_size>0)
        {

            fsize=buff.st_size;
            char *p=malloc(fsize);
            fread(p,fsize,1,fi);
            fclose(fi);
            serti=0;
            if(serti<fsize-2) get_bookmark(BM1NAME, BM1FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM2NAME, BM2FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM3NAME, BM3FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM4NAME, BM4FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM5NAME, BM5FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM6NAME, BM6FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM7NAME, BM7FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM8NAME, BM8FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM9NAME, BM9FILE, serti, p);
            if(serti<fsize-2) get_bookmark(BM0NAME, BM0FILE, serti, p);
            mfree(p);
        }
    }
}

void DoSplices()
{
    extern const int SHOW_DAEMONS;
    extern int show_daemons;
    show_daemons=SHOW_DAEMONS;
    LockSched();
    if (!AddKeybMsgHook_end((void *)my_keyhook))
    {
        ShowMSG(1,(int)"Cant Start Elf!");
        SUBPROC((void *)ElfKiller);
    }
    else
    {
        {
            CSM_RAM *icsm=FindCSMbyID(CSM_root()->idle_id);
            memcpy(&icsmd,icsm->constr,sizeof(icsmd));
            old_icsm_onClose=icsmd.onClose;
            old_icsm_onMessage=icsmd.onMessage;
            icsmd.onClose=MyIDLECSM_onClose;
            icsmd.onMessage=MyIDLECSM_onMessage;
            icsm->constr=&icsmd;
        }
        load_icon_list();
        load_bookmark_list();
        under_idle=GetUnderIdleCSM(); //???? idle_dialog
        FILE *f;
        struct stat buff;
        int sz=0;
        extern const char csmlist_fname[128];
        extern char csm_text[32768];

        if ((f=fopen(csmlist_fname,"rb")) != NULL)
        {
            fstat (fileno (f), &buff);
            if(buff.st_size>0)
            {
                sz = buff.st_size;
                fread(csm_text,sz,1,f);
                if (sz>=0) csm_text[sz]=0;
            }
            fclose(f);
        }
    }

    UnlockSched();
}


int main(const char *exename, const char *fname)
{
    mode=0;
    if (InitConfig()!=2) GBS_StartTimerProc(&start_tmr,TMR_SECOND(60),(void (*)(GBSTMR *))DoSplices);
    else DoSplices();
    init_turnoff();
    return 0;
}


