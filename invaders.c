#include <stdio.h>
#include <math.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <unistd.h> // funçao sleep

int screen_w = 960;
int screen_h = 540;

int grass_h = 60;

int nave_h = 50;
int nave_w = 100;
int vel_nave = 6;

int alien_h = 25;
int alien_w = 50;
int alien_vel_x = 4;
int alien_vel_y = 25;
int dist = 30;

int vel_tiro = 8;
int raio_tiro = 4;

int n_linhas = 4;
int n_colunas = 5;
int num_aliens = 20;

float fps = 60;

int fechar = 0;

int vitoria = 0;

int pont = 0;

int recorde = 0;

int recordeantigo=0;

int fase = 0;

int vitoriafinal = 0;

int alien_velx1 = 0;
int alien_vely1 = 0;

typedef struct Nave{
    float x;
    ALLEGRO_COLOR cor;
    float vel;
    int dir, esq;
} Nave;
typedef struct Alien{
    float x, y;
    float x_vel, y_vel;
    ALLEGRO_COLOR cor;
    int existe;
} Alien;
typedef struct Tiro{
    float x, y, y_vel;
    ALLEGRO_COLOR cor;
    int existe;
} Tiro;
void initNave(Nave *nave){
    nave->x = screen_w/2;
    nave->cor = al_map_rgb(0,0,255);
    nave->vel=vel_nave;
    nave->dir = 0;
    nave->esq = 0;
}
void initAlien(Alien *alien, int x, int y){
    alien-> x = x;
    alien-> y = y;
    alien->x_vel = alien_vel_x;
    alien->y_vel = alien_vel_y;
    alien->cor = al_map_rgb(rand()%256,rand()%256,rand()%256);
    alien->existe = 1;
}
void initAliens(Alien aliens[]){
    int x = 0;
    int y = alien_h;
    int k = 0;
    for(int i=0; i<num_aliens;i++)
    {
    if(k==n_colunas){
    k=0;
    y+=(alien_h+dist);
    }
    x = k*(alien_w+dist);
    initAlien(&aliens[i],x,y);
    k++;
    }
}
void initTiro(Tiro *tiro, float x, float y){
    if(tiro->existe!=1){
    tiro->x=x;
    tiro->y=y;
    tiro->y_vel=vel_tiro;
    tiro->cor = al_map_rgb(255,0,0);
    tiro->existe = 1;
    }
}
void draw_alien(Alien aliens[]){
    for(int i=0;i<num_aliens;i++){
    if(aliens[i].existe)
    al_draw_filled_rectangle(aliens[i].x, aliens[i].y,
                            aliens[i].x + alien_w, aliens[i].y+alien_h,
                            aliens[i].cor);
    }
}
void draw_tiro(Tiro tiro){
    if(tiro.existe)
    al_draw_filled_circle(tiro.x, tiro.y, raio_tiro, tiro.cor);
}

void velxaliens(Alien aliens[]){
    for(int i=0;i<num_aliens;i++)
        {
        alien_velx1 = aliens[i].x_vel;
        }
}

void para_alien(Alien aliens[]){
     for(int i=0;i<num_aliens;i++)
        {
        aliens[i].y_vel=0;
        aliens[i].x_vel=0;
        }
}

void continua_alien(Alien aliens[]){
        for(int i=0;i<num_aliens;i++)
        {
        aliens[i].y_vel=alien_vel_y;
        aliens[i].x_vel=alien_velx1;
        }
    }


