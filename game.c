#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "cJSON.h"

#define WINDOW_WIDTH 860
#define WINDOW_HEIGHT 480
#define HUD_WIDTH 219
#define HUD_HEIGHT 419
#define CHAT_BAR_HEIGHT 61
#define GAME_VIEW_WIDTH 641
#define GAME_VIEW_HEIGHT 419
#define BORDER_WIDTH 2
#define MAX_INPUT_LEN 32
#define HUD_BTN_SIZE 50
#define HUD_PADDING 10
#define MAX_CHARACTERS 16
#define MAX_CHARACTER_NAME 32
#define MAX_RACE 16

typedef enum { TITLE_SCREEN, CHARACTER_SELECT_SCREEN, GAME_SCREEN } ScreenState;
typedef enum { NONE, USERNAME, PASSWORD } ActiveField;

typedef struct {
    char username[MAX_INPUT_LEN];
    char password[MAX_INPUT_LEN];
    ActiveField activeField;
} InputState;

typedef struct {
    int id;
    int user_id;
    char name[32];
    char race[16];
    char gender[16];
    int level;
    int experience;
    int base_strength;
    int base_endurance;
    int base_wisdom;
    int base_mystic;
    int base_agility;
    int base_accuracy;
    int base_intellect;
    int base_luck;
    int skill_sword,         skill_sword_exp;
    int skill_spear,         skill_spear_exp;
    int skill_axe,           skill_axe_exp;
    int skill_dagger,        skill_dagger_exp;
    int skill_bow,           skill_bow_exp;
    int skill_staff,         skill_staff_exp;
    int skill_shield,        skill_shield_exp;
    int skill_magick,        skill_magick_exp;
    int skill_rune,          skill_rune_exp;
    int skill_protection,    skill_protection_exp;
    int skill_healing,       skill_healing_exp;
    int skill_mining,        skill_mining_exp;
    int skill_blacksmith,    skill_blacksmith_exp;
    int skill_foraging,      skill_foraging_exp;
    int skill_alchemy,       skill_alchemy_exp;
    int skill_fishing,       skill_fishing_exp;
    int skill_cooking,       skill_cooking_exp;
    cJSON *raw;
} CharacterSelectEntry;

typedef struct {
    int id;
    int user_id;
    char name[32];
    char race[16];
    char gender[16];
    int level;
    int experience;
    int base_strength;
    int base_endurance;
    int base_wisdom;
    int base_mystic;
    int base_agility;
    int base_accuracy;
    int base_intellect;
    int base_luck;
    int skill_sword,         skill_sword_exp;
    int skill_spear,         skill_spear_exp;
    int skill_axe,           skill_axe_exp;
    int skill_dagger,        skill_dagger_exp;
    int skill_bow,           skill_bow_exp;
    int skill_staff,         skill_staff_exp;
    int skill_shield,        skill_shield_exp;
    int skill_magick,        skill_magick_exp;
    int skill_rune,          skill_rune_exp;
    int skill_protection,    skill_protection_exp;
    int skill_healing,       skill_healing_exp;
    int skill_mining,        skill_mining_exp;
    int skill_blacksmith,    skill_blacksmith_exp;
    int skill_foraging,      skill_foraging_exp;
    int skill_alchemy,       skill_alchemy_exp;
    int skill_fishing,       skill_fishing_exp;
    int skill_cooking,       skill_cooking_exp;
    int current_map;
    int current_map_x;
    int current_map_y;
} CharacterData;

typedef struct {
    char host[64];
    int port;
    char username[MAX_INPUT_LEN];
    char font[128];
    int show_fps;
    int music_volume;
    int sfx_volume;
} Config;

#define JINT(o, k) (cJSON_GetObjectItemCaseSensitive((o), (k)) ? cJSON_GetObjectItemCaseSensitive((o), (k))->valueint : 0)
#define JSTR(o, k, buf) { cJSON *tmp = cJSON_GetObjectItemCaseSensitive((o), (k)); strncpy((buf), (tmp && cJSON_IsString(tmp)) ? tmp->valuestring : "", sizeof(buf)-1); }

