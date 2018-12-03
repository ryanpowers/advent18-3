/*
 ============================================================================
 Name        : advent3.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BOARD_WIDTH (1024)
#define BOARD_HEIGHT (1024)

static uint64_t material[BOARD_WIDTH*BOARD_HEIGHT/sizeof(uint64_t)/8] = { 0 };
static uint64_t doubled[BOARD_WIDTH*BOARD_HEIGHT/sizeof(uint64_t)/8] = { 0 };

static uint16_t overlaps[BOARD_WIDTH*BOARD_HEIGHT] = {0};

uint32_t cutClaim(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
uint32_t checkClaim(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
uint32_t cutOne(uint32_t x, uint32_t y);

int main(void)
{
    FILE * input;

    input = fopen("input.txt", "r");
    if (input)
    {
        uint32_t doubles = 0;
        while(!feof(input))
        {
            uint32_t id, x, y, w, h;
            if (fscanf(input, "#%i @ %i,%i: %ix%i\n", &id, &x, &y, &w, &h) == 5)
            {
                doubles += cutClaim(x, y, w, h);
            }
        }

        printf("doubled cuts = %i\n", doubles);

        fseek(input, 0, SEEK_SET);
        while(!feof(input))
        {
            uint32_t id, x, y, w, h;
            if (fscanf(input, "#%i @ %i,%i: %ix%i\n", &id, &x, &y, &w, &h) == 5)
            {
                if (checkClaim(x, y, w, h) == 0)
                {
                    printf("unique cut = %i\n", id);
                }
            }
        }

        return 0;
    }
    return 1;
}


uint32_t cutClaim(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    uint32_t ret = 0;
    for (uint32_t j = y; j < (y+h); j++)
    {
        for (uint32_t i = x; i < (x+w); i++)
        {
            overlaps[i + j*BOARD_WIDTH]++;
            ret += cutOne(i, j);
        }
    }
    return (ret);
}

uint32_t checkClaim(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    uint32_t ret = 0;
    for (uint32_t j = y; j < (y+h); j++)
    {
        for (uint32_t i = x; i < (x+w); i++)
        {
            ret += overlaps[i + j*BOARD_WIDTH]-1;
        }
    }
    return (ret);
}

uint32_t cutOne(uint32_t x, uint32_t y)
{
    static const uint32_t elemsPerRow = sizeof(material) / BOARD_HEIGHT / sizeof(material[0]);
    uint32_t idx = (y * elemsPerRow) + (x / sizeof(material[0]) / 8);
    uint64_t mask = (uint64_t)1 << (x % (sizeof(mask) * 8));
    uint32_t ret = ((material[idx] & mask) & ~(doubled[idx] & mask)) != 0;
    doubled[idx] |= material[idx] & mask;
    material[idx] |= mask;
    return ret;
}
