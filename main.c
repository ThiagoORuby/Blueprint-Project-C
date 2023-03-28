#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include "src/utils.h"
#include "src/SectorRoom.h"
#include "src/furniture_components.h"
#include "src/vector.h"
#include "src/Rooms.h"

// extern params
extern Room room_types[8];

// Global params
double width, height;
double r1, r2, r3, wf;
double north_angle;
Room * list_rooms = NULL;
int inversed = 0;
int required_size = 11;
int required_rooms[] = {0, 1, 2, 3, 3, 3, 2, 1, 6, 4, 5};
int aditional_rooms[] = {7, 3, 2};
int lateral_doors[] = {0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0};
int frontal_doors[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0};

// algorithm params
int epochs = 0;
int steps = 0;
int interval = 100;
int completed = 0;

void initialize_required_list()
{
    for(int i = 0; i < 11; i++)
    {
        Room aux = room_types[required_rooms[i]];
        double area = random_int(aux.area_interval);
        pushRoom(&list_rooms, aux, area, wf);
    }
}

void draw_room_list(Room * list)
{
    double init_angle = -10, new_angle;
    Room * temp = list;

    temp->super->bg_angle = init_angle;
    temp->super->draw(temp->super, r1 - WALL, r2, 1, 1);
    new_angle = temp->super->bg_angle + temp->super->delta;
    temp = temp->next;
    int i = 0;
    while(temp != list)
    {
        if(i == steps) break;
        temp->super->bg_angle = new_angle;
        temp->super->draw(temp->super, r1 - WALL, r2, 1, 1);
        new_angle = temp->super->bg_angle + temp->super->delta;
        temp = temp->next;
        i++;
    }
}

void draw_room_list_inv(Room * list)
{
    double init_angle = 10, new_angle;
    Room * temp = list;

    temp->super->bg_angle = init_angle;
    temp->super->draw(temp->super, r1 - WALL, r2, 1, -1);
    new_angle = temp->super->bg_angle  + temp->super->delta;
    temp = temp->next;
    int i = 0;
    while(temp != list)
    {
        if(i == steps) break;
        temp->super->bg_angle = new_angle;
        temp->super->draw(temp->super, r1 - WALL, r2, 1, -1);
        new_angle = temp->super->bg_angle  + temp->super->delta;
        temp = temp->next;
        i++;
    }
}

