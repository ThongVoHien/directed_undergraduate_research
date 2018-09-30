/***********************************************************
    PROGRAM FOR DRAWING SCHOENBERG CURVE FOR MATH 396
 (CONTINUOUS, ONTO MAPPING FROM UNIT SEGMENT TO UNIT SQUARE)
 ***********************************************************/


#include <cstdio>
#include <cstring>
#include <cctype>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <climits>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <bitset>
#include <stack>
#include <queue>
#include <set>
#include <list>
#include <unistd.h>

#include <time.h>
#define fr(a) freopen(a,"r",stdin)
#define fw(a) freopen(a,"w",stdout)
#define reset(a) memset(a,0,sizeof(a))
#define upf(i,m,n) for (int i=m;i<=n;i++)
#define downf(i,n,m) for (int i=n;i>=m;i--)
#define all(a) a.begin(),a.end()
#define PB push_back
#define InS insert
#define mP make_pair
#define mT make_tuple
#define EB emplace_back
#define X first
#define Y second
#define bug(x) cout << #x << " = " << x <<" ";
#define line() cout << endl;
#define Sbug(x) cout << #x << " = " << x <<"\n";
#define tc() int tc; cin >> tc; for(int _tc=0;_tc<tc;_tc++)
#define sl(n) scanf("%lld",&n)
#define slf(n) scanf("%lf",&n)
#define ss(n) scanf("%s",n)
#define pl(n) printf("%lld",n)
#define ps(n) printf("%s",n)
#define PQ priority_queue
#define IT iterator
#define stop() int _stop;cin>>_stop;
#define PrintAll(x) for (auto i:x) cout << i << " "; line();
#define rep(i, begin, end) for (__typeof(end) i = (begin) - ((begin) > (end)); i != (end) - ((begin) > (end)); i += 1 - 2 * ((begin) > (end)))
#include <limits.h>

#ifdef MACOSX
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;
typedef long long int ll;
typedef unsigned long long int ull;
typedef long double lf;
typedef pair<int,int>pii;
typedef pair<ll,ll>pll;
typedef vector<int> vi;
typedef vector<ll> vll;

double n, x, y, noPoint, t = 0;
unsigned long noPointDraw = 0, noPointPerStep = 1;
double Pi = 3.1415926535897;
// noPointDraw: number of points on the graph at that time
// noPointPerStep: number of points increasing per step
vector< pair<double, double> > setOfPoints;

/*******************************
    COMPUTATION FOR THE CURVE
 *******************************/
double p(double s){ // initial function, mapping [0,1] to [0,1]
    if (-1.0/3 <= s && s <= 1.0/3) return 0;
    if ( (-2.0/3 <= s && s <= -1.0/3) || (1.0/3 <= s && s <= 2.0/3) ) return abs(3*s)-1;
    return 1;
}
double p_extended(double s){ // funciton mapping R to [0,1], extended from p
    if (s<0) return p_extended(-s);
    while (!(-1.0 <= s && s <= 1.0)) s-= floor( (s+1)/2 )*2;
    return p(s);
}
double computeAllPoint(){
    t = 0;
    setOfPoints.PB({0,0}); // initiate the set of all points on the graph

    upf(i,1,noPoint){ // each point on the unit segment
        x = 0; y = 0; // initiate coordination
        upf(k,0,n){
            x += p_extended( pow(3,2*k)*t )/pow(2,k+1);
            y += p_extended( pow(3,2*k+1)*t )/pow(2,k+1);
        }
        setOfPoints.PB({x,y});
        t += 1.0/(noPoint-1); // move to next point on segment line
    }
}

/************************************
    SET UP THE WINDOW USING OPENGL
 ************************************/
bool mouseIsDragging = false;
int WIDTH = 1000;  // width of the user window
int HEIGHT = 1000;  // height of the user window
char programName[] = "draw Schoenberg curve";

void drawWindow(){
    // each time this function is called, the number of points drawing increases
    if (noPointDraw < setOfPoints.size())
        noPointDraw += noPointPerStep;
    
    // clear the buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // draw the graph
    glBegin(GL_LINES);
    for (int i = 1; i < min( noPointDraw, setOfPoints.size() ); i++){
        glColor3f( sin(i*1.0/setOfPoints.size()*Pi/2+1), sin(i*1.0/setOfPoints.size()*Pi+1), sin(i*1.0/setOfPoints.size()*Pi*2+1));
        glVertex2d( setOfPoints[i-1].X, setOfPoints[i-1].Y);
        glVertex2d( setOfPoints[i].X, setOfPoints[i].Y);
    }
    glEnd();
    
    // draw the last point to make the graph easier to trace
    glColor3f(1,0,1);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex2f(setOfPoints[ min( noPointDraw, setOfPoints.size() )-1].X, setOfPoints[ min( noPointDraw, setOfPoints.size() )-1].Y);
    glEnd();

    // tell the graphics card that we're done-- go ahead and draw!
    //   (technically, we are switching between two color buffers...)
    glutSwapBuffers();
}

// process keyboard events
void keyboard( unsigned char c, int x, int y ){
    int win = glutGetWindow();
    switch(c) {
        case 'q':
        case 'Q':
        case 27:
            // get rid of the window (as part of shutting down)
            glutDestroyWindow(win);
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

// the reshape function handles the case where the user changes the size
//   of the window.  We need to fix the coordinate
//   system, so that the drawing area is still the unit square.
void reshape(int w, int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    WIDTH = w;  HEIGHT = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1., 1., -1., 1., -1.0, 1.0);
}

// the init function sets up the graphics card to draw properly
void init(void){
    // clear the window to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // set up the coordinate system:  number of pixels along x and y
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // set up the coordination of the window: left: -1, right: 1, bottom: -1, top: 1)
    glOrtho(-1., 1., -1., 1., -1.0, 1.0);

    // welcome message
    cout << "Welcome to " << programName << "." << endl;
}


// init_gl_window is the function that starts the ball rolling, in
//  terms of getting everything set up and passing control over to the
//  glut library for event handling.  It needs to tell the glut library
//  about all the essential functions:  what function to call if the
//  window changes shape, what to do to redraw, handle the keyboard,
//  etc.
void init_gl_window(){
    char *argv[] = {programName};
    int argc = sizeof(argv) / sizeof(argv[0]);
    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize(WIDTH,HEIGHT);
    glutInitWindowPosition(200,200);
    glutCreateWindow(programName);
    init();

    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(drawWindow);
    glutMainLoop();
}

int main(){
    cout << "Enter value n, total number of points, and number of points per step:\n";
    cin >> n >> noPoint >> noPointPerStep;
    computeAllPoint();
    init_gl_window();
}






