#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "src/utils.h"
#include "src/SectorRoom.h"
#include "src/furniture_components.h"
#include "src/vector.h"
#include "src/Rooms.h"

// extern params
extern Room room_types[8];

// Global params
double width, height;
double r1 = 3.4;
double r2;
double wf;
double north_angle;
Room * list_rooms = NULL;
int required_rooms[] = {0, 1, 2, 3, 3, 3, 2, 1, 6, 4, 5};
int aditional_rooms[] = {7, 3, 2};

double random_area(int interval[2])
{
    srand(time(NULL)); // initialize random seed
    int random = rand() % (interval[1] - interval[0]) + interval[0];
    return (double) random;
}

void initialize_required_list()
{
    for(int i = 0; i < 11; i++)
    {
        Room aux = room_types[required_rooms[i]];
        double area = random_area(aux.area_interval);
        //printf("area random: %.2lf\n", area);
        pushRoom(&list_rooms, aux, area, wf);
    }
}

void draw_room_list(Room * list)
{
    double init_angle = -10, new_angle;
    Room * temp = list;

    temp->super->bg_angle = init_angle;
    temp->super->draw(temp->super, r1 - WALL, r2, 1);
    new_angle = temp->super->bg_angle + temp->super->delta;
    temp = temp->next;
    while(temp != list)
    {
        temp->super->bg_angle = new_angle;
        temp->super->draw(temp->super, r1 - WALL, r2, 1);
        new_angle = temp->super->bg_angle + temp->super->delta;
        temp = temp->next;
    }
}

double calc_rest_area(Room * list)
{
    double dangle, area;
    Room * prev = list->prev;


    dangle = (list->super->bg_angle + 360) - (prev->super->bg_angle + prev->super->delta);

    area = (dangle * M_PI * (r2 - (r1 - WALL)) * (r2 + (r1 - WALL)))/360;

    printf("Bg angle first = %.2lf, End angle last = %.2lf\n", list->super->bg_angle,
    prev->super->bg_angle + prev->super->delta);
    printf("Area restante: %.2lf\n", area);
    return area;
}

int find_rooms_below_max(Room * list)
{
    int count = 0;
    Room * temp = list;
    do{
        if(temp->super->area < temp->area_interval[1]) count++;
        temp = temp->next;
    }while(temp != list);

    return count;
}

int find_rooms_above_min(Room * list)
{
    int count = 0;
    Room * temp = list;
    do{
        if(temp->super->area > temp->area_interval[0]) count++;
        temp = temp->next;
    }while(temp != list);

    return count;
}

void propagate_new_area(Room * list, double add_area)
{
    double limit, true_add;
    Room * temp = list;
    do{
        if(add_area > 0)
        {
            if(temp->super->area < temp->area_interval[1])
            {
                limit = temp->area_interval[1] - temp->super->area;
                true_add = (add_area > limit) ? limit : add_area;
                temp->super->area += true_add;
                temp->super->true_area += true_add;
            }
        }
        else
        {
            if(temp->super->area > temp->area_interval[0])
            {
                limit = temp->area_interval[0] - temp->super->area;
                true_add = (add_area < limit) ? limit : add_area;
                temp->super->area += true_add;
                temp->super->true_area += true_add;
            }
        }
        temp = temp->next;
    }while(temp != list);
}

int try_add_new_room(Room * list, double area)
{
    for(int i = 0; i < 3; i++)
    {
        Room room = room_types[aditional_rooms[i]];
        if(area >= room.area_interval[0] && area <= room.area_interval[1])
        {
            pushRoom(&list, room, area, wf);
            return 1;
        }
    }
    return 0;
}

void propagate_without_limit(Room * list, double area)
{
    Room * temp;
    double add_area = area/11;

    do{
        temp->super->area += add_area;
        temp->super->true_area += add_area;
        temp = temp->next;
    }while(temp != list);

}

// draw all windows
void draw_all_windows(Room * list)
{
    Room * temp = list;
    char name[20];
    do{
        strcpy(name, temp->super->name);
        if(strcmp(name, "Hall") && strcmp(name, "Closet"))
        {
            strcpy(name, temp->super->name);
            if(!strcmp(name, "WC") || !strcmp(name, "Lavanderia")) 
                temp->super->put_windows(temp->super, 1, 1, 0.6, r2);
            else
                temp->super->put_windows(temp->super, -1, 1, 1.5, r2);
        }
        temp = temp->next;
    }while(temp != list);
}

// draw all doors

// draw furnitures

