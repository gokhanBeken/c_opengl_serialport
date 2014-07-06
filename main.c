//kaynak: http://ceng.ktu.edu.tr/labs/opengl_2011.pdf  http://www.picproje.org/index.php/topic,14747.msg96607.html#msg96607
#include <string.h>
#include <stdlib.h>
#include "stdio.h"
#include "windows.h"
#include "GL/glut.h"

GLfloat rtri;
GLfloat rquad;

//yeni denemeler
GLfloat x_derece=45.0;
GLfloat y_derece=0;
GLfloat z_derece=0;

GLfloat x_durum,y_durum,z_durum;



//seriport için:
DCB dcb;
HANDLE hCom;
COMMTIMEOUTS CommTimeOuts;
DWORD nWrite;
DWORD nRead;

char SeriPortInit(void){
    // Port açma
    hCom = CreateFile((LPCTSTR)"\\\\.\\COM1", GENERIC_READ|GENERIC_WRITE, 
                       0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    // Açýldýmý ?
    if (hCom == INVALID_HANDLE_VALUE)
        return 0;
    
    // Giriþ çýkýþ buffer ayarla
    SetupComm(hCom, 4096, 4096);

    
    // mevcut ayarlarý oku
    GetCommState(hCom, &dcb);
    
    dcb.BaudRate = 19200;               /* Baudrate at which running       */
    dcb.fBinary = TRUE;                 /* Binary Mode (skip EOF check)    */
    dcb.fParity = FALSE;				/* Enable parity checking          */
    dcb.fOutxCtsFlow = FALSE;           /* CTS handshaking on output       */
    dcb.fOutxDsrFlow = FALSE;           /* DSR handshaking on output       */
    dcb.fDtrControl = DTR_CONTROL_ENABLE; /* DTR Flow control                */
    dcb.fDsrSensitivity = FALSE;        /* DSR Sensitivity              */
    dcb.fTXContinueOnXoff = FALSE;      /* Continue TX when Xoff sent */
    dcb.fOutX = FALSE;                  /* Enable output X-ON/X-OFF        */
    dcb.fInX = FALSE;                   /* Enable input X-ON/X-OFF         */
    dcb.fErrorChar = FALSE;             /* Enable Err Replacement          */
    dcb.fNull = FALSE;                  /* Enable Null stripping           */
    dcb.fRtsControl = RTS_CONTROL_ENABLE; /* Rts Flow control                */
    dcb.fAbortOnError = FALSE;          /* Abort all reads and writes on Error */
    dcb.ByteSize = 8;                   /* Number of bits/byte, 4-8        */
    dcb.Parity = 0;                     /* 0-4=None,Odd,Even,Mark,Space    */
    dcb.StopBits = ONESTOPBIT;          /* 0,1,2 = 1, 1.5, 2               */

    // ayar yap
    SetCommState(hCom, &dcb);

    // Timeout ayarla (2s)
    CommTimeOuts.ReadIntervalTimeout = 0;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = 100;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = 2000;
    
    // ayarlarý yaz
    SetCommTimeouts(hCom, &CommTimeOuts);
 
    // DTR RTS aktif yap
    EscapeCommFunction(hCom, SETDTR);
    EscapeCommFunction(hCom, SETRTS);


    // buffer leri önceden boþalt
    PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
}




void ayarlar(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-7.0, 7.0, -2.0, 2.0, -5.0, 5.0);
	
	rtri = 0.0f;
}

