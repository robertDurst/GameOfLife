/**
 * robertDurst @ 2020
 * 
 * 8x8 Conway's Game of Life Universe via bitwise operations in C
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// ================================= GLOBALS ============================== //
const unsigned long ROW_MASK = 255;
const unsigned long ROW_CELLS = 8;
const unsigned long TOTAL_CELLS = 64;
// ======================================================================== //

// =============================== UTILITY CODE =========================== //
// https://stackoverflow.com/questions/3930363/implement-time-delay-in-c
void waitFor(unsigned int secs)
{
	unsigned int retTime = time(0) + secs;
	while (time(0) < retTime)
		;
}

unsigned long getRow(unsigned long num, int row)
{
	return (num & (ROW_MASK << (row * ROW_CELLS))) >> (row * ROW_CELLS);
}

int getNeighbors(unsigned long num, int i)
{
	int hasUp = i > (ROW_CELLS - 1);
	int hasDown = i < (TOTAL_CELLS - ROW_CELLS);
	int hasLeft = i != 0 && (i % ROW_CELLS) - 1 == (i - 1) % ROW_CELLS;
	int hasRight = i != (TOTAL_CELLS - 1) && (i % ROW_CELLS) + 1 == (i + 1) % ROW_CELLS;

	int neighbors = 0;
	unsigned long x = 1;

	neighbors += hasUp && (num & (x << (i - ROW_CELLS))) > 0;
	neighbors += hasUp && hasLeft && (num & (x << (i - ROW_CELLS - 1))) > 0;
	neighbors += hasUp && hasRight && (num & (x << (i - ROW_CELLS + 1))) > 0;
	neighbors += hasDown && (num & (x << (i + ROW_CELLS))) > 0;
	neighbors += hasDown && hasLeft && (num & (x << (i + ROW_CELLS - 1))) > 0;
	neighbors += hasDown && hasRight && (num & (x << (i + ROW_CELLS + 1))) > 0;
	neighbors += hasLeft && (num & (x << (i - 1))) > 0;
	neighbors += hasRight && (num & (x << (i + 1))) > 0;

	return neighbors;
}
// ======================================================================== //

// =============================== DISPLAY CODE =========================== //
// https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
void clear_screen()
{
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void display(unsigned long num, int iteration)
{
	clear_screen();
	printf("Conway's Game of Life\n");
	printf("Iteration: %d\n\n", iteration);
	printf("------------\n");
	for (int i = 0; i < 8; i++)
	{
		unsigned long row = getRow(num, i);
		printf("| ");
		for (int j = 0; j < 8; j++)
		{
			printf(row & (unsigned long)1 << j ? "X" : " ");
		}
		printf(" |\n");
	}
	printf("------------\n");
}
// ======================================================================== //

// ================================ GAME CODE ============================= //
unsigned long init()
{
	// https://stackoverflow.com/questions/7920860/how-to-generate-large-random-numbers-c
	return (((unsigned long)rand() << 0) & 0x00000000FFFFFFFFull) |
		   (((unsigned long)rand() << 32) & 0xFFFFFFFF00000000ull);
};

unsigned long iterate(unsigned long population)
{
	unsigned long population_copy = population;
	for (int i = 0; i < TOTAL_CELLS; i++)
	{
		unsigned long cur = (unsigned long)1 << (i);
		int neighbors = getNeighbors(population, i);
		if (population & cur)
		{
			if (neighbors < 2 || neighbors > 3)
				population_copy ^= cur;
		}
		else
		{
			if (neighbors == 3)
				population_copy ^= cur;
		}
	}

	return population_copy;
}

void simulate()
{
	int i = 0;
	unsigned long population = init();
	while (population)
	{
		waitFor(1);
		population = iterate(population);
		display(population, i++);
	}
}
// ======================================================================== //

int main()
{
	srand(time(NULL));
	simulate();
}