void algorithm(Room * list)
{
    int rooms_qnt;
    double area, add_area;
    draw_room_list(list);
    area = calc_rest_area(list);
    if(area > 1e-8)
    {
        rooms_qnt = find_rooms_below_max(list);
        printf("rooms qnt: %d\n", rooms_qnt);
        if(rooms_qnt > 0)
        {
            add_area = area/rooms_qnt;
            propagate_new_area(list, add_area);
        }
        else // rooms_qnt == 0 -> nao eh possivel aumentar os quartos
        {
            if(!try_add_new_room(list, area)) propagate_without_limit(list, area);
        }
    }
    else if (area < 0)
    {
        rooms_qnt = find_rooms_above_min(list);
        printf("rooms qnt: %d\n", rooms_qnt);
        if(rooms_qnt > 0)
        {
            add_area = area/rooms_qnt;
            propagate_new_area(list, add_area);
        }
    }else
    {
        printf("to aqui");
        draw_all_windows(list);
    }

}

// Draw a circle
void drawCircle(double raio, double centroX, double centroY, int bg_angle, int end_angle)
{
    glPushMatrix();
    glTranslatef(centroX, centroY, 0.0);

    glBegin(GL_LINE_STRIP);
    for (int i = bg_angle; i < end_angle; i++)
    {
        double degInRad = i * M_PI / 180;
        glVertex2f(cos(degInRad) * raio, sin(degInRad) * raio);
    }
    if(end_angle == 360) glVertex2f(cos(0) * raio, sin(0) * raio);
    glEnd();

    glPopMatrix();
}

void drawGround(double w, double h)
{
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(w/2, h/2);
    glVertex2f(w/2, -h/2);
    glVertex2f(-w/2, -h/2);
    glVertex2f(-w/2, h/2);
    glEnd();
}

void RenderScene(void)
{
    // clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT);
    // Enable Zoom
    enable_zoom();

    // draw the ground
    drawGround(width, height);
    drawGround(width + 0.3, height + 0.3);

    // Purple
    glColor3f(1.0f, 0.0f, 0.4f);

    drawCircle(r2, 0, 0, 0, 360);
    drawCircle(r2 - WALL, 0, 0, 0, 360);
    //drawRoom(width - wall, 0, 0, 90, 90 + 30);

    glColor3f(1.0f, 0.0f, 0.4f);
    drawCircle(r1, 0, 0, 0, 360);
    drawCircle(r1 - WALL, 0, 0, 0, 360);

    algorithm(list_rooms);

    // Draw Compass
    drawCompassRose(5, -6, north_angle);

    draw_toilet(0, 0);

    // draw plants
    double s = 0.3;
    glColor3f(0.2, 0.5, 0.1);
    for(double i = 0; i <= 2*M_PI; i += M_PI/12)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2d(0, -1);
        glVertex2d(0 + s*cos(i), -1 + s*sin(i));
        glEnd();
        if(s == 0.3) s = 0.23;
        else s = 0.3;
    }

    glColor3f(1, 1, 1);
    drawCircle(0.2, 0, -1, 0, 360);
    drawCircle(0.15, 0, -1, 0, 360);

    glColor3f(0.1, 0.6, 0.3);
    glPushMatrix();
    for(double i = 0; i <= 360; i += 360.0/8)
    {
        glTranslatef(1, -1, 0);
        glRotatef(i, 0, 0, 1);
        glTranslatef(-1, 1, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(1, -1);
        glVertex2d(1 + 0.05, -1 + 0.05);
        glVertex2d(1 + 0.3, -1);
        glVertex2d(1 + 0.05, -1 - 0.05);
        glEnd();
    }
    glPopMatrix();

    drawCircle(0.3, 2, 1, 0, 360);

    glutSwapBuffers();
}

// press key function
void keyboard(unsigned char key, int x, int y)
{
    if(key == 'q')
    {
        exit(0);
    }
    if(key == '=')
    {
        zoom_in(scale);
    }
    if(key == '-')
    {
        zoom_out(scale);
    }

    glutPostRedisplay();
}

// Set up the rendering state
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void reshape(int w, int h)
{
    //GLdouble aspectRatio;
    double aspectRatio = (double)w / h;
    double sx = aspectRatio > 1.0f ? aspectRatio : 1.0f;
    double sy = aspectRatio > 1.0f ? 1.0f : 1.0f/aspectRatio;
    // set view port
    // view port will be reshape with the windows
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-sx * 16, sx * 16, -sy * 16, sy * 16);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    // Get the house params
    printf("###### BLUEPRINT PROJECT ######\n\n");

    printf("Digite os valores de largura e altura do terreno: ");
    scanf("%lf %lf", &width, &height);

    printf("\nDigite o angulo correspondente ao norte: ");
    scanf("%lf", &north_angle);

    // House limit area == 120m2
    r2 = (height - 0.2)/2;
    if(r2 < 6.2) return printf("Não é possível gerar uma casa circular com essas dimensões");
    
    
    wf = ((r2 - 0.15) - (r1 + 0.15))*0.15;

    // initial configs
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    // create windows
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Blueprint Project");

    // initialize list of room types
    setRoomTypes();

    initialize_required_list();
    printRooms(list_rooms);

    // register GLUT callbacks
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    // Render context
    SetupRC();

    // execute the loop
    glutMainLoop();
    return 0;
}