katardanFloata(char * katar){
	int i=0;
	//strlen(katar)
	int numaraTut=0,numaraTut2=0,numaraTut3=0;
	int virgulTut=0,virgulTut2=0,virgulTut3=0;
	
	for(i=0;i<strlen(katar);i++){
		if(katar[i]=='='){
			numaraTut=i+1;
			break;
		}
	}
	
	for(i=numaraTut;i<strlen(katar);i++){
		if(katar[i]=='='){
			numaraTut2=i+1;
			break;
		}
	}	
	
	for(i=numaraTut2;i<strlen(katar);i++){
		if(katar[i]=='='){
			numaraTut3=i+1;
			break;
		}
	}		
	//x=45,y=0,z=0\r 2,4,7,8,11,12
	//virgüller
	
	for(i=numaraTut;i<strlen(katar);i++){
		if(katar[i]==','){
			virgulTut=i;
			break;
		}
	}
	for(i=numaraTut2;i<strlen(katar);i++){
		if(katar[i]==','){
			virgulTut2=i;
			break;
		}
	}
	for(i=numaraTut3;i<strlen(katar);i++){
		if(katar[i]=='\r'){
			virgulTut3=i;
			break;
		}
	}	
	
	
	//çevirme iþi
	if(virgulTut-numaraTut==1){
		x_derece=(katar[numaraTut]-'0');
	}else if(virgulTut-numaraTut==2){
		x_derece=10*(katar[numaraTut]-'0');
		x_derece+=(katar[numaraTut+1]-'0');
	}else if(virgulTut-numaraTut==3){
		x_derece=100*(katar[numaraTut]-'0');
		x_derece+=10*(katar[numaraTut+1]-'0');
		x_derece+=(katar[numaraTut+2]-'0');
	}else{
	}
	
	if(virgulTut2-numaraTut2==1){
		y_derece=(katar[numaraTut2]-'0');
	}else if(virgulTut2-numaraTut2==2){
		y_derece=10*(katar[numaraTut2]-'0');
		y_derece+=(katar[numaraTut2+1]-'0');
	}else if(virgulTut2-numaraTut2==3){
		y_derece=100*(katar[numaraTut2]-'0');
		y_derece+=10*(katar[numaraTut2+1]-'0');
		y_derece+=(katar[numaraTut2+2]-'0');
	}else{
	}
	
	if(virgulTut3-numaraTut3==1){
		z_derece=(katar[numaraTut3]-'0');
	}else if(virgulTut3-numaraTut3==2){
		z_derece=10*(katar[numaraTut3]-'0');
		z_derece+=(katar[numaraTut3+1]-'0');
	}else if(virgulTut3-numaraTut3==3){
		z_derece=100*(katar[numaraTut3]-'0');
		z_derece+=10*(katar[numaraTut3+1]-'0');
		z_derece+=(katar[numaraTut3+2]-'0');
	}else{
	}		
	 
	
	//printf("%f , %f , %f \r\n",x_derece,y_derece,z_derece);
	
	

}

void idle()
{
		

	float bilgi;
	//char * gelenVeri="x=45,y=0,z=0\r"; //x:dikey, y: kumpas, z: yatay
	
	char * gelenVeri[20];

	//x_durum=1.0; y_durum=1.0; z_durum=0.0; //bu kýsým birþey deðiþtirmiyor, aþaðýda "gosterim" fonksiyonunda ayarlanýyor çünkü
	
	//rtri +=0.01f;
	//rquad+=0.15f; //bunun ne iþe yaradýðýný daha çözemedim
 

 
	memset(&gelenVeri[0], 0, sizeof(gelenVeri));
    ReadFile(hCom, gelenVeri, sizeof(gelenVeri), &nRead, NULL);
    if(gelenVeri[0]!=0)katardanFloata(gelenVeri); //gelen veri boþ deðilse iþle
    

	glutPostRedisplay();	    	
}


