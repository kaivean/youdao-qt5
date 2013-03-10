/* 
 * Compile:
 * gcc -g -L/usr/lib -lX11 -lXtst XRecord.c -o XRecord
 * 
 * Reference:
 * http://www.x.org/docs/Xext/recordlib.pdf
 * http://blogs.sun.com/yongsun/entry/my_first_blog_in_sun
 * http://lists.x.org/archives/xorg/2005-March/006629.html
 * 
 * To enable record extension in Xorg/XFree86, add the following line in
 * Section "Module"
 *       Load "record"
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlibint.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysymdef.h>
#include <X11/keysym.h>

#include <X11/Xlib.h>
#include <X11/extensions/record.h>

Display* display;
XRecordContext context;

/* for this struct, refer to libxnee */
typedef union {
        unsigned char           type ;
        xEvent                          event ;
        xResourceReq            req      ;
        xGenericReply           reply ;
        xError                           error ;
        xConnSetupPrefix        setup;
} XRecordDatum;

/* stop flag */
int stop = 0;

static void callback(XPointer priv, XRecordInterceptData* hook) {
        
        
        // FIXME: we need use XQueryPointer to get the first location
        static int cur_x = 0;
        static int cur_y = 0;
        
        if (hook->category != XRecordFromServer && hook->category != XRecordFromClient) {
                XRecordFreeData (hook);
                return;
        }


        XRecordDatum *data = (XRecordDatum*) hook->data;
        
        int event_type = data->type;
        
        BYTE btncode, keycode;
        btncode = keycode = data->event.u.u.detail;
        
        int rootx = data->event.u.keyButtonPointer.rootX;
        int rooty = data->event.u.keyButtonPointer.rootY;
        int time = hook->server_time;
        int f=0;
        switch (event_type) {
                case KeyPress:
                        // if escape is pressed, stop the loop and clean up, then exit
                        if (keycode == 9) {
                                //stop = 1;
                                XRecordFreeData (hook);
                                XRecordDisableContext(XOpenDisplay( NULL ), context);
                                break;
                        }
                        
                        // Note: you should not use data_disp to do normal X operations !!!
                        printf ("KeyPress: \t%s", XKeysymToString(XKeycodeToKeysym( XOpenDisplay( NULL ), keycode, 0)));
                        //printf ("KeyPress: \t%i", XKeysymToKeycode( XOpenDisplay( NULL ), XK_A));
                        break;
                case KeyRelease:
                        printf ("KeyRelease: \t%s", XKeysymToString(XKeycodeToKeysym( XOpenDisplay( NULL ), keycode, 0)));
                        break;
                case ButtonPress:
 			f=0;
                       // printf ("ButtonPress: \t%d, rootX=%d, rootY=%d", btncode, cur_x, cur_y);
                        break;
                case ButtonRelease:
			if(btncode==1)  f=1;
                       // printf ("ButtonRelease: \t%d, rootX=%d, rootY=%d", btncode, cur_x, cur_y);
                        break;

                default:
                        break;
        }
        if(f==0) printf("0");  

	if(f==1)  printf("1");  
     
      
        
        XRecordFreeData (hook);
}


void main() {
   
   setvbuf(stdout, (char *)NULL, _IONBF, 0);
   //printf("beginning\n");
   display = XOpenDisplay( NULL );
   if (display == 0) {
     fprintf(stderr, "unable to open display\n");
     exit(-1);
   }

   XRecordClientSpec clients = XRecordAllClients;
   XRecordRange* range = XRecordAllocRange();
   if (range == 0) {
     fprintf(stderr, "unable to allocate XRecordRange\n");
     exit(-1);
   }

   range->device_events.first = ButtonPress;
   range->device_events.last = ButtonRelease;
   context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
   if (context == 0) {
     fprintf(stderr, "unable to create XRecordContext\n");
     exit(-1);
   }

   XFree(range);

   //XRecordEnableContextAsync(display, context, callback, 0);
   XRecordEnableContext(display, context, callback, 0);

   //while (stop == 0) {
           //XRecordProcessReplies (display);
           //printf("%i\n\n", stop);
   //}
   
   //XRecordDisableContext(display, context);
   XRecordFreeContext(display, context);
   XCloseDisplay(display);
//printf("beginning\n");
  // return 0;
}