void update_alien(Alien aliens[]){
    for(int j=0;j<num_aliens;j++)
    if(aliens[j].existe){
    {
    if(aliens[j].x +alien_w + aliens[j].x_vel > screen_w || aliens[j].x + aliens[j].x_vel <0){
        for(int i=0;i<num_aliens;i++)
        {
        aliens[i].y += aliens[i].y_vel;
        aliens[i].x_vel*=-1;
        }
    }
    aliens[j].x += aliens[j].x_vel;
    }
    }
}
int numaliens(Alien aliens[]){
    int soma = 0;
    for(int i=0;i<num_aliens;i++)
    soma+=aliens[i].existe;
    return soma;
}
int colisao_alien_solo(Alien aliens[]){
    for(int i=0;i<num_aliens;i++)
    {
    if(aliens[i].y+alien_h > screen_h - nave_h && aliens[i].existe)
    return 1;
    }
    return 0;
}
int colisao_alien_nave(Alien aliens[], Nave nave){
    for(int i = 0; i<num_aliens; i++)
    {
    if(aliens[i].y+alien_h > screen_h - nave_h/2 - nave_h - 10 && aliens[i].x+alien_w>=nave.x-(nave_w/2)+10 && aliens[i].x<=nave.x+(nave_w/2)-10&& aliens[i].existe)
    return 1;
    }
    return 0;
}
void colisao_alien_tiro(Alien aliens[], Tiro *tiro){
    for(int i = 0;i<num_aliens; i++)
    {
        if(aliens[i].existe && tiro->existe)
        if(aliens[i].y<=tiro->y+raio_tiro && aliens[i].y+alien_h>=tiro->y+raio_tiro && aliens[i].x<=tiro->x+raio_tiro && aliens[i].x+alien_w>=tiro->x-raio_tiro){
        aliens[i].existe=0;
        tiro-> existe = 0;
        pont++;
        }
    }
}
void draw_scenario(){
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_filled_rectangle(0, screen_h-nave_h, screen_w,
                             screen_h, al_map_rgb(0,255,0));
}
void draw_nave(Nave nave){
    float y_base = screen_h - nave_h/2;
    al_draw_filled_triangle(nave.x, y_base-nave_h, 
                            nave.x - nave_w/2, y_base,
                            nave.x + nave_w/2, y_base, 
                            nave.cor);
}
void update_nave(Nave *nave){
    if(nave->dir && nave->x + nave->vel<=screen_w){
        nave->x += nave->vel;
    }
    if(nave->esq && nave->x - nave->vel >=0){
        nave->x -= nave->vel;
    }
}
void update_tiro(Tiro *tiro){
    if(tiro->existe){
    if(tiro->y<=0)
    tiro->existe=0;
    tiro->y-=tiro->y_vel;
    }   
}
int main(){
    int playing = 1;
    vitoria = 0;

    if(fase==0){
    pont = 0;
    recorde = 0;
    recordeantigo=0;
    alien_h = 25;
    alien_w = 50;
    alien_vel_x = 4;
    alien_vel_y = 25;
    dist = 30;

    vel_tiro = 10;
    raio_tiro = 4;

    n_linhas = 5;
    n_colunas = 6;
    num_aliens = 30;
    }
    else if(fase==1)
    {
    alien_h = 25;
    alien_w = 50;
    alien_vel_x = 6;
    alien_vel_y = 25;
    dist = 30;

    vel_tiro = 10;
    raio_tiro = 4;

    n_linhas = 5;
    n_colunas = 6;
    num_aliens = 30;
    }
    else if(fase==2)
    {
    alien_h = 25;
    alien_w = 50;
    alien_vel_x = 5;
    alien_vel_y = 25;
    dist = 40;

    vel_tiro = 10;
    raio_tiro = 4;

    n_linhas = 5;
    n_colunas = 6;
    num_aliens = 30;       
    }
    else if(fase==3)
    {
    alien_h = 25;
    alien_w = 50;
    alien_vel_x = 4;
    alien_vel_y = 25;
    dist = 30;

    vel_tiro = 10;
    raio_tiro = 4;

    n_linhas = 5;
    n_colunas = 8;
    num_aliens = 40 ;   
    }
    else if(fase==4)
    {
    alien_h = 25;
    alien_w = 50;
    alien_vel_x = 5;
    alien_vel_y = 20;
    dist = 30;

    vel_tiro = 10;
    raio_tiro = 4;

    n_linhas = 5;
    n_colunas = 8;
    num_aliens = 40;    
    }
    else{
        playing=0;
        vitoriafinal = 1;
    }
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *fonte = NULL;
    ALLEGRO_FONT *fonte2 = NULL;
    ALLEGRO_FONT *fonte3 = NULL;
    ALLEGRO_FONT *fonte4 = NULL;
    FILE *arq;

    if(!al_init()){
        printf("failed to initialize allegro!\n");
        return -1;
    }
    timer = al_create_timer(1.0/fps);
    if(!timer){
        printf("failed to create timer!\n");
        return - 1;
    }
    if(!al_install_keyboard()){
        printf("failed to install keyboard\n");
        return -1;
    }
    if(!al_install_mouse()){
        printf("failed to install mouse\n");
        return -1;
    }
    display = al_create_display(screen_w, screen_h);
    if(!display){
        printf("failed to create display\n");
        al_destroy_timer(timer);
        return -1;
    }
    event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
	al_init_font_addon();
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}
    fonte = al_load_font("arial.ttf", 48, 0);
    if (!fonte){
        al_destroy_display(display);
        fprintf(stderr, "Falha ao carregar fonte");
        return -1;
    }

    fonte2 = al_load_font("arial.ttf", 24, 0);
    if (!fonte2){
        al_destroy_display(display);
        fprintf(stderr, "Falha ao carregar fonte");
        return -1;
    }
    fonte3 = al_load_font("arial.ttf", 96, 0);
    if (!fonte3){
        al_destroy_display(display);
        fprintf(stderr, "Falha ao carregar fonte");
        return -1;
    }

    fonte4 = al_load_font("arial.ttf", 36, 0);
    if (!fonte4){
        al_destroy_display(display);
        fprintf(stderr, "Falha ao carregar fonte");
        return -1;
    }

    //registra eventos de tela
    al_register_event_source(event_queue,al_get_display_event_source(display));
    //registra eventos de teclado
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    //registra eventos de mouse
    al_register_event_source(event_queue, al_get_mouse_event_source());
    //registra eventos de tempo
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    Tiro tiro;
    
    Nave nave;
    initNave(&nave);

    Alien aliens[num_aliens];
    initAliens(aliens);
    
    if(fase==0){
    arq = fopen("recorde.txt", "r");
            fscanf(arq, "%d", &recordeantigo);
            fclose(arq);
    }

    //temporizador
    al_start_timer(timer);
    while(playing){

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type==ALLEGRO_EVENT_TIMER){


            draw_scenario();

            update_nave(&nave);
            
            update_alien(aliens);

            update_tiro(&tiro);

            draw_nave(nave);
            
            draw_alien(aliens);

            draw_tiro(tiro);

            colisao_alien_tiro(aliens, &tiro);

            

            al_draw_textf(fonte2, al_map_rgb(0, 0, 0), 10 , screen_h-30 , ALLEGRO_ALIGN_LEFT, "ALIENS DESTRUÍDOS: %d", pont);

            al_draw_textf(fonte2, al_map_rgb(255, 255, 255), 60 , 20 , ALLEGRO_ALIGN_CENTRE, "FASE %d", fase+1);
          
            playing = (!colisao_alien_solo(aliens))*(!colisao_alien_nave(aliens, nave));
            
            arq = fopen("recorde.txt", "r");
            fscanf(arq, "%d", &recorde);
            fclose(arq);
            if(pont>=recorde){
            recorde = pont;
            arq = fopen("recorde.txt", "w");
            fprintf(arq,"%d",pont);
            fclose(arq);
            }

            al_draw_textf(fonte2, al_map_rgb(0, 0, 0), screen_w-15 , screen_h-30 , ALLEGRO_ALIGN_RIGHT, "RECORDE: %d", recorde);
            
            al_flip_display();
            
            if(al_get_timer_count(timer)%(int)fps==0){
                printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/fps));
                printf("\nRecorde: %d", recorde);
            }
            if(numaliens(aliens)==0){
                if(fase==4){
                    vitoriafinal=1;
                }
                vitoria=1;
                playing=0;
            }
        }

        else if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            fechar = 1;
            playing=0;
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
            printf("\ncodigo tecla: %d", ev.keyboard.keycode);
                switch(ev.keyboard.keycode){

                case ALLEGRO_KEY_A:
                    nave.esq = 1;
                break;

                case ALLEGRO_KEY_D:
                    nave.dir = 1;
                break;
                
                case ALLEGRO_KEY_SPACE:
                    initTiro(&tiro, nave.x, screen_h - grass_h/2 - nave_h);
                break;
                
                case ALLEGRO_KEY_P:
                    velxaliens(aliens);
                    para_alien(aliens);
                break;
            }
        }
        
        else if(ev.type==ALLEGRO_EVENT_KEY_UP){
            //imprime qual tecla foi
            printf("\ncodigo tecla: %d", ev.keyboard.keycode);

            switch(ev.keyboard.keycode){

                case ALLEGRO_KEY_A:
                    nave.esq = 0;
                break;

                case ALLEGRO_KEY_D:
                    nave.dir = 0;
                break;
                case ALLEGRO_KEY_P:
                    continua_alien(aliens);
                break;

            }
        }
    }
    if(fechar){
        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
    }
    else if(vitoriafinal){
        al_clear_to_color(al_map_rgb(0,0,255));
        al_flip_display();
        al_rest(0.5);
        al_clear_to_color(al_map_rgb(255,255,255));
        al_flip_display();
        al_rest(0.4);
        al_clear_to_color(al_map_rgb(0,0,255));
        al_flip_display();
        al_rest(0.3);
        al_clear_to_color(al_map_rgb(255,255,255));
        al_flip_display();
        al_rest(0.2);
        al_clear_to_color(al_map_rgb(0,0,255));
        al_flip_display();
        al_rest(0.1);
        al_clear_to_color(al_map_rgb(255,255,255));
        al_flip_display();
        al_rest(0.5);
        al_draw_textf(fonte4, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)-24 , ALLEGRO_ALIGN_CENTRE, "TODOS OS ALIENÍGENAS FORAM DESTRUÍDOS!!!");
        al_flip_display();
        al_rest(5.0);
    }
    else if(!vitoria){
        al_clear_to_color(al_map_rgb(255,0,0));
        al_flip_display();
        al_rest(0.5);
        al_clear_to_color(al_map_rgb(255,255,255));
        al_flip_display();
        al_rest(0.5);
        al_draw_textf(fonte3, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)-96 , ALLEGRO_ALIGN_CENTRE, "GAME OVER");
        if(pont>recordeantigo)
        {
        al_draw_textf(fonte4, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)+48 , ALLEGRO_ALIGN_CENTRE, "Você fez %d pontos e bateu o recorde de %d pontos!!!", pont, recordeantigo);
        al_draw_textf(fonte4, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)+96 , ALLEGRO_ALIGN_CENTRE, "Pressione qualquer tecla para jogar novamente");
        }
        else
        {
        al_draw_textf(fonte4, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)+48 , ALLEGRO_ALIGN_CENTRE, "Sua pontuação: %d   Recorde: %d", pont, recordeantigo);
        al_draw_textf(fonte4, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)+96 , ALLEGRO_ALIGN_CENTRE, "Pressione qualquer tecla para jogar novamente");
        }
        al_flip_display();
        sleep(1);
        al_rest(1.0);
        ALLEGRO_EVENT ev;
        while(true){
        al_wait_for_event(event_queue, &ev);
        if(ev.type==ALLEGRO_EVENT_TIMER){
            al_rest(1.0/fps);
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            fase=0;
            main();
            break;
        }
        else if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            break;
        }
        }
    }
    else if(vitoria){
        al_clear_to_color(al_map_rgb(0,0,255));
        al_flip_display();
        al_rest(0.5);
        al_clear_to_color(al_map_rgb(255,255,255));
        al_flip_display();
        al_rest(0.5);
        al_draw_textf(fonte3, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)-96 , ALLEGRO_ALIGN_CENTRE, "VENCEU!!!!!!!!");
        al_draw_textf(fonte4, al_map_rgb(0, 0, 0), screen_w/2 , (screen_h/2)+48 , ALLEGRO_ALIGN_CENTRE, "Pressione qualquer tecla para jogar a próxima fase");
        al_flip_display();
        sleep(1);
        al_rest(1.0);
        ALLEGRO_EVENT ev;
        while(true){
        al_wait_for_event(event_queue, &ev);
        if(ev.type==ALLEGRO_EVENT_TIMER){
            al_rest(1.0/fps);
        }
        else if(ev.type==ALLEGRO_EVENT_KEY_DOWN){
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            fase++;
            main();
            break;
        }
        else if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE){
            al_destroy_display(display);
            al_destroy_event_queue(event_queue);
            break;
        }
        }
    }
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}
