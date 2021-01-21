#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <map>
#include <conio.h>

#include <chCamera/chCamera.h>
#include <chUtilities/chUtilities.h>
#include <chMaths/chMaths.h>
#include <chMaths/chVector.h>
#include <chSystem/chSystem.h>
#include <chPajParser/chPajParser.h>
#include <chText/chText.h>

#include "chConstants.h"
#include "chParse.h"
#include "chControl.h"

#include "time.h"

// NOTES
// look should look through the libraries and additional files I have provided to familarise yourselves with the functionallity and code.
// The data is loaded into the data structure, managed by the linked list library, and defined in the chSystem library.
// You will need to expand the definitions for chNode and chArc in the chSystem library to include additional attributes for the siumulation process
// If you wish to implement the mouse selection part of the assignment you may find the camProject and camUnProject functions usefull


// core system global data
chCameraInput g_Input; // structure to hadle input to the camera comming from mouse/keyboard events
chCamera g_Camera; // structure holding the camera position and orientation attributes
chSystem g_System; // data structure holding the imported graph of data - you may need to modify and extend this to support your functionallity
chControl g_Control; // set of flag controls used in my implmentation to retain state of key actions

// global var: parameter name for the file to load
const static char csg_acFileParam[] = { "-input" };

const static float k = 0.1f; // the spring constant
const static float resting_length = 5.0f;
const static float time_step = 1.0f;
const static float dampening_coefficient = 0.2f;
const static float columb_constant = 0.1f;
const static float spring_constant = 0.1f;

const static int RENDER_DEFAULT = 0;
const static int RENDER_SPHERES = 1;

static int mainMenu;
static int renderModeMenu;
// global var: file to load data from
char g_acFile[256];

bool shouldRenderArcs = true; // used to tell OpenGl whether it should render the edges between nodes
bool shouldRenderNodes = true; // used to tell OpenGl whether it should render the nodes
bool shouldRenderText = true;  // used to tell OpenGl whether it should render the text label for the nodes
bool nodePositionIsRandom = false; // used to tell OpenGl whether nodes should be positioned randomly
bool simulationIsRunning = false; // used to tell OpenGl whether the solver should run

int renderMode;	// used to swap between different representations of the nodes

// core functions -> reduce to just the ones needed by glut as pointers to functions to fulfill tasks
void display(); // The rendering function. This is called once for each frame and you should put rendering code here
void idle(); // The idle function is called at least once per frame and is where all simulation and operational code should be placed
void reshape(int iWidth, int iHeight); // called each time the window is moved or resived
void keyboard(unsigned char c, int iXPos, int iYPos); // called for each keyboard press with a standard ascii key
void keyboardUp(unsigned char c, int iXPos, int iYPos); // called for each keyboard release with a standard ascii key
void sKeyboard(int iC, int iXPos, int iYPos); // called for each keyboard press with a non ascii key (eg shift)
void sKeyboardUp(int iC, int iXPos, int iYPos); // called for each keyboard release with a non ascii key (eg shift)
void mouse(int iKey, int iEvent, int iXPos, int iYPos); // called for each mouse key event
void motion(int iXPos, int iYPos); // called for each mouse motion event

// Non glut functions
void myInit(); // the myinit function runs once, before rendering starts and should be used for setup
void nodeDisplay(chNode* pNode); // callled by the display function to draw nodes
void arcDisplay(chArc* pArc); // called by the display function to draw arcs
void buildGrid(); 
void generateRandomPositions(chNode* pNode); // will generate random vertecies for each node to give new positions
void resetForce(chNode* pNode);
void calculateDistance(chArc* pArc);
void createMenu();
void processMenuEvents(int option);
void renderDefault(chNode* pNode, unsigned int worldSystem);
void renderSpheres(chNode* pNode);

