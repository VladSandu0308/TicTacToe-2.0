// Copyright 2018 Sandu Vlad
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define p1 'X'
#define p2 '0'

// functie pentru citirea matricei mare
char **read_matrixA(int n) {
  int x = n * n;
  char **a = malloc(x * sizeof(char *));
  for (int i = 0; i < x; ++i) a[i] = calloc(x, sizeof(char));
  for (int i = 0; i < x; ++i) memset(a[i], '.', x * sizeof(char));
  return a;
}
// functie pentru citirea macroboardului
char **read_matrixB(int n) {
  char **b = malloc(n * sizeof(char *));
  for (int i = 0; i < n; ++i) b[i] = calloc(n, sizeof(char));
  for (int i = 0; i < n; ++i) memset(b[i], '-', n * sizeof(char));
  return b;
}
// functie pentru printarea macroboardului
void printB(char **b, int n) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) printf("%c", b[i][j]);
    printf("\n");
  }
}
// 2 functii pentru eliberarea memoriei celor 2 matrice
void free_2dA(char **a, int n) {
  int x = n * n;
  for (int i = 0; i < x; ++i) free(a[i]);
  free(a);
}

void free_2dB(char **b, int n) {
  for (int i = 0; i < n; ++i) free(b[i]);
  free(b);
}
// algoritmul de timp round robin
int *robin(char **a, int n) {
  // Am initializat un vector, astfel incat sa pot intoarce indicii convenabili
  int *v = malloc(4 * sizeof(int *));
  int k = 0, aux = 0, temp = 0, i = 0;
  for (i = 0; i < (n * n); ++i) {
    k = 0;
    aux = i;
    while (aux < n * n) {
      if (a[aux][k] == '.') {
        v[0] = aux;
        v[1] = k;
        return v;
      }
      if (v[2] == 1) {
        v[2] = 2;
        return v;
      }
      if (i == (n * n) - 1) {
        v[2] = 1;
        return v;
      }
      temp++;
      k++;
      aux++;
    }
    k = 0;
    aux = i;
    while (aux < n * n - 1) {
      if (a[k][aux + 1] == '.') {
        v[0] = k;
        v[1] = aux + 1;
        v[3] = temp++;
        return v;
      }
      temp++;
      k++;
      aux++;
    }
  }
  return 0;
}
// functie pentru eroarea 'NOT YOUR TURN'
void turns(int *i, char player, int *m, int *auy) {
  if ((*i) % 2 == 0 && player == p2) {
    printf("NOT YOUR TURN\n");
    (*m)--;
    (*i)--;
    (*auy)++;
  }
  if ((*i) % 2 == 1 && player == p1) {
    printf("NOT YOUR TURN\n");
    (*m)--;
    (*i)--;
    (*auy)++;
  }
}
// functie pentru eroarea 'INVALID INDEX'
void invld(char **a, int n, int x, int y, int *v, char player, int *auy) {
  if (x >= (n * n) || y >= (n * n) || x < 0 || y < 0) {
    *auy = 1;
    v = robin(a, n);
    a[v[0]][v[1]] = player;
    printf("INVALID INDEX\n");
    if (v[2] == 1) {
      printf("FULL BOARD\n");
    }
  }
}
// functie pentru eroarea 'NOT AN EMPTY CELL'
void occup(char **a, int n, int x, int y, int *v, char player, int *auy) {
  if (a[x][y] == '0' || a[x][y] == 'X') {
    *auy = 1;
    v = robin(a, n);
    a[v[0]][v[1]] = player;
    printf("NOT AN EMPTY CELL\n");
    if (v[2] == 1) {
      printf("FULL BOARD\n");
    }
  }
}
// functie pentru completarea boardului in pozitia introdusa
int fill(char **a, int i, char player, int x, int y) {
  if (i % 2 == 0 && player == p1 && a[x][y] == '.') {
    return player;
  }
  if (i % 2 == 1 && player == p2 && a[x][y] == '.') {
    return player;
  }
  return 0;
}
// functie pentru verificarea mircoboardului(daca a castigat cineva jocul
// respectiv)
// in care se afla elementul introdus, si implicit modificare macroboardului
// cand este cazul
void game(int x, int y, int n, char **a, char **b) {
  int counter = 1, auy = y, aux = x, i = 0, j = 0;
  while (aux % n != 0) {
    aux--;
  }
  while (auy % n != 0) {
    auy--;
  }
  counter = 1;
  i = aux;
  for (i = aux; i < aux + n; ++i) {
    counter = 1;
    for (j = auy + 1; j < auy + n; ++j) {
      if (a[i][j] == a[i][j - 1]) {
        counter++;
      } else {
        break;
      }
    }
    if (counter == n && a[i][j - 1] != '.' && b[x / n][y / n] == '-') {
      b[x / n][y / n] = a[i][j - 1];
      return;
    }
  }
  counter = 1;
  for (j = auy; j < auy + n; ++j) {
    counter = 1;
    for (i = aux + 1; i < aux + n; ++i) {
      if (a[i][j] == a[i - 1][j]) {
        continue;
      } else {
        counter = 0;
        break;
      }
    }
    if (counter == 1 && a[i - 1][j] != '.' && b[x / n][y / n] == '-') {
      b[x / n][y / n] = a[i - 1][j];
      return;
    }
  }

  counter = 1;
  j = auy + 1;
  for (i = aux + 1, j = auy + 1; i <= aux + n - 1; ++i, ++j) {
    if (a[i][j] == a[i - 1][j - 1]) {
      counter++;
    } else {
      break;
    }
    if (counter == n && a[i][j] != '.' && b[x / n][y / n] == '-') {
      b[x / n][y / n] = a[i][j];
      return;
    }
  }
  counter = 1;
  for (i = aux + n - 1, j = auy; j < auy + n - 1; i--, ++j) {
    if (a[i][j] == a[i - 1][j + 1]) {
      continue;
    } else {
      counter = 0;
      break;
    }
  }
  if (counter == 1 && a[i][j] != '.' && b[aux / n][auy / n] == '-') {
    b[aux / n][auy / n] = a[i][j];
    return;
  }
}
// functie pentru stabilirea castigatorului
void winner(char **b, int n) {
  int aux = 0, auy = 0, i = 0, j = 0, counter = 1;
  for (i = 0; i < n; ++i) {
    counter = 1;
    for (j = 1; j < n; ++j) {
      if (b[i][j] == b[i][j - 1]) {
        counter++;
      } else {
        break;
      }
    }
    if (counter == n && b[i][j - 1] == 'X') {
      aux++;
    }
    if (counter == n && b[i][j - 1] == '0') {
      auy++;
    }
  }

  for (j = 0; j < n; ++j) {
    counter = 1;
    for (i = 1; i < n; ++i) {
      if (b[i][j] == b[i - 1][j]) {
        counter++;
      } else {
        break;
      }
    }
    if (counter == n && b[i - 1][j] == 'X') {
      aux++;
    }
    if (counter == n && b[i - 1][j] == '0') {
      auy++;
    }
  }
  counter = 1;
  for (i = 1, j = 1; i < n; ++i, ++j) {
    if (b[i][j] == b[i - 1][j - 1]) {
      counter++;
    } else {
      break;
    }
    if (counter == n && b[i][j] == 'X') {
      aux++;
    }
    if (counter == n && b[i][j] == '0') {
      auy++;
    }
  }
  counter = 1;
  for (i = 1, j = n - 2; i < n; ++i, j--) {
    if (b[i][j] == b[i - 1][j + 1]) {
      counter++;
    } else {
      break;
    }
    if (counter == n && b[i][j] == 'X') {
      aux++;
    }
    if (counter == n && b[i][j] == '0') {
      auy++;
    }
  }
  if (aux > auy) {
    printf("X won\n");
  }
  if (auy > aux) {
    printf("0 won\n");
  }
  if (auy == aux) {
    printf("Draw again! Let's play darts!\n");
  }
}
// functie pentru stabilirea coeficientilor de atentie pentru fiecare jucator
void attention(char **a, char **b, int n, int *tempx, int *tempy) {
  int aux = 0, auy = 0, auxf = 0, auyf = 0;
  double medx = 0, medy = 0;
  for (int i = 0; i < n * n; ++i)
    for (int j = 0; j < n * n; ++j) {
      if (a[i][j] == 'X') {
        aux++;
      }
      if (a[i][j] == '0') {
        auy++;
      }
    }
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      if (b[i][j] == 'X') {
        auxf++;
      }
      if (b[i][j] == '0') {
        auyf++;
      }
    }
  medx = (double)(auxf - (*tempx)) / (double)aux;
  medy = (double)(auyf - (*tempy)) / (double)auy;
  if (aux == 0) {
    printf("X N/A\n");
  } else {
    printf("X %.10f\n", medx);
  }
  if (auy == 0) {
    printf("0 N/A\n");
  } else {
    printf("0 %.10f\n", medy);
  }
}