void fill_character_struct(CharacterSelectEntry *entry, cJSON *c) {
    entry->id = JINT(c, "id");
    entry->user_id = JINT(c, "user_id");
    JSTR(c, "name", entry->name);
    JSTR(c, "race", entry->race);
    JSTR(c, "sex", entry->gender);
    entry->level = JINT(c, "level");
    entry->experience = JINT(c, "experience");
    entry->base_strength = JINT(c, "base_strength");
    entry->base_endurance = JINT(c, "base_endurance");
    entry->base_wisdom = JINT(c, "base_wisdom");
    entry->base_mystic = JINT(c, "base_mystic");
    entry->base_agility = JINT(c, "base_agility");
    entry->base_accuracy = JINT(c, "base_accuracy");
    entry->base_intellect = JINT(c, "base_intellect");
    entry->base_luck = JINT(c, "base_luck");
    entry->skill_sword = JINT(c, "skill_sword");
    entry->skill_sword_exp = JINT(c, "skill_sword_exp");
    entry->skill_spear = JINT(c, "skill_spear");
    entry->skill_spear_exp = JINT(c, "skill_spear_exp");
    entry->skill_axe = JINT(c, "skill_axe");
    entry->skill_axe_exp = JINT(c, "skill_axe_exp");
    entry->skill_dagger = JINT(c, "skill_dagger");
    entry->skill_dagger_exp = JINT(c, "skill_dagger_exp");
    entry->skill_bow = JINT(c, "skill_bow");
    entry->skill_bow_exp = JINT(c, "skill_bow_exp");
    entry->skill_staff = JINT(c, "skill_staff");
    entry->skill_staff_exp = JINT(c, "skill_staff_exp");
    entry->skill_shield = JINT(c, "skill_shield");
    entry->skill_shield_exp = JINT(c, "skill_shield_exp");
    entry->skill_magick = JINT(c, "skill_magick");
    entry->skill_magick_exp = JINT(c, "skill_magick_exp");
    entry->skill_rune = JINT(c, "skill_rune");
    entry->skill_rune_exp = JINT(c, "skill_rune_exp");
    entry->skill_protection = JINT(c, "skill_protection");
    entry->skill_protection_exp = JINT(c, "skill_protection_exp");
    entry->skill_healing = JINT(c, "skill_healing");
    entry->skill_healing_exp = JINT(c, "skill_healing_exp");
    entry->skill_mining = JINT(c, "skill_mining");
    entry->skill_mining_exp = JINT(c, "skill_mining_exp");
    entry->skill_blacksmith = JINT(c, "skill_blacksmith");
    entry->skill_blacksmith_exp = JINT(c, "skill_blacksmith_exp");
    entry->skill_foraging = JINT(c, "skill_foraging");
    entry->skill_foraging_exp = JINT(c, "skill_foraging_exp");
    entry->skill_alchemy = JINT(c, "skill_alchemy");
    entry->skill_alchemy_exp = JINT(c, "skill_alchemy_exp");
    entry->skill_fishing = JINT(c, "skill_fishing");
    entry->skill_fishing_exp = JINT(c, "skill_fishing_exp");
    entry->skill_cooking = JINT(c, "skill_cooking");
    entry->skill_cooking_exp = JINT(c, "skill_cooking_exp");
}

static int load_config(Config* config) {
    strcpy(config->host, "localhost");
    config->port = 7777;
    strcpy(config->username, "");
    strcpy(config->font, "arial.ttf");
    config->show_fps = 0;
    config->music_volume = 100;
    config->sfx_volume = 100;
    FILE* f = fopen("game.conf", "r");
    if (!f) return -1;
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = 0;
        char* key = line;
        char* val = eq+1;
        val[strcspn(val, "\r\n")] = 0;
        if (!strcmp(key, "HOST")) strncpy(config->host, val, sizeof(config->host)-1);
        else if (!strcmp(key, "PORT")) config->port = atoi(val);
        else if (!strcmp(key, "USERNAME")) strncpy(config->username, val, sizeof(config->username)-1);
        else if (!strcmp(key, "FONT")) strncpy(config->font, val, sizeof(config->font)-1);
        else if (!strcmp(key, "SHOW_FPS")) config->show_fps = atoi(val);
        else if (!strcmp(key, "MUSIC_VOLUME")) config->music_volume = atoi(val);
        else if (!strcmp(key, "SFX_VOLUME")) config->sfx_volume = atoi(val);
    }
    fclose(f);
    return 0;
}

void sha512_hex(const char* input, char* output) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512((unsigned char*)input, strlen(input), hash);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
        sprintf(output + i*2, "%02x", hash[i]);
    output[SHA512_DIGEST_LENGTH*2] = 0;
}

int send_login_and_get_characters(const char* host, int port, const char* username, const char* password, char* outbuf, size_t outlen) {
    int sock;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char userpass[1024], hashed_pw[SHA512_DIGEST_LENGTH*2+1];

    sha512_hex(password, hashed_pw);
    snprintf(userpass, sizeof(userpass), "%s|%s\n", username, hashed_pw);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    server = gethostbyname(host);
    if (!server) { close(sock); return -2; }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) { close(sock); return -3; }

    send(sock, userpass, strlen(userpass), 0);
    int n = recv(sock, outbuf, outlen-1, 0);
    if (n <= 0) { close(sock); return -4; }
    outbuf[n] = 0;
    close(sock);
    return 0;
}

static bool point_in_circle(int px, int py, int cx, int cy, int r) {
    int dx = px - cx;
    int dy = py - cy;
    return dx * dx + dy * dy <= r * r;
}