void createMenu() {
	
	mainMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Toggle Edges", 1);
	glutAddMenuEntry("Toggle Nodes", 2);
	glutAddMenuEntry("Toggle Text", 3);
	glutAddMenuEntry("Toggle Grid", 4);
	glutAddMenuEntry("Toggle Solver", 5);
	glutAddMenuEntry("Render Spheres", 6);
	glutAddMenuEntry("Render Default", 7);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {
	switch (option)
	{
	case 1:
		shouldRenderArcs = !shouldRenderArcs;
		break;
	case 2:
		shouldRenderNodes = !shouldRenderNodes;
		break;
	case 3:
		shouldRenderText = !shouldRenderText;
		break;
	case 4:
		controlToggle(g_Control, csg_uiControlDrawGrid);
		break;
	case 5:
		simulationIsRunning = !simulationIsRunning;
		break;
	case 6:
		renderMode = RENDER_SPHERES;
		break;
	case 7:
		renderMode = RENDER_DEFAULT;
		break;
	default:
		break;
	}
}
void nodeDisplay(chNode* pNode) // function to render a node (called from display())
{
	float* position; // The world position of the node

	if (nodePositionIsRandom) {
		position = pNode->m_afRandomPosition;
	}
	else {
		position = pNode->m_afPosition;
	}
	unsigned int continent = pNode->m_uiContinent; // The continent id of the nodes country
	unsigned int worldSystem = pNode->m_uiWorldSystem; // The system the nodes country belongs to (IE: England = 1st world) (even under the tories...)

	glPushMatrix(); // Push current matrix
	glPushAttrib(GL_ALL_ATTRIB_BITS); // Push current attributes


	if (continent == 1) { // Africa
		float afCol[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
		utilitiesColourToMat(afCol, 2.0f);
	}

	if (continent == 2) { // Asia
		float afCol[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
		utilitiesColourToMat(afCol, 2.0f);
	}

	if (continent == 3) { // Europe
		float afCol[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
		utilitiesColourToMat(afCol, 2.0f);
	}

	if (continent == 4) { // North America
		float afCol[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
		utilitiesColourToMat(afCol, 2.0f);
	}

	if (continent == 5) { // Oceania
		float afCol[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		utilitiesColourToMat(afCol, 2.0f);
	}

	if (continent == 6) { // South America
		float afCol[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
		utilitiesColourToMat(afCol, 2.0f);
	}

	glTranslated(position[0], position[1], position[2]); // Translate the camera to the nodes position

	if (renderMode == RENDER_DEFAULT) {
		renderDefault(pNode, worldSystem);
	}

	if (renderMode == RENDER_SPHERES) {
		renderSpheres(pNode);
	}

	if (shouldRenderText) {
		glTranslatef(0.0f, 20.0f, 0.0f); // Translate so text will render 20 units above center node
		glScalef(10.0f, 10.0f, 10.0f); // Scale up the text for readability
		glMultMatrixf(camRotMatInv(g_Camera)); // Align the currnt stack with the camera (so text is always facing camera)
		outlinePrint(pNode->m_acName, true); // Render the countries name as a text label
	}


	glPopMatrix(); // Pop matrix and return to previous state
	glPopAttrib(); // Pop attributes and return to previous state
}

void renderDefault(chNode* pNode, unsigned int worldSystem) {
	if (worldSystem == 1) { // First world
		glutSolidSphere(mathsRadiusOfSphereFromVolume(pNode->m_fMass), 15, 15);
	}

	if (worldSystem == 2) { // Second world
		glutSolidCube(mathsDimensionOfCubeFromVolume(pNode->m_fMass));
	}

	if (worldSystem == 3) { // Third world
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Rotate cone to be facing upwards
		glutSolidCone(mathsRadiusOfConeFromVolume(pNode->m_fMass), 25, 15, 15);
		glRotatef(90, 1.0f, 0.0f, 0.0f); // Reverse rotation back for text placement
	}


}

void renderSpheres(chNode* pNode) {
	glutSolidSphere(mathsRadiusOfSphereFromVolume(pNode->m_fMass), 15, 15);
}
void arcDisplay(chArc* pArc) // function to render an arc (called from display())
{
	chNode* m_pNode0 = pArc->m_pNode0; // Get a refference to the node representing the start of the arc
	chNode* m_pNode1 = pArc->m_pNode1; // Get a refference to the node representing the end of the arc

	float* arcPos0; // pull the position of the node representing the start of the arc
	float* arcPos1; // pull the position of the node representing the end of the arc

	if (nodePositionIsRandom) {
		arcPos0 = m_pNode0->m_afRandomPosition;
		arcPos1 = m_pNode1->m_afRandomPosition;
	}
	else {
		arcPos0 = m_pNode0->m_afPosition;
		arcPos1 = m_pNode1->m_afPosition;
	}

	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(arcPos0[0], arcPos0[1], arcPos0[2]);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(arcPos1[0], arcPos1[1], arcPos1[2]);

	glEnd();
}

// This function will generate a random vertex for each node present in the list
void generateRandomPositions(chNode* pNode) {
	pNode->m_afRandomPosition[0] = randFloat(-500, 500);
	pNode->m_afRandomPosition[1] = randFloat(-500, 500);
	pNode->m_afRandomPosition[2] = randFloat(-500, 500);
}

// Step 1 of simulation
void resetForce(chNode* pNode) {
	// reset the resultant force F to 0
	pNode->m_afForce[0] = 0.0f;
	pNode->m_afForce[1] = 0.0f;
	pNode->m_afForce[2] = 0.0f;
}



void hookes(chArc* pArc) {
	chNode* pNode0 = pArc->m_pNode0; // Reference to node at start of arc
	chNode* pNode1 = pArc->m_pNode1; // Reference to node at end of arc

	float springLength = pArc->m_fIdealLen; // get the ideal length of the spring (rest)
	//printf("Ideal spring length: %f\n", springLength);
	float dx = pNode0->m_afPosition[0] - pNode1->m_afPosition[0]; // Get x distance between the nodes
	float dy = pNode0->m_afPosition[1] - pNode1->m_afPosition[1]; // Get y distance between the nodes
	float dz = pNode0->m_afPosition[2] - pNode1->m_afPosition[2]; // Get z distance between the nodes

	float distance = sqrt(dx * dx + dy * dy + dz * dz); // Use classical distance formulae

	float d = distance - springLength; // calculate displacement of spring
	/*printf("Displacement: %f\n\n", d);*/

	float xUnit = dx / distance;
	float yUnit = dy / distance;
	float zUnit = dz / distance;

	float springForceX = -1 * spring_constant * d * xUnit; // calculate spring force x axis
	float springForceY = -1 * spring_constant * d * yUnit; // calculate spring force y axis
	float springForceZ = -1 * spring_constant * d * zUnit; // calculate spring force z axis

	pNode0->m_afForce[0] += springForceX; // apply force to start nodes x axis
	pNode0->m_afForce[1] += springForceY; // apply force to start nodes y axis
	pNode0->m_afForce[2] += springForceZ; // apply force to start nodes z axis

}

void coulombs(chArc* pArc) {
	chNode* pNode0 = pArc->m_pNode0; // Reference to node at start of arc
	chNode* pNode1 = pArc->m_pNode1; // Reference to node at end of arc

	float dx = pNode0->m_afPosition[0] - pNode1->m_afPosition[0]; // Get x distance between the nodes
	float dy = pNode0->m_afPosition[1] - pNode1->m_afPosition[1]; // Get y distance between the nodes
	float dz = pNode0->m_afPosition[2] - pNode1->m_afPosition[2]; // Get z distance between the nodes

	float distance = sqrt(dx * dx + dy * dy + dz * dz); // Use classical distance formulae

	float xUnit = dx / distance;
	float yUnit = dy / distance;
	float zUnit = dz / distance;

	float coulombForceX = columb_constant * (pNode0->m_fMass * pNode1->m_fMass) / pow(distance, 2.0f) * xUnit;
	float coulombForceY = columb_constant * (pNode0->m_fMass * pNode1->m_fMass) / pow(distance, 2.0f) * yUnit;
	float coulombForceZ = columb_constant * (pNode0->m_fMass * pNode1->m_fMass) / pow(distance, 2.0f) * zUnit;

	pNode0->m_afForce[0] += coulombForceX;
	pNode0->m_afForce[1] += coulombForceY;
	pNode0->m_afForce[2] += coulombForceZ;
}

void moveNodes(chArc* pArc) {
	chNode* pNode0 = pArc->m_pNode0;

	if (nodePositionIsRandom) {
		pNode0->m_afRandomPosition[0] += pNode0->m_afForce[0] / pNode0->m_fMass;
		pNode0->m_afRandomPosition[1] += pNode0->m_afForce[1] / pNode0->m_fMass;
		pNode0->m_afRandomPosition[2] += pNode0->m_afForce[2] / pNode0->m_fMass;
	}
	else {
		pNode0->m_afPosition[0] += pNode0->m_afForce[0] / pNode0->m_fMass;
		pNode0->m_afPosition[1] += pNode0->m_afForce[1] / pNode0->m_fMass;
		pNode0->m_afPosition[2] += pNode0->m_afForce[2] / pNode0->m_fMass;
	}
}

void calculateMotion(chNode* pNode) {
	
	// Calculate acceleration due to the spring force.
	// f=ma therefore a=f/m
	pNode->m_afAcceleration[0] = pNode->m_afForce[0] / pNode->m_fMass;
	pNode->m_afAcceleration[1] = pNode->m_afForce[1] / pNode->m_fMass;
	pNode->m_afAcceleration[2] = pNode->m_afForce[2] / pNode->m_fMass;
	
	pNode->m_afVelocity[0] = (pNode->m_afVelocity[0] + time_step * pNode->m_afAcceleration[0]) * dampening_coefficient;
	pNode->m_afVelocity[1] = (pNode->m_afVelocity[1] + time_step * pNode->m_afAcceleration[1]) * dampening_coefficient;
	pNode->m_afVelocity[2] = (pNode->m_afVelocity[2] + time_step * pNode->m_afAcceleration[2]) * dampening_coefficient;

	if (nodePositionIsRandom) {
		pNode->m_afRandomPosition[0] = pNode->m_afRandomPosition[0] + time_step * pNode->m_afVelocity[0] + pNode->m_afAcceleration[0] * pow(time_step, 2.0f) / 2.0f;
		pNode->m_afRandomPosition[1] = pNode->m_afRandomPosition[1] + time_step * pNode->m_afVelocity[1] + pNode->m_afAcceleration[1] * pow(time_step, 2.0f) / 2.0f;
		pNode->m_afRandomPosition[2] = pNode->m_afRandomPosition[2] + time_step * pNode->m_afVelocity[2] + pNode->m_afAcceleration[2] * pow(time_step, 2.0f) / 2.0f;
	}
	else {
		pNode->m_afPosition[0] = pNode->m_afPosition[0] + time_step * pNode->m_afVelocity[0] + pNode->m_afAcceleration[0] * pow(time_step, 2.0f) / 2.0f;
		pNode->m_afPosition[1] = pNode->m_afPosition[1] + time_step * pNode->m_afVelocity[1] + pNode->m_afAcceleration[1] * pow(time_step, 2.0f) / 2.0f;
		pNode->m_afPosition[2] = pNode->m_afPosition[2] + time_step * pNode->m_afVelocity[2] + pNode->m_afAcceleration[2] * pow(time_step, 2.0f) / 2.0f;
	}
}

// draw the scene. Called once per frame and should only deal with scene drawing (not updating the simulator)
void display()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // clear the rendering buffers

	glLoadIdentity(); // clear the current transformation state
	glMultMatrixf(camObjMat(g_Camera)); // apply the current camera transform

	// draw the grid if the control flag for it is true	
	if (controlActive(g_Control, csg_uiControlDrawGrid)) glCallList(gs_uiGridDisplayList);

	glPushAttrib(GL_ALL_ATTRIB_BITS); // push attribute state to enable constrained state changes

	if (shouldRenderNodes) {
		visitNodes(&g_System, nodeDisplay); // loop through all of the nodes and draw them with the nodeDisplay function
	}

	if (shouldRenderArcs) {
		visitArcs(&g_System, arcDisplay); // loop through all of the arcs and draw them with the arcDisplay function
	}

	glPopAttrib();


	glFlush(); // ensure all the ogl instructions have been processed
	glutSwapBuffers(); // present the rendered scene to the screen
}

// processing of system and camera data outside of the renderng loop
void idle()
{
	if (simulationIsRunning) 
	{
		visitNodes(&g_System, resetForce); // for each body, reset the resultant force (f) to zero
		//visitArcs(&g_System, coulombs); // Apply coulombs 
		visitArcs(&g_System, hookes); // apply hookes law
		visitNodes(&g_System, calculateMotion); // calculate amount of movement for each body

	}
	controlChangeResetAll(g_Control); // re-set the update status for all of the control flags
	camProcessInput(g_Input, g_Camera); // update the camera pos/ori based on changes since last render
	camResetViewportChanged(g_Camera); // re-set the camera's viwport changed flag after all events have been processed
	glutPostRedisplay();// ask glut to update the screen
}

// respond to a change in window position or shape
void reshape(int iWidth, int iHeight)
{
	glViewport(0, 0, iWidth, iHeight);  // re-size the rendering context to match window
	camSetViewport(g_Camera, 0, 0, iWidth, iHeight); // inform the camera of the new rendering context size
	glMatrixMode(GL_PROJECTION); // switch to the projection matrix stack 
	glLoadIdentity(); // clear the current projection matrix state
	gluPerspective(csg_fCameraViewAngle, ((float)iWidth) / ((float)iHeight), csg_fNearClip, csg_fFarClip); // apply new state based on re-sized window
	glMatrixMode(GL_MODELVIEW); // swap back to the model view matrix stac
	glGetFloatv(GL_PROJECTION_MATRIX, g_Camera.m_afProjMat); // get the current projection matrix and sort in the camera model
	glutPostRedisplay(); // ask glut to update the screen
}

// detect key presses and assign them to actions
void keyboard(unsigned char c, int iXPos, int iYPos)
{
	switch (c)
	{
	case 'w':
		camInputTravel(g_Input, tri_pos); // mouse zoom
		break;
	case 's':
		camInputTravel(g_Input, tri_neg); // mouse zoom
		break;
	case 'c':
		camPrint(g_Camera); // print the camera data to the comsole
		break;
	case 'g':
		controlToggle(g_Control, csg_uiControlDrawGrid); // toggle the drawing of the grid
		break;
	case '1':
		shouldRenderNodes = !shouldRenderNodes; // Toggle the drawing of the nodes
		printf("[INFO]: Should render nodes changed: new Value: %s \n", shouldRenderNodes ? "True" : "False");
		break;
	case '2':
		shouldRenderArcs = !shouldRenderArcs; // Toggle the drawing of the arcs
		printf("[INFO]: Should render arcs changed: new Value: %s \n", shouldRenderArcs ? "True" : "False");
		break;
	case '3':
		//shouldRenderText = !shouldRenderText; // Toggle the drawing of text (country names)
		printf("[INFO]: Should render text changed: new Value: %s \n", shouldRenderText ? "True" : "False");
		break;
	case 'r':
		if (!nodePositionIsRandom) {
			visitNodes(&g_System, generateRandomPositions); // generate a new set of positions for the nodes
		}
		nodePositionIsRandom = !nodePositionIsRandom;
		simulationIsRunning = false; // dont want the simulation to continue running between changes
		printf("[INFO]: Node position is random changed: new Value %s \n", nodePositionIsRandom ? "True" : "False");
		break;
	case 't':
		simulationIsRunning = !simulationIsRunning; // Toggle the simulation
		printf("[INFO]: Simulation running changed: new Value is %s \n", simulationIsRunning ? "True" : "False");
	}
}

// detect standard key releases
void keyboardUp(unsigned char c, int iXPos, int iYPos)
{
	switch (c)
	{
		// end the camera zoom action
	case 'w':
	case 's':
		camInputTravel(g_Input, tri_null);
		break;
	}
}

void sKeyboard(int iC, int iXPos, int iYPos)
{
	// detect the pressing of arrow keys for ouse zoom and record the state for processing by the camera
	switch (iC)
	{
	case GLUT_KEY_UP:
		camInputTravel(g_Input, tri_pos);
		break;
	case GLUT_KEY_DOWN:
		camInputTravel(g_Input, tri_neg);
		break;
	}
}

void sKeyboardUp(int iC, int iXPos, int iYPos)
{
	// detect when mouse zoom action (arrow keys) has ended
	switch (iC)
	{
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		camInputTravel(g_Input, tri_null);
		break;
	}
}

void mouse(int iKey, int iEvent, int iXPos, int iYPos)
{
	// capture the mouse events for the camera motion and record in the current mouse input state
	if (iKey == GLUT_LEFT_BUTTON)
	{
		camInputMouse(g_Input, (iEvent == GLUT_DOWN) ? true : false);
		if (iEvent == GLUT_DOWN)camInputSetMouseStart(g_Input, iXPos, iYPos);
	}
	else if (iKey == GLUT_MIDDLE_BUTTON)
	{
		camInputMousePan(g_Input, (iEvent == GLUT_DOWN) ? true : false);
		if (iEvent == GLUT_DOWN)camInputSetMouseStart(g_Input, iXPos, iYPos);
	}
}

void motion(int iXPos, int iYPos)
{
	// if mouse is in a mode that tracks motion pass this to the camera model
	if (g_Input.m_bMouse || g_Input.m_bMousePan) camInputSetMouseLast(g_Input, iXPos, iYPos);
}


void myInit()
{
	// setup my event control structure
	controlInit(g_Control);

	// initalise the maths library
	initMaths();

	// Camera setup
	camInit(g_Camera); // initalise the camera model
	camInputInit(g_Input); // initialise the persistant camera input data 
	camInputExplore(g_Input, true); // define the camera navigation mode

	// opengl setup - this is a basic default for all rendering in the render loop
	glClearColor(csg_afColourClear[0], csg_afColourClear[1], csg_afColourClear[2], csg_afColourClear[3]); // set the window background colour
	glEnable(GL_DEPTH_TEST); // enables occusion of rendered primatives in the window
	glEnable(GL_LIGHT0); // switch on the primary light
	glEnable(GL_LIGHTING); // enable lighting calculations to take place
	glEnable(GL_BLEND); // allows transparency and fine lines to be drawn
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // defines a basic transparency blending mode
	glEnable(GL_NORMALIZE); // normalises the normal vectors used for lighting - you may be able to switch this iff (performance gain) is you normalise all normals your self
	glEnable(GL_CULL_FACE); // switch on culling of unseen faces
	glCullFace(GL_BACK); // set culling to not draw the backfaces of primatives

	// build the grid display list - display list are a performance optimization 
	buildGrid();

	// initialise the data system and load the data file
	initSystem(&g_System);
	parse(g_acFile, parseSection, parseNetwork, parseArc, parsePartition, parseVector);
	visitNodes(&g_System, generateRandomPositions); // generate random positions to switch between
	renderMode = RENDER_DEFAULT;
	createMenu();
}

int main(int argc, char* argv[])
{
	// check parameters to pull out the path and file name for the data file
	for (int i = 0; i < argc; i++) if (!strcmp(argv[i], csg_acFileParam)) sprintf_s(g_acFile, "%s", argv[++i]);


	if (strlen(g_acFile))
	{
		// if there is a data file

		glutInit(&argc, (char**)argv); // start glut (opengl window and rendering manager)

		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // define buffers to use in ogl
		glutInitWindowPosition(csg_uiWindowDefinition[csg_uiX], csg_uiWindowDefinition[csg_uiY]);  // set rendering window position
		glutInitWindowSize(csg_uiWindowDefinition[csg_uiWidth], csg_uiWindowDefinition[csg_uiHeight]); // set rendering window size
		glutCreateWindow("chAssignment1-2020");  // create rendering window and give it a name

		buildFont(); // setup text rendering (use outline print function to render 3D text


		myInit(); // application specific initialisation

		// provide glut with callback functions to enact tasks within the event loop
		glutDisplayFunc(display);
		glutIdleFunc(idle);
		glutReshapeFunc(reshape);
		glutKeyboardFunc(keyboard);
		glutKeyboardUpFunc(keyboardUp);
		glutSpecialFunc(sKeyboard);
		glutSpecialUpFunc(sKeyboardUp);
		glutMouseFunc(mouse);
		glutMotionFunc(motion);
		glutMainLoop(); // start the rendering loop running, this will only ext when the rendering window is closed 
		killFont(); // cleanup the text rendering process

		return 0; // return a null error code to show everything worked
	}
	else
	{
		// if there isn't a data file 

		printf("The data file cannot be found, press any key to exit...\n");
		_getch();
		return 1; // error code
	}
}

void buildGrid()
{
	if (!gs_uiGridDisplayList) gs_uiGridDisplayList = glGenLists(1); // create a display list

	glNewList(gs_uiGridDisplayList, GL_COMPILE); // start recording display list

	glPushAttrib(GL_ALL_ATTRIB_BITS); // push attrib marker
	glDisable(GL_LIGHTING); // switch of lighting to render lines

	glColor4fv(csg_afDisplayListGridColour); // set line colour

	// draw the grid lines
	glBegin(GL_LINES);
	for (int i = (int)csg_fDisplayListGridMin; i <= (int)csg_fDisplayListGridMax; i++)
	{
		glVertex3f(((float)i) * csg_fDisplayListGridSpace, 0.0f, csg_fDisplayListGridMin * csg_fDisplayListGridSpace);
		glVertex3f(((float)i) * csg_fDisplayListGridSpace, 0.0f, csg_fDisplayListGridMax * csg_fDisplayListGridSpace);
		glVertex3f(csg_fDisplayListGridMin * csg_fDisplayListGridSpace, 0.0f, ((float)i) * csg_fDisplayListGridSpace);
		glVertex3f(csg_fDisplayListGridMax * csg_fDisplayListGridSpace, 0.0f, ((float)i) * csg_fDisplayListGridSpace);
	}
	glEnd(); // end line drawing

	glPopAttrib(); // pop attrib marker (undo switching off lighting)

	glEndList(); // finish recording the displaylist
}
