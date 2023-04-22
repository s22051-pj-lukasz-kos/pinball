#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <memory>
#include <tuple>
#include <vector>
#include <iostream>
#include <array>


using vec2d = std::array<double, 2>;

vec2d operator+(vec2d a, vec2d b) {
    return {a[0]+b[0], a[1]+b[1]};
}

class player_c {
public:
    double angle;
    vec2d position;
};

bool handle_events(player_c &player) {
    SDL_Event e;
    auto *key_state = SDL_GetKeyboardState(nullptr);
    while( SDL_PollEvent( &e ) != 0 ) {
        switch (e.type) {
            case SDL_QUIT:
                return false;
        }
    }

    if (key_state[SDL_SCANCODE_UP]) {
        vec2d forward vec = angle_to_vector(player.angle):
                player.position = player.position +
    }
//    if (key_state[SDL_SCANCODE_DOWN]) rect.y += 8;
    if (key_state[SDL_SCANCODE_LEFT]) player.angle-=M_PI/10.0;
    if (key_state[SDL_SCANCODE_RIGHT]) player.angle+=M_PI/10.0;
    return true;
}

std::pair<std::shared_ptr<SDL_Window>, std::shared_ptr<SDL_Renderer>> create_context() {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(640, 480,SDL_WINDOW_SHOWN, &window, &renderer);

    std::shared_ptr<SDL_Window> w (window, [](SDL_Window *window) {
        SDL_DestroyWindow(window);
    });
    std::shared_ptr<SDL_Renderer> r (renderer, [](SDL_Renderer *renderer) {
        SDL_DestroyRenderer(renderer);
    });
    return {w, r};
}

std::shared_ptr<SDL_Texture> load_texture (std::shared_ptr<SDL_Renderer> renderer, std::string texture_name) {
    auto surface = SDL_LoadBMP(("assets/" + texture_name).c_str());
    if(!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        throw std::invalid_argument(SDL_GetError());
    }
    /*
     * Set transparent pixel (transparent color)
     */
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface -> format, 0x0ff, 0x0, 0x0ff));

    auto texture = SDL_CreateTextureFromSurface(renderer.get(), surface);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        throw std::invalid_argument(SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return std::shared_ptr<SDL_Texture>(texture, [](auto *p) { SDL_DestroyTexture(p); });
}

std::pair<int, int> get_texture_w_h(std::shared_ptr<SDL_Texture> texture) {
    int w, h;
    SDL_QueryTexture(texture.get(), nullptr, nullptr, &w, &h);
    return {w, h};
}

SDL_Rect get_texture_rect(std::shared_ptr<SDL_Texture> texture) {
    auto [w, h] = get_texture_w_h(texture);
    return {0, 0, w, h};
}


void play_the_game(std::shared_ptr<SDL_Renderer> renderer_p) {
    auto clouds = load_texture(renderer_p, "clouds.bmp");
    auto background = load_texture(renderer_p, "background.bmp");
    auto player_texture = load_texture(renderer_p, "player1.bmp");
    SDL_Rect player_rect = get_texture_rect(player_texture);
    player_c player = {0, {320.0, 200.0}};


    auto prev_ticks = SDL_GetTicks();
    int frame_dropped = 0;
    while (handle_events(player)) {
        if (!frame_dropped) {
//                SDL_SetRenderDrawColor(renderer_p.get(), 0, 0, 0, 255);
//                SDL_RenderClear(renderer_p.get());
            SDL_RenderCopy(renderer_p.get(), background.get(), nullptr, nullptr);
            {
                auto [w, h] = get_texture_w_h(clouds);
                SDL_Rect clouds_rect = { player_rect.x / 2 - 200, player_rect.y / 2 - 200, w, h};
                SDL_RenderCopy(renderer_p.get(), clouds.get(), nullptr, &clouds_rect);
            }
            auto rect = player_rect;
            rect.x = player.position[0] - rect.w/2;
            rect.y = player.position[1] - rect.h/2;

            SDL_RenderCopyEx(renderer_p.get(), player_texture.get(), nullptr, &rect, 180.0*player.angle/M_PI, nullptr, SDL_FLIP_NONE);
//            SDL_SetRenderDrawColor(renderer_p.get(), 255, 100, 50, 255);
//            SDL_RenderFillRect(renderer_p.get(), &player_rect);
            SDL_RenderPresent(renderer_p.get());
        }

        auto ticks = SDL_GetTicks();
        if ((ticks - prev_ticks) < 33) {
            SDL_Delay(33 - (ticks - prev_ticks));
            frame_dropped = 0;
        } else { frame_dropped = 1; }
        prev_ticks += 33;
    }
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING );
    auto [window_p, renderer_p] = create_context();
    play_the_game(renderer_p);
    SDL_Quit();
    return 0;
}
