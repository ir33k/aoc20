#include <stdio.h>

typedef struct { int x; int y; int z; } Point;

int
points_get_neighbors_count(Point * points, int * points_count,
                           Point * target) {
  int i,x,y,z, result = 0;

  for (x=-1; x<2; x++)
    for (y=-1; y<2; y++)
      for (z=-1; z<2; z++)
        for (i=0; i < *points_count; i++)
          if (points[i].x == target->x + x &&
              points[i].y == target->y + y &&
              points[i].z == target->z + z)
            result++;

  return result - 1;
}

void
points_copy(Point * source, int * source_count,
            Point * target, int * target_count) {
  for (int i=0; i < *source_count; i ++)
    target[i] = source[i];

  *target_count = *source_count;
}

int
solve1(char file_name[6], int cycles) {
  FILE * file = fopen(file_name, "r");

  if (file == NULL) return -1;

  char line[16];
  int i,j,k, active_neighbors;

  int x=0, y=0, z=0;
  Point points1[1024], points2[1024], neighbor;
  int points1_count = 0, points2_count = 0;

  while (fgets(line, sizeof line, file) != NULL) {
    for (x=0; line[x] != '\n'; x++)
      if (line[x] == '#') {
        points1[points1_count].x = x;
        points1[points1_count].y = y;
        points1[points1_count].z = z;
        points1_count++;
      }

    y++;
  }

  fclose(file);

  /* printf("cycle: 0\n"); */
  /* for (l=0; l < points1_count; l++) */
  /*   printf("%d,%d,%d\n", points1[l].x, points1[l].y, points1[l].z); */

  for (i=0; i<cycles; i++) {
    /* printf("cycle: %d\n", i+1); */
    points2_count = 0;

    for (j=0; j < points1_count; j++) {
      active_neighbors = points_get_neighbors_count(points1, &points1_count, &points1[j]);

      if (active_neighbors == 2 || active_neighbors == 3) {
        points2[points2_count] = points1[j];
        points2_count++;
      }

      for (x=-1; x<2; x++)
        for (y=-1; y<2; y++)
          for (z=-1; z<2; z++) {
            neighbor.x = points1[j].x + x;
            neighbor.y = points1[j].y + y;
            neighbor.z = points1[j].z + z;

            for (k=0; k < points1_count; k++)
              if (points1[k].x == neighbor.x &&
                  points1[k].y == neighbor.y &&
                  points1[k].z == neighbor.z)
                goto skip;
            
            for (k=0; k < points2_count; k++)
              if (points2[k].x == neighbor.x &&
                  points2[k].y == neighbor.y &&
                  points2[k].z == neighbor.z)
                goto skip;

            active_neighbors = points_get_neighbors_count(points1, &points1_count, &neighbor);

            /* printf("neighbor (%d) %d,%d,%d\n", */
            /*        active_neighbors, */
            /*        neighbor.x, */
            /*        neighbor.y, */
            /*        neighbor.z); */

            if (active_neighbors + 1 == 3) {
              points2[points2_count] = neighbor;
              points2_count++;
            }

          skip: continue;
          }

    }

    points_copy(points2, &points2_count, points1, &points1_count);

    /* for (l=0; l < points1_count; l++) */
    /*   printf("%d,%d,%d\n", points1[l].x, points1[l].y, points1[l].z); */
  }

  return points1_count;
}

int
main() {
  printf("%d (112)\n", solve1("17i1", 6));
  printf("%d (395)\n", solve1("17i2", 6));
  return 0;
}