double calc_rest_area(Room * list)
{
    double dangle, area;
    Room * prev = list->prev;

    if(list->super->bg_angle == -1) return 0;

    dangle = (list->super->bg_angle + 360) - (prev->super->bg_angle + prev->super->delta);

    area = (dangle * M_PI * (r2 - (r1 - WALL)) * (r2 + (r1 - WALL)))/360;


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
    printf("Add Area: %.2lf\n", add_area);
    do{
        if(add_area > 0)
        {
            if(temp->super->area < temp->area_interval[1])
            {
                limit = temp->area_interval[1] - temp->super->area;
                true_add = (add_area > limit) ? limit : add_area;
                add_area += (add_area > limit) ? add_area - limit : 0;
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

int try_add_new_room(Room * list, double * area)
{
    for(int i = 0; i < 3; i++)
    {
        Room room = room_types[aditional_rooms[i]];
        if(*area >= room.area_interval[1] + wf)
        {
            pushRoom(&list, room, room.area_interval[1], wf);
            lateral_doors[required_size] = 0;
            frontal_doors[required_size] = 1;
            required_size++;
            *area -= (room.area_interval[1] + wf);
        }
        else{
            return 0;
        }
    }
    return 1;
}

void propagate_without_limit(Room * list, double area)
{
    Room * temp = list;
    double add_area = area/required_size;

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
void draw_all_doors(Room * list)
{
    Room * temp = list;
    double size[] = {-1, -1, -1};
    int index1 = 0, index2 = (inversed) ? required_size - 1 : 0;
    // hall first
    int mapping[3][2] = {{0, 0}, {0, 0}, {0, 0}};
    mapping[1][0] = frontal_doors[index1];
    mapping[2][0] = lateral_doors[index2];
    temp->super->put_doors(temp->super, mapping, size, r1, r2);
    temp = temp->next;
    index1 = (inversed) ? required_size - 1 : 1;
    index2 += (inversed) ? -1 : 1;
    while(temp != list)
    {
        int mapping2[3][2] = {{0, 0}, {0, 0}, {0, 0}};
        mapping2[1][0] = frontal_doors[index1];
        mapping2[2][0] = lateral_doors[index2];
        temp->super->put_doors(temp->super, mapping2, size, r1, r2);
        temp = temp->next;
        index1 += (inversed) ? -1 : 1;
        index2 += (inversed) ? -1 : 1;
    }
}

//TODO: draw all furnitures

// algorithm
void algorithm(Room * list)
{
    int rooms_qnt, ret;
    double area, add_area;
    char info[30];
    area = calc_rest_area(list);
    printf("Add Area: %.2lf\n", area);
    if(area > 1e-8)
    {
        rooms_qnt = find_rooms_below_max(list);
        printf("rooms qnt: %d\n", rooms_qnt);
        if(rooms_qnt > 0)
        {
            add_area = area/rooms_qnt;
            propagate_new_area(list, add_area);
            return;
        }
        else // rooms_qnt == 0 -> nao eh possivel aumentar os quartos
        {
            ret = try_add_new_room(list, &area); // tenta adicionar novos cômodos
            if(!ret || area > 1e-8) propagate_without_limit(list, area); // força um aumento
            return;
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
        return;
    }else
    {
        completed = TRUE;
    }

}

void update(int value) {

    glutPostRedisplay();
    if(steps < required_size)
    {
        steps++;
        glutTimerFunc(interval, update, 0);
    }
    else
    {
        //printf("Parei de animar!\n");
        glutTimerFunc(0, update, 0);
    }
}

void draw_info_params()
{
    // draw info params
    drawText("INFO PARAMS", 100, 520);
    char info_epochs[20], info_area[30], status[20];
    sprintf(info_epochs, "Epochs: %d", epochs);
    drawText(info_epochs, 100, 500);
    sprintf(info_area, "Remaining Area: %.2lfm2", calc_rest_area(list_rooms));
    drawText(info_area, 100, 480);
    sprintf(status, "Status: %s", (completed) ? "Completed" : "In progress");
    drawText(status, 100, 460);

    // Draw info keys
    drawText("N : Next epoch", 100, 200);
    drawText("+ : Zoom in", 100, 180);
    drawText("- : Zoom out", 100, 160);
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

    drawHouseArea(r2);
    drawMainRoom(r1);
    
    draw_room_list(list_rooms);
    
    if(completed && steps >= required_size)
    {
        draw_all_windows(list_rooms);
        draw_all_doors(list_rooms);
    }

    // Draw Compass
    drawCompassRose(5, -6, north_angle);
    // draw info params
    draw_info_params();

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
    if(key == 'n' || key == 'N')
    {
        if(!completed)
        {
            algorithm(list_rooms); // update room areas
            epochs++;
            steps = 0; // restart draw
            printf("required_size = %d\n", required_size);
        }
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
    srand(time(NULL)); // initialize random seed
    // Get the house params
    printf("###### BLUEPRINT PROJECT ######\n\n");

    printf("Digite os valores de largura e altura do terreno: ");
    scanf("%lf %lf", &width, &height);

    printf("\nDigite o angulo correspondente ao norte: ");
    scanf("%lf", &north_angle);

    // House limit area == 120m2
    r2 = (height - 3.0)/2;
    r3 = (width - 3.0)/2;
    if((r2 < 6.2 || r2 > 8.2) || (r3 < 6.2)) 
        return printf("Não é possível gerar uma casa circular com essas dimensões!\n");

    r1 = random_double(3.1, r2/2);
    printf("r1 = %.2lf\n", r1);
    
    wf = ((r2 - 0.15) - (r1 + 0.15))*0.15; // wall factor

    // initial configs
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    int screen_width = glutGet(GLUT_SCREEN_WIDTH);
    int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

    // create windows
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(screen_width, screen_height);
    glutCreateWindow("Blueprint Project");

    // Posiciona a janela no canto superior esquerdo da tela
    int border_width = glutGet(GLUT_WINDOW_BORDER_WIDTH);
    glutPositionWindow(0 - border_width, 0);

    // initialize list of room types
    setRoomTypes();

    initialize_required_list();
    printRooms(list_rooms);
    // se o angulo está nessa faixa, comodos serao invertidos
    if(!(north_angle >= 90 && north_angle <= 270))
    {
        reverseList(&list_rooms);
        printRooms(list_rooms);
        inversed = 1;
    }

    // register GLUT callbacks
    glutDisplayFunc(RenderScene);

    glutTimerFunc(interval, update, 0);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    // Render context
    SetupRC();

    // execute the loop
    glutMainLoop();
    return 0;
}