void render_item_tooltip(SDL_Renderer *renderer, TTF_Font *font, int mouse_x, int mouse_y, cJSON *itemData, int imageId) {
    SDL_Color textColor = {255,255,255,255};
    char name[64] = {0}, type[32] = {0}, desc[128] = {0};
    int power = 0;

    cJSON *nameItem = cJSON_GetObjectItemCaseSensitive(itemData, "item_name");
    cJSON *typeItem = cJSON_GetObjectItemCaseSensitive(itemData, "item_type");
    cJSON *powerItem = cJSON_GetObjectItemCaseSensitive(itemData, "item_power");
    cJSON *descItem = cJSON_GetObjectItemCaseSensitive(itemData, "item_description");

    if (nameItem && cJSON_IsString(nameItem)) strncpy(name, nameItem->valuestring, sizeof(name)-1);
    if (typeItem && cJSON_IsString(typeItem)) strncpy(type, typeItem->valuestring, sizeof(type)-1);
    if (descItem && cJSON_IsString(descItem)) strncpy(desc, descItem->valuestring, sizeof(desc)-1);
    if (powerItem && cJSON_IsNumber(powerItem)) power = powerItem->valueint;

    char lines[4][128];
    int nlines = 0;
    snprintf(lines[nlines++], sizeof(lines[0]), "%s", name[0] ? name : "Unknown Item");
    snprintf(lines[nlines++], sizeof(lines[0]), "Type: %s", type[0] ? type : "Unknown");
    if (power) snprintf(lines[nlines++], sizeof(lines[0]), "Power: %d", power);
    if (desc[0]) snprintf(lines[nlines++], sizeof(lines[0]), "%s", desc);

    int width = 0, height = 0;
    int padding = 8, line_h = 0;
    for (int i=0; i<nlines; ++i) {
        int w=0, h=0;
        TTF_SizeText(font, lines[i], &w, &h);
        if (w > width) width = w;
        if (h > line_h) line_h = h;
    }
    width += padding * 2;
    height = line_h * nlines + padding * 2;

    int box_x = mouse_x + 20;
    int box_y = mouse_y + 10;
    if (box_x + width > WINDOW_WIDTH) box_x = WINDOW_WIDTH - width - 4;
    if (box_y + height > WINDOW_HEIGHT) box_y = WINDOW_HEIGHT - height - 4;

    SDL_Rect box = {box_x, box_y, width, height};
    SDL_SetRenderDrawColor(renderer, 32,32,56,220);
    SDL_RenderFillRect(renderer, &box);
    SDL_SetRenderDrawColor(renderer, 160,160,240,255);
    SDL_RenderDrawRect(renderer, &box);

    int tx = box_x + padding;
    int ty = box_y + padding;
    for (int i=0; i<nlines; ++i) {
        SDL_Surface *surf = TTF_RenderText_Solid(font, lines[i], textColor);
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect dst = {tx, ty, surf->w, surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &dst);
        ty += surf->h;
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}

typedef struct {
    const char *slot;
    int x, y;
} EquipSlotDisplay;

static const EquipSlotDisplay equipSlots[] = {
    {"armor",   28,  70},
    {"helmet", 100,  70},
    {"amulet", 177,  70},
    {"ring",    20, 140},
    {"gloves", 185, 140},
    {"onhand",  20, 187},
    {"offhand",185, 190},
    {"legs",    20, 237},
    {"boots",  185, 237}
};
#define N_EQUIP_SLOTS (sizeof(equipSlots)/sizeof(equipSlots[0]))

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    Config config = {0};
    load_config(&config);

    SDL_Window* window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_SetWindowResizable(window, SDL_FALSE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* bgSurface = IMG_Load("images/background.png");
    SDL_Texture* bgTexture = bgSurface ? SDL_CreateTextureFromSurface(renderer, bgSurface) : NULL;
    if (bgSurface) SDL_FreeSurface(bgSurface);

    SDL_Surface* hudSurface = IMG_Load("images/hud.png");
    SDL_Texture* hudTexture = hudSurface ? SDL_CreateTextureFromSurface(renderer, hudSurface) : NULL;
    if (hudSurface) SDL_FreeSurface(hudSurface);

    SDL_Surface* chatSurface = IMG_Load("images/chat.png");
    SDL_Texture* chatTexture = chatSurface ? SDL_CreateTextureFromSurface(renderer, chatSurface) : NULL;
    if (chatSurface) SDL_FreeSurface(chatSurface);

    SDL_Surface* equipmentSurface = IMG_Load("images/equipment.png");
    SDL_Texture* equipmentTexture = equipmentSurface ? SDL_CreateTextureFromSurface(renderer, equipmentSurface) : NULL;
    if (equipmentSurface) SDL_FreeSurface(equipmentSurface);

    SDL_Surface* bagSurface = IMG_Load("images/bag.png");
    SDL_Texture* bagTexture = bagSurface ? SDL_CreateTextureFromSurface(renderer, bagSurface) : NULL;
    if (bagSurface) SDL_FreeSurface(bagSurface);

    SDL_Surface* skillsSurface = IMG_Load("images/skills.png");
    SDL_Texture* skillsTexture = skillsSurface ? SDL_CreateTextureFromSurface(renderer, skillsSurface) : NULL;
    if (skillsSurface) SDL_FreeSurface(skillsSurface);

    TTF_Font* font = TTF_OpenFont(config.font[0] ? config.font : "arial.ttf", 14);
    TTF_Font* statFont = TTF_OpenFont(config.font[0] ? config.font : "arial.ttf", (int)(14 * 0.9));
    TTF_Font* skillFont = TTF_OpenFont(config.font[0] ? config.font : "arial.ttf", (int)(14 * 0.8));
    if (!font || !statFont || !skillFont) {
        printf("Failed to load %s: %s\n", config.font[0] ? config.font : "arial.ttf", TTF_GetError());
        font = TTF_OpenFont("arial.ttf", 14);
        statFont = TTF_OpenFont("arial.ttf", (int)(14 * 0.9));
        skillFont = TTF_OpenFont("arial.ttf", (int)(14 * 0.8));
        if (!font || !statFont || !skillFont) {
            printf("Failed to load arial.ttf: %s\n", TTF_GetError());
            return 1;
        }
    }
    SDL_Color textColor = {255, 255, 255, 255};

    InputState input = {"", "", NONE};
    if (config.username[0]) strncpy(input.username, config.username, MAX_INPUT_LEN-1);

    CharacterData character = {0};
    ScreenState currentScreen = TITLE_SCREEN;
    SDL_StartTextInput();
    bool quit = false;
    SDL_Event e;
    SDL_Rect loginButtonRect = {WINDOW_WIDTH / 4 + 50, WINDOW_HEIGHT / 4 + 160, 200, 40};

    bool showEquipment = false;
    bool showSkills = false;
    bool showBag = false;
    bool showSpells = false;
    bool showTeam = false;

    SDL_Rect hudButtonRects[4] = {
        {WINDOW_WIDTH - HUD_WIDTH + 42, 41, 30, 41},
        {WINDOW_WIDTH - HUD_WIDTH + 73, 41, 35, 41},
        {WINDOW_WIDTH - HUD_WIDTH + 109, 41, 35, 41},
        {WINDOW_WIDTH - HUD_WIDTH + 145, 41, 32, 41}
    };

    SDL_Rect bagLeftArrow = {WINDOW_WIDTH - HUD_WIDTH + 40, 242, 15, 26};
    SDL_Rect bagInventory = {WINDOW_WIDTH - HUD_WIDTH + 58, 242, 103, 26};
    SDL_Rect bagRightArrow = {WINDOW_WIDTH - HUD_WIDTH + 166, 242, 14, 26};

    SDL_Rect equipWindow = {5, 5, 243, 358};
    SDL_Rect skillsWindow = {5, 5, 380, 398};
    SDL_Rect bagWindow = {5, 5, 244, 239};
    SDL_Rect spellsWindow = {250, 5, 200, 300};

    const int bagBtnCenterX = WINDOW_WIDTH - HUD_WIDTH + 51;
    const int bagBtnCenterY = 302;
    const int bagBtnRadius = 20;
    const int logoutBtnCenterX = WINDOW_WIDTH - HUD_WIDTH + 175;
    const int logoutBtnCenterY = 370;
    const int logoutBtnRadius = 21;

    CharacterSelectEntry charEntries[MAX_CHARACTERS];
    int nCharacters = 0, selectedCharIndex = -1;
    cJSON* rootJson = NULL;

    Uint32 frameStart = 0;
    int frames = 0;
    float fps = 0.0f;
    Uint32 fpsLast = SDL_GetTicks();

    int mouse_x = 0, mouse_y = 0;

    while (!quit) {
        frameStart = SDL_GetTicks();
        SDL_GetMouseState(&mouse_x, &mouse_y);

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
            if (e.type == SDL_KEYDOWN && currentScreen == TITLE_SCREEN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && input.activeField != NONE) {
                    char* target = input.activeField == USERNAME ? input.username : input.password;
                    int len = strlen(target);
                    if (len > 0) target[len - 1] = '\0';
                }
            } else if (e.type == SDL_TEXTINPUT && input.activeField != NONE) {
                char* target = input.activeField == USERNAME ? input.username : input.password;
                int len = strlen(target);
                if (len < MAX_INPUT_LEN - 1) strncat(target, e.text.text, MAX_INPUT_LEN - len - 1);
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (currentScreen == TITLE_SCREEN) {
                    SDL_Rect usernameRect = {WINDOW_WIDTH / 4 + 50, WINDOW_HEIGHT / 4 + 50, 200, 40};
                    SDL_Rect passwordRect = {WINDOW_WIDTH / 4 + 50, WINDOW_HEIGHT / 4 + 100, 200, 40};
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &usernameRect)) input.activeField = USERNAME;
                    else if (SDL_PointInRect(&(SDL_Point){x, y}, &passwordRect)) input.activeField = PASSWORD;
                    else if (SDL_PointInRect(&(SDL_Point){x, y}, &loginButtonRect)) {
                        char jsonbuf[4096];
                        if (send_login_and_get_characters(config.host, config.port, input.username, input.password, jsonbuf, sizeof(jsonbuf)) != 0) {
                            printf("Login/network error\n");
                            continue;
                        }
                        cJSON *root = cJSON_Parse(jsonbuf);
                        if (!root || !cJSON_IsArray(root) || cJSON_GetArraySize(root) == 0) {
                            printf("No characters found or login failed\n");
                            if (root) cJSON_Delete(root);
                            continue;
                        }
                        nCharacters = cJSON_GetArraySize(root);
                        for (int i = 0; i < nCharacters && i < MAX_CHARACTERS; i++) {
                            cJSON* c = cJSON_GetArrayItem(root, i);
                            fill_character_struct(&charEntries[i], c);
                            charEntries[i].raw = cJSON_Duplicate(c, 1);
                        }
                        rootJson = root;
                        currentScreen = CHARACTER_SELECT_SCREEN;
                        selectedCharIndex = -1;
                    } else input.activeField = NONE;
                } else if (currentScreen == CHARACTER_SELECT_SCREEN) {
                    int startY = 100;
                    for (int i = 0; i < nCharacters; i++) {
                        SDL_Rect rect = {WINDOW_WIDTH/4 + 50, startY + i*40, 200, 30};
                        if (SDL_PointInRect(&(SDL_Point){x, y}, &rect)) {
                            selectedCharIndex = i;
                            if (e.button.button == SDL_BUTTON_LEFT) {
                                memcpy(&character, &charEntries[selectedCharIndex], sizeof(CharacterData));
                                for (int j = 0; j < nCharacters; j++) {
                                    if (j != selectedCharIndex && charEntries[j].raw)
                                        cJSON_Delete(charEntries[j].raw);
                                }
                                cJSON_Delete(rootJson);
                                rootJson = NULL;
                                currentScreen = GAME_SCREEN;
                            }
                        }
                    }
                } else if (currentScreen == GAME_SCREEN) {
                    if (SDL_PointInRect(&(SDL_Point){x, y}, &hudButtonRects[0])) showEquipment = !showEquipment;
                    else if (SDL_PointInRect(&(SDL_Point){x, y}, &hudButtonRects[1])) showSkills = !showSkills;
                    else if (SDL_PointInRect(&(SDL_Point){x, y}, &hudButtonRects[2])) showSpells = !showSpells;
                    else if (SDL_PointInRect(&(SDL_Point){x, y}, &hudButtonRects[3])) showTeam = !showTeam;
                    else if (point_in_circle(x, y, bagBtnCenterX, bagBtnCenterY, bagBtnRadius)) showBag = !showBag;
                    else if (point_in_circle(x, y, logoutBtnCenterX, logoutBtnCenterY, logoutBtnRadius)) {
                        currentScreen = TITLE_SCREEN;
                        showEquipment = false;
                        showSkills = false;
                        showBag = false;
                        showSpells = false;
                        showTeam = false;
                        input.activeField = NONE;
                        input.username[0] = '\0';
                        input.password[0] = '\0';
                        nCharacters = 0;
                        selectedCharIndex = -1;
                        memset(&character, 0, sizeof(character));
                        if (config.username[0]) strncpy(input.username, config.username, MAX_INPUT_LEN-1);
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        int hover_bag_slot = -1, hover_equip_slot = -1;
        cJSON *hover_itemData = NULL;
        int hover_imageId = -1;

        if (currentScreen == TITLE_SCREEN) {
            if (bgTexture)
                SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
            else {
                SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
                SDL_RenderClear(renderer);
            }
            SDL_Rect usernameRect = {WINDOW_WIDTH / 4 + 50, WINDOW_HEIGHT / 4 + 50, 200, 40};
            SDL_Rect passwordRect = {WINDOW_WIDTH / 4 + 50, WINDOW_HEIGHT / 4 + 100, 200, 40};

            SDL_SetRenderDrawColor(renderer, 80, 80, 100, 200);
            SDL_RenderFillRect(renderer, &usernameRect);
            SDL_RenderFillRect(renderer, &passwordRect);

            SDL_SetRenderDrawColor(renderer, 180, 180, 255, 255);
            if (input.activeField == USERNAME)
                SDL_RenderDrawRect(renderer, &usernameRect);
            else if (input.activeField == PASSWORD)
                SDL_RenderDrawRect(renderer, &passwordRect);

            SDL_Surface* surf;
            SDL_Texture* tex;
            SDL_Rect dst;
            surf = TTF_RenderText_Solid(font, strlen(input.username) ? input.username : " ", textColor);
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst = usernameRect;
            dst.x += 10; dst.y += 10; dst.w = surf->w; dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf); SDL_DestroyTexture(tex);

            char pwBuffer[MAX_INPUT_LEN];
            size_t pwLen = strlen(input.password);
            if (pwLen > 0)
                memset(pwBuffer, '*', pwLen);
            pwBuffer[pwLen] = 0;
            surf = TTF_RenderText_Solid(font, pwLen ? pwBuffer : " ", textColor);
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst = passwordRect;
            dst.x += 10; dst.y += 10; dst.w = surf->w; dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf); SDL_DestroyTexture(tex);

            surf = TTF_RenderText_Solid(font, "Username", textColor);
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst.x = usernameRect.x; dst.y = usernameRect.y - 20; dst.w = surf->w; dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf); SDL_DestroyTexture(tex);

            surf = TTF_RenderText_Solid(font, "Password", textColor);
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst.x = passwordRect.x; dst.y = passwordRect.y - 20; dst.w = surf->w; dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf); SDL_DestroyTexture(tex);

            SDL_SetRenderDrawColor(renderer, 100, 120, 220, 200);
            SDL_RenderFillRect(renderer, &loginButtonRect);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &loginButtonRect);

            surf = TTF_RenderText_Solid(font, "Login", textColor);
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst.x = loginButtonRect.x + (loginButtonRect.w - surf->w) / 2;
            dst.y = loginButtonRect.y + (loginButtonRect.h - surf->h) / 2;
            dst.w = surf->w; dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
        }

        if (currentScreen == CHARACTER_SELECT_SCREEN) {
            SDL_Surface* surf;
            SDL_Texture* tex;
            SDL_Rect dst;
            int startY = 100;
            for (int i = 0; i < nCharacters; i++) {
                char line[128];
                snprintf(line, sizeof(line), "%s Lv %d %s", charEntries[i].name, charEntries[i].level, charEntries[i].race);
                surf = TTF_RenderText_Solid(font, line, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = WINDOW_WIDTH/4 + 50; dst.y = startY + i*40; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
                if (selectedCharIndex == i) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
                    SDL_RenderDrawRect(renderer, &dst);
                }
            }
            surf = TTF_RenderText_Solid(font, "Select a character", textColor);
            tex = SDL_CreateTextureFromSurface(renderer, surf);
            dst.x = WINDOW_WIDTH/4 + 50; dst.y = 60; dst.w = surf->w; dst.h = surf->h;
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
        }

        if (currentScreen == GAME_SCREEN) {
            SDL_Rect hud = {WINDOW_WIDTH - HUD_WIDTH, 0, HUD_WIDTH, HUD_HEIGHT};
            if (hudTexture) SDL_RenderCopy(renderer, hudTexture, NULL, &hud);
            SDL_Rect chat = {0, WINDOW_HEIGHT - CHAT_BAR_HEIGHT, WINDOW_WIDTH, CHAT_BAR_HEIGHT};
            if (chatTexture) SDL_RenderCopy(renderer, chatTexture, NULL, &chat);
            SDL_Rect game = {0, 0, GAME_VIEW_WIDTH, GAME_VIEW_HEIGHT};
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
            SDL_RenderFillRect(renderer, &game);

            // Equipment window with items
            if (showEquipment && equipmentTexture) {
                SDL_RenderCopy(renderer, equipmentTexture, NULL, &equipWindow);
                if (selectedCharIndex >= 0 && charEntries[selectedCharIndex].raw) {
                    cJSON *equipObj = cJSON_GetObjectItemCaseSensitive(charEntries[selectedCharIndex].raw, "equipment");
                    for (int i = 0; i < N_EQUIP_SLOTS; ++i) {
                        const EquipSlotDisplay *slotDef = &equipSlots[i];
                        cJSON *slotVal = equipObj ? cJSON_GetObjectItemCaseSensitive(equipObj, slotDef->slot) : NULL;
                        int imageId = -1;
                        cJSON *itemData = NULL;
                        if (slotVal && cJSON_IsObject(slotVal)) {
                            itemData = cJSON_GetObjectItemCaseSensitive(slotVal, "item_data");
                            if (itemData && cJSON_IsObject(itemData)) {
                                cJSON *imgVal = cJSON_GetObjectItemCaseSensitive(itemData, "item_image");
                                if (imgVal && cJSON_IsNumber(imgVal)) {
                                    imageId = imgVal->valueint;
                                }
                            }
                        } else if (slotVal && cJSON_IsNumber(slotVal)) {
                            imageId = slotVal->valueint;
                        }
                        SDL_Rect iconRect = {equipWindow.x + slotDef->x, equipWindow.y + slotDef->y, 38, 38};
                        if (imageId > 0) {
                            char imagePath[256];
                            snprintf(imagePath, sizeof(imagePath), "images/items/%d.png", imageId);
                            SDL_Surface *iconSurf = IMG_Load(imagePath);
                            if (iconSurf) {
                                SDL_Texture *iconTex = SDL_CreateTextureFromSurface(renderer, iconSurf);
                                SDL_RenderCopy(renderer, iconTex, NULL, &iconRect);
                                SDL_DestroyTexture(iconTex);
                                SDL_FreeSurface(iconSurf);
                            } else {
                                SDL_SetRenderDrawColor(renderer, 80, 80, 100, 80);
                                SDL_RenderFillRect(renderer, &iconRect);
                            }
                        } else {
                            SDL_SetRenderDrawColor(renderer, 40, 40, 60, 60);
                            SDL_RenderFillRect(renderer, &iconRect);
                        }
                        SDL_SetRenderDrawColor(renderer, 120, 120, 180, 170);
                        SDL_RenderDrawRect(renderer, &iconRect);

                        if (mouse_x >= iconRect.x && mouse_x <= iconRect.x + iconRect.w &&
                            mouse_y >= iconRect.y && mouse_y <= iconRect.y + iconRect.h &&
                            imageId > 0) {
                            hover_equip_slot = i;
                            hover_imageId = imageId;
                            hover_itemData = itemData;
                        }
                    }
                }
            }
            if (showSkills && skillsTexture) {
                SDL_RenderCopy(renderer, skillsTexture, NULL, &skillsWindow);
                int x = skillsWindow.x + 20;
                int y = skillsWindow.y + 30;
                char info[128];
                SDL_Surface* surf;
                SDL_Texture* tex;
                SDL_Rect dst;

                snprintf(info, sizeof(info), "Name: %s", character.name);
                surf = TTF_RenderText_Solid(font, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 20;

                snprintf(info, sizeof(info), "Lv %d", character.level);
                surf = TTF_RenderText_Solid(font, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 20;

                snprintf(info, sizeof(info), "Race: %s", character.race);
                surf = TTF_RenderText_Solid(font, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 20;

                snprintf(info, sizeof(info), "Gender: %s", character.gender);
                surf = TTF_RenderText_Solid(font, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 20;

                snprintf(info, sizeof(info), "Base Strength: %d", character.base_strength);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Endurance: %d", character.base_endurance);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Wisdom: %d", character.base_wisdom);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Mystic: %d", character.base_mystic);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Agility: %d", character.base_agility);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Accuracy: %d", character.base_accuracy);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Intellect: %d", character.base_intellect);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex); y += 18;

                snprintf(info, sizeof(info), "Base Luck: %d", character.base_luck);
                surf = TTF_RenderText_Solid(statFont, info, textColor);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                dst.x = x; dst.y = y; dst.w = surf->w; dst.h = surf->h;
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex);

                int skill_x = skillsWindow.x + skillsWindow.w / 2 + 10;
                int skill_y = skillsWindow.y + 30;
                struct {
                    const char* label; int value; int exp;
                } skills[] = {
                    {"Sword",      character.skill_sword,      character.skill_sword_exp},
                    {"Spear",      character.skill_spear,      character.skill_spear_exp},
                    {"Axe",        character.skill_axe,        character.skill_axe_exp},
                    {"Dagger",     character.skill_dagger,     character.skill_dagger_exp},
                    {"Bow",        character.skill_bow,        character.skill_bow_exp},
                    {"Staff",      character.skill_staff,      character.skill_staff_exp},
                    {"Shield",     character.skill_shield,     character.skill_shield_exp},
                    {"Magick",     character.skill_magick,     character.skill_magick_exp},
                    {"Rune",       character.skill_rune,       character.skill_rune_exp},
                    {"Protection", character.skill_protection, character.skill_protection_exp},
                    {"Healing",    character.skill_healing,    character.skill_healing_exp},
                    {"Mining",     character.skill_mining,     character.skill_mining_exp},
                    {"Blacksmith", character.skill_blacksmith, character.skill_blacksmith_exp},
                    {"Foraging",   character.skill_foraging,   character.skill_foraging_exp},
                    {"Alchemy",    character.skill_alchemy,    character.skill_alchemy_exp},
                    {"Fishing",    character.skill_fishing,    character.skill_fishing_exp},
                    {"Cooking",    character.skill_cooking,    character.skill_cooking_exp},
                };

                for (int i = 0; i < sizeof(skills)/sizeof(skills[0]); ++i) {
                    snprintf(info, sizeof(info), "%s: %d (exp: %d)", skills[i].label, skills[i].value, skills[i].exp);
                    surf = TTF_RenderText_Solid(skillFont, info, textColor);
                    tex = SDL_CreateTextureFromSurface(renderer, surf);
                    dst.x = skill_x; dst.y = skill_y; dst.w = surf->w; dst.h = surf->h;
                    SDL_RenderCopy(renderer, tex, NULL, &dst);
                    SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
                    skill_y += 16;
                }
            }
            if (showBag && bagTexture) {
                SDL_RenderCopy(renderer, bagTexture, NULL, &bagWindow);
                if (selectedCharIndex >= 0 && charEntries[selectedCharIndex].raw) {
                    cJSON *bagObj = cJSON_GetObjectItemCaseSensitive(charEntries[selectedCharIndex].raw, "bag");
                    if (bagObj) {
                        int iconSize = 40;
                        int pad = 4;
                        int startX = bagWindow.x + 50;
                        int startY = bagWindow.y + 50;
                        int maxCols = 7;
                        char slotname[16];
                        for (int slot = 0; slot < 49; slot++) {
                            snprintf(slotname, sizeof(slotname), "bag%d", slot);
                            cJSON *slotVal = cJSON_GetObjectItemCaseSensitive(bagObj, slotname);
                            int iconCol = slot % maxCols;
                            int iconRow = slot / maxCols;
                            SDL_Rect iconRect = {
                                startX + iconCol * (iconSize + pad),
                                startY + iconRow * (iconSize + pad),
                                iconSize, iconSize
                            };
                            int imageId = -1;
                            cJSON *itemData = NULL;
                            if (slotVal && cJSON_IsObject(slotVal)) {
                                itemData = cJSON_GetObjectItemCaseSensitive(slotVal, "item_data");
                                if (itemData && cJSON_IsObject(itemData)) {
                                    cJSON *imgVal = cJSON_GetObjectItemCaseSensitive(itemData, "item_image");
                                    if (imgVal && cJSON_IsNumber(imgVal)) {
                                        imageId = imgVal->valueint;
                                    }
                                }
                            } else if (slotVal && cJSON_IsNumber(slotVal)) {
                                imageId = slotVal->valueint;
                            }
                            if (imageId > 0) {
                                char imagePath[256];
                                snprintf(imagePath, sizeof(imagePath), "images/items/%d.png", imageId);
                                SDL_Surface *iconSurf = IMG_Load(imagePath);
                                if (iconSurf) {
                                    SDL_Texture *iconTex = SDL_CreateTextureFromSurface(renderer, iconSurf);
                                    SDL_RenderCopy(renderer, iconTex, NULL, &iconRect);
                                    SDL_DestroyTexture(iconTex);
                                    SDL_FreeSurface(iconSurf);
                                } else {
                                    SDL_SetRenderDrawColor(renderer, 80, 80, 100, 80);
                                    SDL_RenderFillRect(renderer, &iconRect);
                                }
                            } else {
                                SDL_SetRenderDrawColor(renderer, 40, 40, 60, 60);
                                SDL_RenderFillRect(renderer, &iconRect);
                            }
                            SDL_SetRenderDrawColor(renderer, 100, 100, 150, 120);
                            SDL_RenderDrawRect(renderer, &iconRect);

                            if (mouse_x >= iconRect.x && mouse_x <= iconRect.x + iconRect.w &&
                                mouse_y >= iconRect.y && mouse_y <= iconRect.y + iconRect.h &&
                                imageId > 0 && itemData) {
                                hover_bag_slot = slot;
                                hover_itemData = itemData;
                                hover_imageId = imageId;
                            }
                        }
                    }
                }
            }
            if (hover_itemData) {
                render_item_tooltip(renderer, font, mouse_x, mouse_y, hover_itemData, hover_imageId);
            }
            if (showSpells) {
                SDL_SetRenderDrawColor(renderer, 40, 20, 60, 230);
                SDL_RenderFillRect(renderer, &spellsWindow);
                SDL_SetRenderDrawColor(renderer, 180, 180, 220, 255);
                SDL_RenderDrawRect(renderer, &spellsWindow);
                SDL_Surface* surf = TTF_RenderText_Solid(font, "Spells", textColor);
                SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_Rect dst = {spellsWindow.x + 20, spellsWindow.y + 20, surf->w, surf->h};
                SDL_RenderCopy(renderer, tex, NULL, &dst);
                SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
            }
            SDL_RenderDrawRect(renderer, &bagLeftArrow);
            SDL_RenderDrawRect(renderer, &bagInventory);
            SDL_RenderDrawRect(renderer, &bagRightArrow);
        }

        if (config.show_fps) {
            frames++;
            Uint32 now = SDL_GetTicks();
            if (now - fpsLast >= 1000) {
                fps = frames * 1000.0f / (now - fpsLast);
                fpsLast = now;
                frames = 0;
            }
            char fps_buf[32];
            snprintf(fps_buf, sizeof(fps_buf), "FPS: %.1f", fps);
            SDL_Surface* fps_surf = TTF_RenderText_Solid(font, fps_buf, textColor);
            SDL_Texture* fps_tex = SDL_CreateTextureFromSurface(renderer, fps_surf);
            SDL_Rect fps_rect = {8, 8, fps_surf->w, fps_surf->h};
            SDL_RenderCopy(renderer, fps_tex, NULL, &fps_rect);
            SDL_FreeSurface(fps_surf);
            SDL_DestroyTexture(fps_tex);
        }

        SDL_RenderPresent(renderer);
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 16) SDL_Delay(16 - frameTime);
    }

    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(hudTexture);
    SDL_DestroyTexture(chatTexture);
    SDL_DestroyTexture(equipmentTexture);
    SDL_DestroyTexture(bagTexture);
    SDL_DestroyTexture(skillsTexture);
    TTF_CloseFont(font);
    TTF_CloseFont(statFont);
    TTF_CloseFont(skillFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_StopTextInput();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
