#include "TestGame.h"
#include "SDL_net.h"

//#include <stdio.h>
//#include <stdlib.h>

#include <string.h>
#include "SDL.h"
//#include "SDL_net.h"

// '0' -> 0, '1' -> 1
int charToInt(char c) {
    return c - '0';
}

int YY = 0;

static int run(void* ptr)
{
    TCPsocket socket = (TCPsocket) ptr;
    // message buffer
    char message[1024];
    int received;
    do {
        received = SDLNet_TCP_Recv(socket, message, 1024);
        int y = charToInt(message[0]) * 100 + charToInt(message[1]) * 10 + charToInt(message[2]);

        YY = y;

        std::cout << "Got: " << y << std::endl;
    } while (received > 0);

    return 0;
}

int main(int argc, char** argv)
{
    IPaddress ip;
    TCPsocket sock;
    
    int len = 200;
    Uint16 port;

    /* check our commandline */
/*    if (argc < 3)
    {
        printf("%s host port\n", argv[0]);
        exit(0);
    }*/

    /* initialize SDL */
    if (SDL_Init(0) == -1)
    {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    /* initialize SDL_net */
    if (SDLNet_Init() == -1)
    {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    /* get the port from the commandline */
    //port = (Uint16)strtol(argv[2], NULL, 0);


    port = 55555;

    /* Resolve the argument into an IPaddress type */
    if (SDLNet_ResolveHost(&ip, "localhost", port) == -1)
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(3);
    }

    /* open the server socket */
    sock = SDLNet_TCP_Open(&ip);
    
    if (!sock)
    {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(4);
    }

    // send data
    //while (len > 2) {
    //    /* read the buffer from stdin */
    //    printf("Enter Message, or Q to make the server quit:\n");
    //    fgets(message, 1024, stdin);
    //    len = strlen(message);

    //    /* strip the newline */
    //    message[len - 1] = '\0';

    //    if (len)
    //    {
    //        int result;

    //        /* print out the message */
    //        printf("Sending: %.*s\n", len, message);

    //        result = SDLNet_TCP_Send(sock, message, len); /* add 1 for the NULL */
    //        if (result < len)
    //            printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    //    }

    //}

    SDL_Thread* thread = SDL_CreateThread(run, "ConnectionThread", (void*)sock);

    // WINDOW

    SDL_Window * window = SDL_CreateWindow("Multiplayer Pong Client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (nullptr == window)
        throw EngineException("Failed to create window", SDL_GetError());

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    bool running = true;

    SDL_Rect rect = { 0, 0, 20, 60 };
    SDL_Event event;

    while (running) {

        while (SDL_PollEvent(&event)) {
            if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
            }

            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        rect.y = YY;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(17);
    }



















    



    SDLNet_TCP_Close(sock);

    /* shutdown SDL_net */
    SDLNet_Quit();

    /* shutdown SDL */
    SDL_Quit();

    return(0);
}

/*int main(int argc, char * args[]) {



	try {
		TestGame game;
		game.runMainLoop();
	} catch (EngineException & e) {
		std::cout << e.what() << std::endl;
		getchar();
	}

	return 0;
}*/