void keyboardCB(unsigned char key, int x, int y)
{
	rtri +=0.5f;
	
    switch(key)
    {
	    case 27: // ESCAPE
	        //clearSharedMem();
	        exit(0);
	        break;

	    case 'w': 
	    	x_derece-=1.0;
	    	x_durum=1.0; y_durum=0.0; z_durum=0.0;
	        break;
	        
	    case 'a': 
	        z_derece+=1.0; 
	        x_durum=0.0; y_durum=0.0; z_durum=0.1;
	        break;
			
	    case 's': 
	    	x_derece+=1.0;
	    	x_durum=0.1; y_durum=0.0; z_durum=0.0;
	        break;
			
	    case 'd': 
	        z_derece-=1.0;
	        x_durum=0.0; y_durum=0.0; z_durum=0.1;
	        break;		
			
		//kumpas:
		case 'z': 
	        y_derece-=1.0;
	        x_durum=0.0; y_durum=0.1; z_durum=0.0;
	        break;	
			
		case 'x': 
	        y_derece+=1.0;
	        x_durum=0.0; y_durum=0.1; z_durum=0.0;
	        break;									        

		//sýfýrla
		case 'o': 
			x_durum=0.1;
			y_durum=0.1;
			z_durum=0.1;
			
	        x_derece=45.0;
	        y_derece=0.0;
	        z_derece=0.0;
	        break;		
		
	        
    default:
    	//x_aci=-0.00f; y_aci=-0.00f;  z_aci=-0.00f;   
        ;
    }
    
    //iþi saðlama alalým diye yazdýk þart deðil:
    if(x_derece==360.0 || x_derece==-360.0)x_derece=0.0;
    if(y_derece==360.0 || y_derece==-360.0)x_derece=0.0;
    if(y_derece==360.0 || z_derece==-360.0)x_derece=0.0;
    
    glutPostRedisplay();

}

void gosterim(void)
{
	glEnable(GL_DEPTH_TEST);
	glColor3f(1.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	
	//nesneyi taþýmak: Nesneyi taþýmak: glTranslate{fd}( x, y, z )
	//bilgi: http://ozlemerden.wordpress.com/category/opengl/
	//Nesneyi bir eksen etrafýnda döndürmek: glRotate{fd}( açý, x, y, z )

	glPopMatrix();
	//glTranslatef(4.0f,0.0f,0.0f); 
	glPushMatrix();
	
	
	
	//glRotatef(rtri,x_aci,y_aci,z_aci); //açý dediði aslýnda ne kadar çok döneceðini ayarlýyor
	
	//if(x_durum==1)glRotatef(rtri,0.1f,0.0f,0.0f);
	//if(y_durum==1)glRotatef(rtri,0.0f,0.1f,0.0f);
	//if(z_durum==1)glRotatef(rtri,0.0f,0.0f,0.1f);
	
	
	glRotatef(x_derece,0.1f,0.0f,0.0f);
	glRotatef(y_derece,0.0f,0.1f,0.0f);
	glRotatef(z_derece,0.0f,0.0f,0.1f);
	
	
	glBegin(GL_QUADS);
	glColor3f(0.0f,1.0f,0.0f);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f,0.5f,0.0f);
	glVertex3f( 1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f( 1.0f,-1.0f,-1.0f);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glVertex3f( 1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,1.0f,0.0f);
	glVertex3f( 1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glColor3f(0.0f,0.0f,1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	glVertex3f( 1.0f,-1.0f, 1.0f);
	glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
	
	
}









int main(int argc,char ** argv)
{
	int i;

    
    
	for(i=0;i<argc;i++)	printf("%d , %s \r\n",i,argv[i]);

	SeriPortInit();
    
    // seri porta bilgi gönder
    WriteFile(hCom, "Merhaba\r\n", (DWORD) strlen("Merhaba\r\n"), &nWrite, NULL);
    
    
    /*
    // port kapat
    CloseHandle(hCom);
    */
    
    
    
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(900,400);
	glutCreateWindow("pencere");
	ayarlar();
	glutDisplayFunc(gosterim);
	
	glutKeyboardFunc(keyboardCB); //klavye fonksiyonum
	
	glutIdleFunc(idle); //arka plan deðer deðiþtirmek için genel fonksiyon
	glutMainLoop();
	
	    

    return 0;
}