int main() {
  char player, **b, **a;
  int n, m, i = 0, auy = 0, *v, tempx = 0, tempy = 0, alfa = 0;
  int x, y;
  scanf("%d%d", &n, &m);
  // alocarea celor 2 matrice
  a = read_matrixA(n);
  b = read_matrixB(n);
  for (i = 0; i < m; ++i) {
    auy = 0;
    scanf(" %c%d%d", &player, &x, &y);
    v = robin(a, n);
    // condiita daca i-ul depaseste numarul de mutari posibile,
    // sa nu se mai continue algortimul
    if (i > n * n * n * n) {
      break;
    }
    // verficarea erorilor
    turns(&i, player, &m, &auy);
    if (auy == 0) {
      invld(a, n, x, y, v, player, &auy);
    }
    if (auy == 0) {
      occup(a, n, x, y, v, player, &auy);
    }
    if (auy == 0) {
      a[x][y] = fill(a, i, player, x, y);
    }
    if (auy == 0) {
      game(x, y, n, a, b);
      continue;
    }
    // stochez b-ul inainte de a verifica microboardul,
    // astfel incat daca apar diferente dupa verificare
    // sa tin cont de ele cand verific coeficientul de atentie
    alfa = b[v[0] / n][v[1] / n];

    if (auy == 1) {
      game(v[0], v[1], n, a, b);
      if (alfa != b[v[0] / n][v[1] / n]) {
        if (player == 'X') {
          tempx++;
        }
        if (player == '0') {
          tempy++;
        }
      }
    }
  }
  // afisez macroboardul, castigatorul si coeficientii de atentie
  printB(b, n);
  winner(b, n);
  attention(a, b, n, &tempx, &tempy);
  free_2dA(a, n);
  free_2dB(b, n);
  return 0;
}
