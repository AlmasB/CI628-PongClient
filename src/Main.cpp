#include <iostream>
#include <vector>
#include <string>
// string.h?

#include "SDL.h"
#include "SDL_net.h"

using namespace std;

// '0' -> 0, '1' -> 1
int charToInt(char c) {
    return c - '0';
}





struct GameData {
    int player1Y;
    int player2Y;
    int ballX;
    int ballY;
} game_data;

const char* IP_NAME = "localhost";
const Uint16 PORT = 55555;







static int on_receive(void* socket_ptr) {
    TCPsocket socket = (TCPsocket)socket_ptr;

    const int message_length = 1024;

    char message[message_length];
    int received;

    do {
        received = SDLNet_TCP_Recv(socket, message, message_length);
        message[received] = '\0';

        char* pch = strtok(message, ",");

        string cmd(pch);

        cout << "Cmd is: " << cmd << endl;

        vector<string> args;

        while (pch != NULL) {
            //printf("%s\n", pch);
            pch = strtok(NULL, ",");

            if (pch != NULL) {
                args.push_back(string(pch));
            }
        }

        if (cmd == "GAME_DATA") {
            for (auto arg : args) {
                cout << arg << endl;
            }
        }

        if (cmd == "exit") {
            break;
        }

    } while (received > 0);

    return 0;
}

void loop(SDL_Renderer* renderer) {
    bool running = true;

    SDL_Rect player1 = { 0, 0, 20, 60 };
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

        // update_frame
        player1.y = game_data.player1Y;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &player1);

        SDL_RenderPresent(renderer);

        SDL_Delay(17);
    }
}

int run_game() {
    SDL_Window* window = SDL_CreateWindow(
        "Multiplayer Pong Client",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    if (nullptr == window) {
        std::cout << "Failed to create window" << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // TODO: check renderer

    loop(renderer);

    return 0;
}



int main(int argc, char** argv) {

    // Initialize SDL
    if (SDL_Init(0) == -1) {
        printf("SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }

    // Initialize SDL_net
    if (SDLNet_Init() == -1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

    IPaddress ip;

    // Resolve host (ip name + port) into an IPaddress type
    if (SDLNet_ResolveHost(&ip, IP_NAME, PORT) == -1) {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(3);
    }

    // Open the connection to the server
    TCPsocket socket = SDLNet_TCP_Open(&ip);

    if (!socket) {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(4);
    }

    SDL_CreateThread(on_receive, "ConnectionThread", (void*)socket);

    run_game();

    // Close connection to the server
    SDLNet_TCP_Close(socket);

    // Shutdown SDL_net
    SDLNet_Quit();

    // Shutdown SDL
    SDL_Quit();

    return 0;
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