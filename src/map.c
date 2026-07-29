typedef struct cute_tiled_chunk_t cute_tiled_chunk_t;

    #define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
    #define CUTE_TILED_IMPLEMENTATION
    #include "external/cute._tiled.h"
    #include "map.h"
    #include <SDL3_image/SDL_image.h>
    #include <SDL3/SDL_log.h>
    #include <stdlib.h>
    #include <string.h>

    static cute_tiled_map_t* g_map = NULL;

    typedef struct {
        SDL_Texture* tex;
        int firstgid;
        int columns;
        int margin;
        int spacing;
        int tilewidth;
        int tileheight;
    } TilesetTex;

    static TilesetTex* g_tilesets = NULL;
    static int g_num_tilesets = 0;

    void MapLoad(const char* filepath, SDL_Renderer* renderer) {
        if (g_map) MapDestroy();

        g_map = cute_tiled_load_map_from_file(filepath, NULL);
        if (!g_map) return;

        int count = 0;
        cute_tiled_tileset_t* ts = g_map->tilesets;
        while (ts) { count++; ts = ts->next; }

        g_num_tilesets = count;
        g_tilesets = calloc(count, sizeof(TilesetTex));

        ts = g_map->tilesets;
        int i = 0;
        while (ts) {
            g_tilesets[i].tex = IMG_LoadTexture(renderer, ts->image.ptr);
            g_tilesets[i].firstgid = ts->firstgid;
            g_tilesets[i].columns = ts->columns;
            g_tilesets[i].margin = ts->margin;
            g_tilesets[i].spacing = ts->spacing;
            g_tilesets[i].tilewidth = ts->tilewidth;
            g_tilesets[i].tileheight = ts->tileheight;
            i++;
            ts = ts->next;
        }
    }

    void MapRender(SDL_Renderer* renderer, float target_w, float target_h, float cam_x, float cam_y, float zoom, bool render_tops) {
        if (!g_map) return;

        float map_w = (float)(g_map->width * g_map->tilewidth);
        float map_h = (float)(g_map->height * g_map->tileheight);
        float sx = target_w / map_w;
        float sy = target_h / map_h;

        cute_tiled_layer_t* layer = g_map->layers;
        while (layer) {
            if (strcmp(layer->type.ptr, "tilelayer") == 0 && layer->visible == 1) {
                 bool is_top = (strcmp(layer->name.ptr, "Tops") == 0);
                if (is_top != render_tops) {
                    layer = layer->next;
                    continue;
                }
                int* data = layer->data;
                for (int y = 0; y < layer->height; y++) {
                    for (int x = 0; x < layer->width; x++) {
                        int gid = data[y * layer->width + x];
                        if (gid == 0) continue;

                        int pure_gid = gid & ~(0xE0000000);

                        TilesetTex* ts = NULL;
                        for (int i = g_num_tilesets - 1; i >= 0; i--) {
                            if (pure_gid >= g_tilesets[i].firstgid) { ts = &g_tilesets[i]; break; }
                        }

                        if (ts && ts->tex) {
                            int local_id = pure_gid - ts->firstgid;
                            int tx = (local_id % ts->columns) * (ts->tilewidth + ts->spacing) + ts->margin;
                            int ty = (local_id / ts->columns) * (ts->tileheight + ts->spacing) + ts->margin;

                            SDL_FRect src = {(float)tx, (float)ty, (float)ts->tilewidth, (float)ts->tileheight};

                            // Scale map to 1600x900 world first, then apply camera & zoom
                            SDL_FRect dst = {
                                ((x * g_map->tilewidth * sx) - cam_x) * zoom,
                                ((y * g_map->tileheight * sy) - cam_y) * zoom,
                                (g_map->tilewidth * sx) * zoom,
                                (g_map->tileheight * sy) * zoom
                            };

                            SDL_RenderTexture(renderer, ts->tex, &src, &dst);
                        }
                    }
                }
            }
            layer = layer->next;
        }
    }

    void MapDestroy(void) {
        if (!g_map) return;
        for (int i = 0; i < g_num_tilesets; i++) {
            if (g_tilesets[i].tex) SDL_DestroyTexture(g_tilesets[i].tex);
        }
        free(g_tilesets);
        g_tilesets = NULL;
        g_num_tilesets = 0;

        cute_tiled_free_map(g_map);
        g_map = NULL;
    }

bool MapCheckCollision(float x, float y, float w, float h, float target_w, float target_h) {
        if (!g_map) return false;
        float sx = target_w / (g_map->width * g_map->tilewidth);
        float sy = target_h / (g_map->height * g_map->tileheight);
        float tile_w = g_map->tilewidth * sx;
        float tile_h = g_map->tileheight * sy;

        int start_col = (int)(x / tile_w);
        int end_col = (int)((x + w) / tile_w);
        int start_row = (int)(y / tile_h);
        int end_row = (int)((y + h) / tile_h);

        cute_tiled_layer_t* layer = g_map->layers;
        while (layer) {
            if (strcmp(layer->name.ptr, "Collision") == 0) {
                int* data = layer->data;
                for (int r = start_row; r <= end_row; r++) {
                    for (int c = start_col; c <= end_col; c++) {
                        if (r >= 0 && r < layer->height && c >= 0 && c < layer->width) {
                            if (data[r * layer->width + c] != 0) return true;
                        }
                    }
                }
            }
            layer = layer->next;
        }
        return false;
    }    
    
    int MapGetTileCols(void) { return g_map ? g_map->width : 30; }
    int MapGetTileRows(void) { return g_map ? g_map->height : 20; }