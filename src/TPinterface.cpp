#include "TPinterface.h"
#include <vector>
// buffer to be used to store the hits during picking
#define BUFSIZE 256

bool selec;
int sel;
char player;
using namespace std;

GLuint selectBuf[BUFSIZE];



TPinterface::TPinterface()
{
	
}


void TPinterface::initGUI()
{
	/*GLUI_Panel *varLuz= addPanel("Luzes", 2);
	addCheckboxToPanel(varLuz, "Luz 1",&((DemoScene *) scene)->state1,NULL);
	addColumn ();
	GLUI_Panel *varClock= addPanel("Relogio", 3);
	addStaticText("Rel�gio");
	addButton ("Pause/Start",1);
	addColumn ();*/
	GLUI_Panel *text= addPanel("Wireframe");
	GLUI_RadioGroup *group1=addRadioGroupToPanel(text,&((DemoScene *) scene)->wire);
	addRadioButtonToGroup(group1,"Normal");
	addRadioButtonToGroup(group1,"Wireframe");
	addColumn();
	GLUI_Panel *varwire= addPanel("Appearance");
	GLUI_RadioGroup *group=addRadioGroupToPanel(varwire,&((DemoScene *) scene)->texttab);
	addRadioButtonToGroup(group,"Standard");
	addRadioButtonToGroup(group,"Rock");
	addColumn();
	//socketConnect();
	//quit();
}
void TPinterface::processGUI(GLUI_Control *ctrl)
{	

}

void TPinterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);
	
	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void TPinterface::performPicking(int x, int y) 
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);

	// Initialize the picking name stack
	glInitNames();

	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);

	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();

	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);

	// reset projection matrix
	glLoadIdentity();

	// get current viewport and use it as reference for 
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);

	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);

	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();

	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();

	glFlush();

	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	processHits(hits, selectBuf);
}

void TPinterface::processHits (GLint hits, GLuint buffer[]) 
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected,x,y;
	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}
	
	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
		int size;
		for (int i=0; i<nselected; i++){
			
			if(i==0){
				x=selected[i];
				printf("%d ",x);
			}
			else{
				y=selected[i];
				printf("%d ",y);
				if(!selec){
				//if(((DemoScene *) scene)->turn=='a'){
					size= ((DemoScene *) scene)->tab->jog1Pecas.size();
					for(int j=0;j< size;j++){
						if((((DemoScene *) scene)->tab->jog1Pecas.at(j)->x==x) && (((DemoScene *) scene)->tab->jog1Pecas.at(j)->y==y)){
							printf(" Player A Piece selected");
							selec = true;
							sel = j;
							player = 'a';
						}
					}

				//}
				//else if(((DemoScene *) scene)->turn=='b'){
					size= ((DemoScene *) scene)->tab->jog2Pecas.size();
					for(int j=0;j< size;j++){
						if((((DemoScene *) scene)->tab->jog2Pecas.at(j)->x==x) && (((DemoScene *) scene)->tab->jog1Pecas.at(j)->y==y))
							printf(" Player B Piece");
					}
				//}
				}
				else
				{
					selec = false;
					((DemoScene *) scene)->tab->jog1Pecas.at(sel)->setX(x);
					((DemoScene *) scene)->tab->jog1Pecas.at(sel)->setY(y);
				}
			}

		}
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}

bool TPinterface::socketConnect() {// Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
		printf("Client: Error at WSAStartup().\n");
    else
        printf("Client: WSAStartup() is OK.\n");

	// Create a socket.
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return false;
    }
	else
       printf("Client: socket() is OK.\n");

    // Connect to a server.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    // Just test using the localhost, you can try other IP address
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(60070);

    if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Client: connect() - Failed to connect.\n");
        WSACleanup();
        return false;
    }
    else {
       printf("Client: connect() is OK.\n");
       printf("Client: Can start sending and receiving data...\n");
    }

    // Send and receive data.
	printf("Connected\n");
	return true;
}
void TPinterface::envia(char *s, int len) {
	int bytesSent = send(m_socket, s, len, 0);
	if(bytesSent == SOCKET_ERROR)
		printf("Client: send() error %ld.\n", WSAGetLastError());
}
void TPinterface::recebe(char *ans) {
	int bytesRecv = SOCKET_ERROR;
	int pos = 0;
	while (true) {
		recv(m_socket, &ans[pos], 1, 0);
		if (ans[pos] == '\n')
			break;
		pos++;
	}
	ans[pos] = 0;
	
}
void TPinterface::quit() {
	cout << "Asking prolog to quit" << endl;
	char buff[] = "quit.\n";
	envia(buff, 6);
	char ans[128];
	recebe(ans);
}
