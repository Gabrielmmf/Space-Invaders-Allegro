INCLUDE_ALLEGRO=/usr/include/x86_64-linux-gnu
PKG_CONFIG= `pkg-config --libs allegro-5 allegro_primitives-5 allegro_acodec-5 allegro_audio-5 allegro_color-5 allegro_dialog-5 allegro_font-5 allegro_image-5 allegro_main-5 allegro_memfile-5 allegro_physfs-5 allegro_ttf-5 allegro_video-5`

all: invaders.out

invaders.out: invaders.o
	gcc -o invaders.out invaders.o -lm $(PKG_CONFIG)

invaders.o: invaders.c
	gcc -I $(INCLUDE_ALLEGRO) -c invaders.c		
	
 
clean:
	rm invaders.o
	rm invaders.out
