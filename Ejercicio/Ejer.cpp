#include <cstdlib>
//Incluimos las funciones básicas de Allegro
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include "allegro5/allegro_primitives.h"
#include "Jugador.h"
#include "Enemigo.h"
using namespace std;
int main(int argc, char** argv) {
	const float FPS = 60;
	bool gameOver = false;
	//  Crea un puntero a un ALLEGRO_DISPLAY
	ALLEGRO_DISPLAY* ventana;
	ALLEGRO_DISPLAY *display = NULL;
	float widthPantalla = 800;
	float heightPantalla = 600;
	ALLEGRO_BITMAP  *bitmapGameOver = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer = NULL;

	Jugador *player = new Jugador();
	Enemigo *Enemy = new Enemigo(200,200,36,38,widthPantalla, heightPantalla);
	
	//  Inicia allegro5, esto es necesario para realizar cualquier
	//  función de allegro
	al_init();
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_init_primitives_addon();
	al_install_keyboard();
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	if (!al_init()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize allegro!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	if (!al_init_image_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}
	display = al_create_display(widthPantalla, heightPantalla);

	if (!display) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	bitmapGameOver = al_load_bitmap("../Sprite/gameOver.png");
	player->loadImage();
	Enemy->loadImage();
	if (!player->CheckLoadImage(display))
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}
	if (!Enemy->CheckLoadImage(display))
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}
	if (!bitmapGameOver)
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}
	//game loop
	al_start_timer(timer);
	while (!gameOver) {
		//  La siguiente función limpia el buffer, con un color determinado, 
		//  recibe como parámetro un ALLEGRO_COLOR. 
		//  La función al_map_rgb(0,0,0) recibe como tres enteres sin signo,
		//  cada uno determina la cantidad del color rojo, verde y azul
		//  respectivamente; esta función devuelve un ALLEGRO_COLOR con lo
		//  que encaja perfecto con el parámetro que recibe la función
		//  al_clear_to_color(...)
		al_clear_to_color(al_map_rgb(0, 0, 0));
		
		player->draw(player->getBitmap(), 0);
		Enemy->draw(Enemy->getBitmap(), 0);
		//  Intercambia los buffers, ahora la ventana mostrará tendrá fondo
		//  de color negro. Si minimiza la ventana y la vuelve restaurar, se
		//  dará cuenta que ahora la pantalla muestra lo que estuve detrás.
		//  Esto es porque el buffer ahora tiene lo que estaba detrás de la
		//  ventana. Luego veremos como redibujar la ventana cuando se realice
		//  un evento.
		Enemy->movimiento();
		al_flip_display();
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_UP:
			//ANDA
			if (player->getY() > 0)
			{
				player->setY(player->getY() - 2);
				player->setImage(1);
			}
			break;
		case ALLEGRO_KEY_DOWN:
			//ANDA
			if (player->getY() < heightPantalla - player->getH())
			{
				player->setY(player->getY() + 2);
				player->setImage(4);
			}
			break;
		case ALLEGRO_KEY_LEFT:
			//ANDA
			if (player->getX() > 0)
			{
				player->setX(player->getX() - 2);
				player->setImage(3);
			}
			break;
		case ALLEGRO_KEY_RIGHT:
			//ANDA
			if (player->getX() < widthPantalla - player->getH())
			{
				player->setX(player->getX() + 2);
				player->setImage(2);
			}
			break;
		}
		//coluciones
		if (player->colicionEnemigo(Enemy->getW(), Enemy->getH(), Enemy->getX(), Enemy->getY()))
		{
			gameOver = true;
		}
	}
	gameOver = false;
	while (!gameOver)
	{
		
		al_clear_to_color(al_map_rgb(0, 0, 0));
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		al_draw_bitmap(bitmapGameOver, 0, 0, 0);
		switch (ev.keyboard.keycode)
		{
			case ALLEGRO_KEY_ENTER:
				gameOver = true;
				cout << "Entre" << endl;
			break;
		}
		al_flip_display();
	}
	//  No olvidarnos de eliminar el contenido bajo el puntero de ventana,
	//  esto eliminará a la ventana de la memoria.
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	delete player;
	return 0;
}
