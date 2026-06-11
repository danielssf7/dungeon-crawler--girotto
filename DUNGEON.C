#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <windows.h> 
#else
    #include <unistd.h> // Necessário para o usleep()
#endif

#define TAM 10
#define TAM1 10 
#define TAM2 15
#define TAM3 25

// STRUCT DOS DADOS
typedef struct
{
    int vidas;
    char armas[20];
    int chaves;
} stts;

// STRUCT DOS MOBS
typedef struct
{
    int x;
    int y;
    int vivo;
    char simbolo;
} mobs;

// FUNCAO UNIVERSAL QUE DESENHA QUALQUER MAPA
void desenho(int dim, char mapa[dim][dim])
{
    int i, j;

    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            printf("%c ", mapa[i][j]);
        }
        printf("\n");
    }
}

// Verifica colisoes (Apenas paredes e obstáculos intransponíveis)
int colisao(char obj)
{
    if(obj == '*' || obj == 'N' || obj == 'D' || obj == 'k')
    {
        return 1;
    }
    return 0;
}

// Funcao que define movimento e colisao universal
void mov(int dim, char tecla, int *x, int *y, char *person, char mapa[dim][dim])
{
    if(tecla == 'w' && colisao(mapa[*x - 1][*y]) == 0)
    {
        *person = '^';
        (*x)--;
    }
    else if(tecla == 'a' && colisao(mapa[*x][*y - 1]) == 0)
    {
        *person = '<';
        (*y)--;
    }
    else if(tecla == 's' && colisao(mapa[*x + 1][*y]) == 0)
    {
        *person = 'v';
        (*x)++;
    }
    else if(tecla == 'd' && colisao(mapa[*x][*y + 1]) == 0)
    {
        *person = '>';
        (*y)++;
    }
}

// FUNCAO QUE FAZ INTERACAO COM O NPC
void npc(int dim, int *x, int *y, char mapa[dim][dim], int *arm, int tecla, char person)
{
    if(person == '^')
    {
        if(mapa[*x - 1][*y] == 'N' && (tecla == 'i' || tecla == 'I'))
        {
            printf("\n=================================================\n");
            printf("Jorge: Ola, jovem aventureiro!\n");
            printf("Jorge: Eu sou Jorge, o ferreiro desta vila.\n");
            printf("Jorge: Ha muito tempo protejo estas terras.\n\n");

            printf("Jorge: Monstros tem aparecido alem dos portoes,\n");
            printf("      e muitos viajantes nao retornam.\n");
            printf("      Se pretende seguir viagem, voce vai precisar de uma arma.\n\n");

            printf("Jorge: Escolha com sabedoria:\n\n");

            printf("1 - Espada\n");
            printf("   Dano: 15\n");
            printf("   Uma arma confiavel para combate corpo a corpo.\n\n");

            printf("2 - Arco e Flecha\n");
            printf("   Dano: 10\n");
            printf("   Menos dano, mas permite atacar de longe.\n\n");

            printf("3 - Cajado\n");
            printf("   Dano: 20\n");
            printf("   Canaliza magia poderosa, mas exige coragem.\n\n");

            printf("Qual sera sua arma? ");
            scanf("%d", arm);
            printf("\n");

            if(*arm == 1)
            {
                printf("Jorge: Uma excelente escolha!\n");
                printf("Jorge: A espada acompanha os herois ha seculos.\n");
                printf("Jorge: Que sua lamina nunca perca o brilho.\n");
            }
            else if(*arm == 2)
            {
                printf("Jorge: Um arqueiro habilidoso vale por dez soldados.\n");
                printf("Jorge: Mire com calma e acerte seu alvo.\n");
            }
            else if(*arm == 3)
            {
                printf("Jorge: Interessante... poucos escolhem o caminho da magia.\n");
                printf("Jorge: Use este power com responsabilidade.\n");
            }
            else
            {
                printf("Jorge: Nao reconheco essa arma.\n");
                printf("Jorge: Volte quando souber o que deseja.\n");
                *arm = 0;
            }

            printf("\nJorge: Boa sorte em sua jornada, aventureiro!\n");
            printf("=================================================\n");
            
            printf("Pressione ENTER para continuar...");
            getchar(); 
            getchar(); 
        }
    }
}

// Funcao de configuracao da arma e dano
void armas(stts *p, int arm, int tecla)
{
    int dano = 0;

    if(arm == 1) strcpy(p->armas, "espada");
    if(arm == 2) strcpy(p->armas, "arco e flecha");
    if(arm == 3) strcpy(p->armas, "cajado");

    if(tecla == 'j')
    {
        if(strcmp(p->armas, "espada") == 0)
        {
            dano = 15;
            printf("Voce atacou com a espada!\n");
        }
        else if(strcmp(p->armas, "arco e flecha") == 0)
        {
            dano = 10;
            printf("Voce disparou uma flecha!\n");
        }
        else if(strcmp(p->armas, "cajado") == 0)
        {
            dano = 20;
            printf("Voce lancou uma magia!\n");
        }
        else
        {
            printf("Voce nao possui uma arma!\n");
            return;
        }

        printf("Dano causado: %d\n", dano);
    }
}

// Funcao de perseguicao do mob
void perseguir(mobs *mob, int px, int py, int dim, char mapa[dim][dim], int *vidas)
{
    if(mob->vivo == 0) return;

    if( (mob->x - 1 == px && mob->y == py) || 
        (mob->x + 1 == px && mob->y == py) || 
        (mob->x == px && mob->y - 1 == py) || 
        (mob->x == px && mob->y + 1 == py) )
    {
        (*vidas)--; 
        return;     
    }

    if(px < mob->x && mapa[mob->x - 1][mob->y] != '*')
        mob->x--;
    else if(px > mob->x && mapa[mob->x + 1][mob->y] != '*')
        mob->x++;
    else if(py < mob->y && mapa[mob->x][mob->y - 1] != '*')
        mob->y--;
    else if(py > mob->y && mapa[mob->x][mob->y + 1] != '*')
        mob->y++;
}

void destruir(char *obj)
{
    if(*obj == 'k' || *obj == 'Y' || *obj == 'Z')
    {
        *obj = ' ';
    }
}

// Função que valida o hit nos monstros
void checarAtaque(mobs *mob, int x, int y, char dir, int arm)
{
    if(mob->vivo == 0) return;

    int atingido = 0;

    if(arm == 1) 
    {
        if(dir == '^' && (mob->x == x-1 || mob->x == x-2) && (mob->y >= y-1 && mob->y <= y+1)) atingido = 1;
        else if(dir == 'v' && (mob->x == x+1 || mob->x == x+2) && (mob->y >= y-1 && mob->y <= y+1)) atingido = 1;
        else if(dir == '<' && (mob->x >= x-1 && mob->x <= x+1) && (mob->y == y-1 || mob->y == y-2)) atingido = 1;
        else if(dir == '>' && (mob->x >= x-1 && mob->x <= x+1) && (mob->y == y+1 || mob->y == y+2)) atingido = 1;
    }
    else if(arm == 2) 
    {
        if(dir == '^' && mob->y == y && mob->x >= x-4 && mob->x <= x-1) atingido = 1;
        else if(dir == 'v' && mob->y == y && mob->x >= x+1 && mob->x <= x+4) atingido = 1;
        else if(dir == '<' && mob->x == x && mob->y >= y-4 && mob->y <= y-1) atingido = 1;
        else if(dir == '>' && mob->x == x && mob->y >= y+1 && mob->y <= y+4) atingido = 1;
    }
    else if(arm == 3) 
    {
        if(mob->x >= x-1 && mob->x <= x+1 && mob->y >= y-1 && mob->y <= y+1)
        {
            if(!(mob->x == x && mob->y == y)) atingido = 1;
        }
    }

    if(atingido)
    {
        mob->vivo = 0; 
    }
}

// Funcao de ataque universal
void ataque(int dim, char mapa[dim][dim], int x, int y, char dir, int arm)
{
    if(arm == 1) 
    {
        if(dir == '^')
        {
            destruir(&mapa[x-1][y-1]); destruir(&mapa[x-1][y]); destruir(&mapa[x-1][y+1]);
            destruir(&mapa[x-2][y-1]); destruir(&mapa[x-2][y]); destruir(&mapa[x-2][y+1]);
        }
        else if(dir == 'v')
        {
            destruir(&mapa[x+1][y-1]); destruir(&mapa[x+1][y]); destruir(&mapa[x+1][y+1]);
            destruir(&mapa[x+2][y-1]); destruir(&mapa[x+2][y]); destruir(&mapa[x+2][y+1]);
        }
        else if(dir == '<')
        {
            destruir(&mapa[x-1][y-1]); destruir(&mapa[x][y-1]); destruir(&mapa[x+1][y-1]);
            destruir(&mapa[x-1][y-2]); destruir(&mapa[x][y-2]); destruir(&mapa[x+1][y-2]);
        }
        else if(dir == '>')
        {
            destruir(&mapa[x-1][y+1]); destruir(&mapa[x][y+1]); destruir(&mapa[x+1][y+1]);
            destruir(&mapa[x-1][y-2]); destruir(&mapa[x][y+2]); destruir(&mapa[x+1][y+2]);
        }
    }
    else if(arm == 2) 
    {
        if(dir == '^')
        {
            destruir(&mapa[x-1][y]); destruir(&mapa[x-2][y]); destruir(&mapa[x-3][y]); destruir(&mapa[x-4][y]);
        }
        else if(dir == 'v')
        {
            destruir(&mapa[x+1][y]); destruir(&mapa[x+2][y]); destruir(&mapa[x+3][y]); destruir(&mapa[x+4][y]);
        }
        else if(dir == '<')
        {
            destruir(&mapa[x][y-1]); destruir(&mapa[x][y-2]); destruir(&mapa[x][y-3]); destruir(&mapa[x][y-4]);
        }
        else if(dir == '>')
        {
            destruir(&mapa[x][y+1]); destruir(&mapa[x][y+2]); destruir(&mapa[x][y+3]); destruir(&mapa[x][y+4]);
        }
    }
    else if(arm == 3) 
    {
        destruir(&mapa[x-1][y-1]); destruir(&mapa[x-1][y]); destruir(&mapa[x-1][y+1]);
        destruir(&mapa[x][y-1]);                               destruir(&mapa[x][y+1]);
        destruir(&mapa[x+1][y-1]); destruir(&mapa[x+1][y]); destruir(&mapa[x+1][y+1]);
    }
}

// ======================================================
// HITBOXES VISUAIS
// ======================================================
void mostrarHitboxArco(int dim, char mapa[dim][dim], int x, int y, char dir)
{
    char copia[dim][dim];
    int i, j;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
            copia[i][j] = mapa[i][j];

    if(dir == '^')
    {
        copia[x-1][y] = 'z'; copia[x-2][y] = 'z'; copia[x-3][y] = 'z'; copia[x-4][y] = 'z';
    }
    else if(dir == 'v')
    {
        copia[x+1][y] = 'z'; copia[x+2][y] = 'z'; copia[x+3][y] = 'z'; copia[x+4][y] = 'z';
    }
    else if(dir == '<')
    {
        copia[x][y-1] = 'z'; copia[x][y-2] = 'z'; copia[x][y-3] = 'z'; copia[x][y-4] = 'z';
    }
    else if(dir == '>')
    {
        copia[x][y+1] = 'z'; copia[x][y+2] = 'z'; copia[x][y+3] = 'z'; copia[x][y+4] = 'z';
    }

    copia[x][y] = dir;

    system("clear || cls");
    drawing: desenho(dim, copia);

    #ifdef _WIN32
        system("timeout /t 1 > nul");
    #else
        usleep(300000);
    #endif
}

void mostrarHitboxEspada(int dim, char mapa[dim][dim], int x, int y, char dir)
{
    char copia[dim][dim];
    int i, j;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
            copia[i][j] = mapa[i][j];

    if(dir == '^')
    {
        copia[x-1][y-1] = 'z'; copia[x-1][y]   = 'z'; copia[x-1][y+1] = 'z';
        copia[x-2][y-1] = 'z'; copia[x-2][y]   = 'z'; copia[x-2][y+1] = 'z';
    }
    else if(dir == 'v')
    {
        copia[x+1][y-1] = 'z'; copia[x+1][y]   = 'z'; copia[x+1][y+1] = 'z';
        copia[x+2][y-1] = 'z'; copia[x+2][y]   = 'z'; copia[x+2][y+1] = 'z';
    }
    else if(dir == '<')
    {
        copia[x-1][y-1] = 'z'; copia[x][y-1]   = 'z'; copia[x+1][y-1] = 'z';
        copia[x-1][y-2] = 'z'; copia[x][y-2]   = 'z'; copia[x+1][y-2] = 'z';
    }
    else if(dir == '>')
    {
        copia[x-1][y+1] = 'z'; copia[x][y+1]   = 'z'; copia[x+1][y+1] = 'z';
        copia[x-1][y+2] = 'z'; copia[x][y+2]   = 'z'; copia[x+1][y+2] = 'z';
    }

    copia[x][y] = dir;

    system("clear || cls");
    desenho(dim, copia);

    #ifdef _WIN32
        system("timeout /t 1 > nul");
    #else
        usleep(300000);
    #endif
}

void mostrarHitboxCajado(int dim, char mapa[dim][dim], int x, int y, char dir)
{
    char copia[dim][dim];
    int i, j;

    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
            copia[i][j] = mapa[i][j];

    copia[x-1][y-1] = 'z'; copia[x-1][y] = 'z'; copia[x-1][y+1] = 'z';
    copia[x][y-1]   = 'z';                      copia[x][y+1]   = 'z';
    copia[x+1][y-1] = 'z'; copia[x+1][y] = 'z'; copia[x+1][y+1] = 'z';

    copia[x][y] = dir;

    system("clear || cls");
    desenho(dim, copia);

    #ifdef _WIN32
        system("timeout /t 1 > nul");
    #else
        usleep(300000);
    #endif
}

// ======================================================
// MAIN
// ======================================================

int main()
{
    int comando;
    
    printf("\n      Bem-vindo ao programa de tortura mental!\n");
    printf("\nEste programa ira testar sua paciencia e persistencia.\n");
    printf("\n*********************************************************\n");
    printf("\nDigite 1 >> start\n");
    printf("Digite 2 >> tutorial\n");
    printf("Digite 3 >> sair\n");
    printf("\n*********************************************************\n");

    scanf("%d", &comando);

    while(comando < 1 || comando > 3)
    {
        printf("Comando invalido! Digite 1 (Start), 2 (Tutorial) ou 3 (Sair):\n");
        scanf("%d", &comando);
    }

    if(comando == 1)
    {
        stts inf = {3, "nenhuma", 0};
        char person = '^';
        int x = 5, y = 5;
        
        mobs monstro1 = {3, 3, 1, 'X'}; 
        mobs monstro2 = {3, 4, 1, 'X'}; 
        mobs monstro3 = {11, 4, 1, 'X'}; 
        mobs monstro4 = {5, 11, 1, 'X'}; 
        mobs monstro5 = {15, 12, 1, 'X'}; 
        
        int arm = 0;
        int fase = 0;
        char tecla;
        char temp; 
        
        // SISTEMA DE PORTAS (0 = Trancada, 1 = Aberta)
        int porta1_aberta = 0;
        int porta2_aberta = 0;
        int porta3_aberta = 0;
        
        char temp_m2 = ' ', temp_m3 = ' '; 
        char temp_m4 = ' ', temp_m5 = ' '; 
        
        char vila[TAM][TAM] = {
            {'*','*','*','*','*','*','*','*','*','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ','N',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ','L','*'},
            {'*','*','*','*','*','*','*','*','*','*'}
        };

        char fase1[TAM1][TAM1] = {
            {'*','*','*','*','*','*','*','*','*','*'},
            {'*',' ',' ',' ','k',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ',' ','@',' ',' ',' ',' ','*'}, // Chave @ aqui
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ','*','*','*',' ',' ',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ',' ','D',' ',' ',' ',' ',' ','*'}, // Porta D aqui
            {'*',' ',' ',' ',' ',' ',' ','L',' ','*'},
            {'*','*','*','*','*','*','*','*','*','*'}
        };
        
        char fase2[TAM2][TAM2] = {
            {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},        
            {'*',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ',' ','*','*'},
            {'*',' ','#','#','#','#','#',' ','*',' ','#','#',' ','*','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ',' ','*','*'},
            {'*','*','*',' ','*','*','*','*','*',' ','*','*',' ','*','*'},
            {'*',' ','@',' ',' ',' ','O',' ',' ',' ','*',' ',' ','*','*'}, // Chave @ aqui
            {'*',' ','*','*','*','*','*',' ','*','*','*','*','*',' ','*'},
            {'*',' ',' ',' ','D',' ',' ',' ',' ',' ',' ',' ','@',' ','*'}, // Porta D e outra chave @
            {'*',' ','#','#','#','#','#','#','#',' ','#','#','#','#','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*',' ','*','*','*','*','*',' ','*','*','*','*','*',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ','D',' ','*'}, // Outra Porta D
            {'*',' ','#','#','#','#','#',' ','*',' ','#','#','#',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ','L',' ',' ',' ',' ',' ',' ','*'},
            {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'}
        };

        char fase3[TAM3][TAM3] = {
            {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
            {'*','@',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*'}, // Chave @ no início
            {'*',' ','#','#','#','#','#',' ','*','*','*','*','*','*','*','*','*',' ','#','#','#','#','#',' ','*'},
            {'*',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*','*','*',' ','*','*','*',' ','*',' ','#','#','#','#','#',' ','*',' ','*','*','*',' ','*','*','*'},
            {'*',' ',' ',' ','*',' ','*',' ','*',' ',' ',' ',' ',' ',' ',' ','*',' ','*',' ','*',' ',' ',' ','*'},
            {'*',' ','*','*','*',' ','*',' ','*','*','*','*',' ','*','*','*','*',' ','*',' ','*','*','*',' ','*'},
            {'*',' ','*',' ',' ',' ','*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ','*',' ','*'},
            {'*',' ','*',' ','*','*','*','*','*','*','*',' ','*','*','*','*','*','*','*','*','*',' ','*',' ','*'},
            {'*',' ',' ',' ','*',' ',' ',' ',' ',' ','*',' ','*',' ',' ',' ',' ',' ',' ',' ','*',' ',' ',' ','*'},
            {'*','*','*',' ','*',' ','#','#','#',' ','*',' ','*',' ','#','#','#',' ','*',' ','*',' ','*','*','*'},
            {'*',' Congressional',' ',' ','*',' ','#',' ','#',' ','*',' ','*',' ','#',' ','#',' ','*',' ','*',' ',' ',' ','*'},
            {'*',' ','*','*','*',' ','#',' ','#',' ','*',' ','*',' ','#',' ','#',' ','*',' ','*','*','*',' ','*'},
            {'*',' ',' ',' ',' ',' ','#','#','#',' ',' ',' ',' ',' ','#','#','#',' ',' ',' ',' ',' ',' ',' ','*'},
            {'*','*','*','*','*',' ',' ',' ',' ',' ','*','*','*','*','*',' ',' ',' ',' ',' ','*','*','*','*','*'},
            {'*',' ',' ',' ','*','*','*',' ','*','*','*',' ',' ',' ','*','*','*',' ','*','*','*',' ',' ',' ','*'},
            {'*',' ','#',' ','*',' ',' ',' ',' ',' ','*',' ','#',' ','*',' ',' ',' ',' ',' ','*',' ','#',' ','*'},
            {'*',' ','#',' ','*',' ','*','*','*',' ','*',' ','#',' ','*',' ','*','*','*',' ','*',' ','#',' ','*'},
            {'*',' ','#',' ',' ',' ','*',' ',' ',' ','*',' ','#',' ','*',' ',' ',' ','*',' ',' ',' ','#',' ','*'},
            {'*',' ','#','#','#',' ','*',' ','*','*','*',' ','#',' ','*','*','*',' ','*',' ','#','#','#',' ','*'},
            {'*',' ',' ',' ','#',' ','*',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ','*',' ','#',' ',' ',' ','*'},
            {'*','*','*',' ','#',' ','*','*','*','*','*','*','*','*','*','*','*','*','*',' ','#',' ','*','*','*'},
            {'*',' ',' ',' ','#',' ',' ',' ',' ',' ','D',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ','*'}, // Porta D antes da escada
            {'*',' ','#','#','#','#','#','#','#','#','#','#','L','#','#','#','#','#','#','#','#','#','#',' ','*'},
            {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'}
        };

        printf("\nM jogo comecou! Boa sorte!\n\n");

        while(1)
        {
            if(inf.vidas <= 0)
            {
                printf("\n====================================\n");
                printf("         GAME OVER! Você morreu.     \n");
                printf("====================================\n");
                break;
            }

            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif

            // RENDERIZAR FASES
            switch(fase)
            {
                case 0:
                    temp = vila[x][y];
                    vila[x][y] = person;
                    desenho(TAM, vila);
                    vila[x][y] = temp;
                    printf("\n==============\nSTATUS:\nVIDA: %d\nARMA: %s\n==============\n", inf.vidas, inf.armas);
                    break;

                case 1:
                    temp = fase1[x][y];
                    fase1[x][y] = person;
                    if(monstro1.vivo) fase1[monstro1.x][monstro1.y] = monstro1.simbolo; 
                    desenho(TAM1, fase1);
                    if(monstro1.vivo) fase1[monstro1.x][monstro1.y] = ' '; 
                    fase1[x][y] = temp; 
                    printf("\n==============\nSTATUS:\nVIDA: %d\nARMA: %s\nCHAVES: %d\n==============\n", inf.vidas, inf.armas, inf.chaves);
                    break;

                case 2:
                    temp = fase2[x][y];
                    fase2[x][y] = person;
                    
                    if(monstro2.vivo) {
                        temp_m2 = fase2[monstro2.x][monstro2.y];
                        fase2[monstro2.x][monstro2.y] = monstro2.simbolo;
                    }
                    if(monstro3.vivo) {
                        temp_m3 = fase2[monstro3.x][monstro3.y];
                        fase2[monstro3.x][monstro3.y] = monstro3.simbolo;
                    }
                    
                    desenho(TAM2, fase2);
                    
                    if(monstro2.vivo) fase2[monstro2.x][monstro2.y] = temp_m2;
                    if(monstro3.vivo) fase2[monstro3.x][monstro3.y] = temp_m3;
                    fase2[x][y] = temp;
                    
                    printf("\n==============\nSTATUS:\nVIDA: %d\nARMA: %s\nCHAVES: %d\n==============\n", inf.vidas, inf.armas, inf.chaves);
                    break;

                case 3:
                    temp = fase3[x][y];
                    fase3[x][y] = person;
                    
                    if(monstro4.vivo) {
                        temp_m4 = fase3[monstro4.x][monstro4.y];
                        fase3[monstro4.x][monstro4.y] = monstro4.simbolo;
                    }
                    if(monstro5.vivo) {
                        temp_m5 = fase3[monstro5.x][monstro5.y];
                        fase3[monstro5.x][monstro5.y] = monstro5.simbolo;
                    }
                    
                    desenho(TAM3, fase3);
                    
                    if(monstro4.vivo) fase3[monstro4.x][monstro4.y] = temp_m4;
                    if(monstro5.vivo) fase3[monstro5.x][monstro5.y] = temp_m5;
                    fase3[x][y] = temp;
                    
                    printf("\n==============\nSTATUS:\nVIDA: %d\nARMA: %s\nCHAVES: %d\n==============\n", inf.vidas, inf.armas, inf.chaves);
                    break;
            }

            printf("\nAcao (w,a,s,d: andar | i: interagir/abrir porta | o: ataque | q: sair): ");
            scanf(" %c", &tecla);

            if(tecla == 'q') {
                break;
            }

            // AÇÕES E LÓGICA DAS FASES
            switch(fase)
            {
                case 0:
                    npc(TAM, &x, &y, vila, &arm, tecla, person);
                    armas(&inf, arm, tecla);
                    mov(TAM, tecla, &x, &y, &person, vila);
                    
                    if(tecla == 'o') 
                    {
                        if(arm == 1) mostrarHitboxEspada(TAM, vila, x, y, person);
                        else if(arm == 2) mostrarHitboxArco(TAM, vila, x, y, person);
                        else if(arm == 3) mostrarHitboxCajado(TAM, vila, x, y, person);
                        ataque(TAM, vila, x, y, person, arm);
                    }
                    if(vila[x][y] == 'L' && tecla == 'i') { fase = 1; x = 8; y = 1; }
                    break;

                case 1:
                    mov(TAM1, tecla, &x, &y, &person, fase1);
                    
                    // COLETAR CHAVE @
                    if(fase1[x][y] == '@') {
                        inf.chaves++;
                        fase1[x][y] = ' ';
                    }

                    // ABRIR PORTA D COM INTERAÇÃO 'i'
                    if(tecla == 'i') {
                        if(fase1[x-1][y] == 'D' || fase1[x+1][y] == 'D' || fase1[x][y-1] == 'D' || fase1[x][y+1] == 'D') {
                            if(inf.chaves > 0) {
                                inf.chaves--;
                                porta1_aberta = 1;
                                if(fase1[x-1][y] == 'D') fase1[x-1][y] = ' ';
                                if(fase1[x+1][y] == 'D') fase1[x+1][y] = ' ';
                                if(fase1[x][y-1] == 'D') fase1[x][y-1] = ' ';
                                if(fase1[x][y+1] == 'D') fase1[x][y+1] = ' ';
                                printf("\nPorta aberta com sucesso!\n");
                                #ifdef _WIN32
                                    Sleep(1000);
                                #else
                                    usleep(1000000);
                                #endif
                            } else {
                                printf("\nVoce nao tem uma chave (@) para abrir a porta!\n");
                                #ifdef _WIN32
                                    Sleep(1000);
                                #else
                                    usleep(1000000);
                                #endif
                            }
                        }
                    }

                    if(tecla == 'o')
                    {
                        if(arm == 1) mostrarHitboxEspada(TAM1, fase1, x, y, person);
                        else if(arm == 2) mostrarHitboxArco(TAM1, fase1, x, y, person);
                        else if(arm == 3) mostrarHitboxCajado(TAM1, fase1, x, y, person);
                        ataque(TAM1, fase1, x, y, person, arm);
                        checarAtaque(&monstro1, x, y, person, arm);
                    }
                    perseguir(&monstro1, x, y, TAM1, fase1, &inf.vidas); 
                    
                    if(fase1[x][y] == 'L' && tecla == 'i') { fase = 0; x = 8; y = 8; }
                    
                    // TESTA SE PODE SUBIR A ESCADA (Tecla 'm' ou 'i')
                    else if(fase1[x][y] == 'L' && tecla == 'm') { 
                        if(porta1_aberta) { 
                            fase = 2; x = 13; y = 6; 
                        } else {
                            printf("\nA porta está trancada! Voce nao pode subir a escada.\n");
                            #ifdef _WIN32
                                Sleep(1500);
                            #else
                                usleep(1500000);
                            #endif
                        }
                    }
                    break;

                case 2:
                    mov(TAM2, tecla, &x, &y, &person, fase2);

                    // COLETAR CHAVE @
                    if(fase2[x][y] == '@') {
                        inf.chaves++;
                        fase2[x][y] = ' ';
                    }

                    // ABRIR PORTA D COM INTERAÇÃO 'i'
                    if(tecla == 'i') {
                        if(fase2[x-1][y] == 'D' || fase2[x+1][y] == 'D' || fase2[x][y-1] == 'D' || fase2[x][y+1] == 'D') {
                            if(inf.chaves > 0) {
                                inf.chaves--;
                                porta2_aberta = 1;
                                if(fase2[x-1][y] == 'D') fase2[x-1][y] = ' ';
                                if(fase2[x+1][y] == 'D') fase2[x+1][y] = ' ';
                                if(fase2[x][y-1] == 'D') fase2[x][y-1] = ' ';
                                if(fase2[x][y+1] == 'D') fase2[x][y+1] = ' ';
                                printf("\nPorta aberta com sucesso!\n");
                                #ifdef _WIN32
                                    Sleep(1000);
                                #else
                                    usleep(1000000);
                                #endif
                            } else {
                                printf("\nVoce nao tem uma chave (@) para abrir a porta!\n");
                                #ifdef _WIN32
                                    Sleep(1000);
                                #else
                                    usleep(1000000);
                                #endif
                            }
                        }
                    }

                    if(fase2[x][y] == '#')
                    {
                        inf.vidas--; 
                    }

                    if(tecla == 'o') 
                    {
                        if(arm == 1) mostrarHitboxEspada(TAM2, fase2, x, y, person);
                        else if(arm == 2) mostrarHitboxArco(TAM2, fase2, x, y, person);
                        else if(arm == 3) mostrarHitboxCajado(TAM2, fase2, x, y, person);
                        ataque(TAM2, fase2, x, y, person, arm);
                        checarAtaque(&monstro2, x, y, person, arm);
                        checarAtaque(&monstro3, x, y, person, arm);
                    }

                    perseguir(&monstro2, x, y, TAM2, fase2, &inf.vidas);
                    perseguir(&monstro3, x, y, TAM2, fase2, &inf.vidas);

                    if(fase2[x][y] == 'L' && tecla == 'i') { fase = 1; x = 8; y = 7; }
                    
                    // TESTA SE PODE SUBIR A ESCADA PARA O 3º ANDAR
                    else if(fase2[x][y] == 'L' && tecla == 'm') { 
                        if(porta2_aberta) { 
                            fase = 3; x = 22; y = 12; 
                        } else {
                            printf("\nA porta está trancada! Voce nao pode subir a escada.\n");
                            #ifdef _WIN32
                                Sleep(1500);
                            #else
                                usleep(1500000);
                            #endif
                        }
                    } 
                    break;

                case 3:
                    mov(TAM3, tecla, &x, &y, &person, fase3);

                    // COLETAR CHAVE @
                    if(fase3[x][y] == '@') {
                        inf.chaves++;
                        fase3[x][y] = ' ';
                    }

                    // ABRIR PORTA D COM INTERAÇÃO 'i'
                    if(tecla == 'i') {
                        if(fase3[x-1][y] == 'D' || fase3[x+1][y] == 'D' || fase3[x][y-1] == 'D' || fase3[x][y+1] == 'D') {
                            if(inf.chaves > 0) {
                                inf.chaves--;
                                porta3_aberta = 1;
                                if(fase3[x-1][y] == 'D') fase3[x-1][y] = ' ';
                                if(fase3[x+1][y] == 'D') fase3[x+1][y] = ' ';
                                if(fase3[x][y-1] == 'D') fase3[x][y-1] = ' ';
                                if(fase3[x][y+1] == 'D') fase3[x][y+1] = ' ';
                                printf("\nPorta aberta com sucesso!\n");
                                #ifdef _WIN32
                                    Sleep(1000);
                                #else
                                    usleep(1000000);
                                #endif
                            } else {
                                printf("\nVoce nao tem uma chave (@) para abrir a porta!\n");
                                #ifdef _WIN32
                                    Sleep(1000);
                                #else
                                    usleep(1000000);
                                #endif
                            }
                        }
                    }

                    if(fase3[x][y] == '#')
                    {
                        inf.vidas--; 
                    }

                    if(tecla == 'o') 
                    {
                        if(arm == 1) mostrarHitboxEspada(TAM3, fase3, x, y, person);
                        else if(arm == 2) mostrarHitboxArco(TAM3, fase3, x, y, person);
                        else if(arm == 3) mostrarHitboxCajado(TAM3, fase3, x, y, person);
                        ataque(TAM3, fase3, x, y, person, arm);
                        checarAtaque(&monstro4, x, y, person, arm);
                        checarAtaque(&monstro5, x, y, person, arm);
                    }

                    perseguir(&monstro4, x, y, TAM3, fase3, &inf.vidas);
                    perseguir(&monstro5, x, y, TAM3, fase3, &inf.vidas);

                    // PARTE ADICIONADA: Voltar para a fase 2 descendo as escadas
                    if(fase3[x][y] == 'L' && tecla == 'i') { 
                        fase = 2; 
                        x = 13; 
                        y = 7; 
                    }
                    
                    // Condição de Vitória: Chegar ao final da fase 3 (posição da última chave @ no canto superior esquerdo)
                    if(x == 1 && y == 1) {
                        printf("\n=================================================\n");
                        printf(" ¡FELICIDADES! ¡Has completado el juego mental! \n");
                        printf("=================================================\n");
                        #ifdef _WIN32
                            Sleep(3000);
                        #else
                            usleep(3000000);
                        #endif
                        return 0; // Termina el programa ganando
                    }
                    break;
            } // Fim do switch(fase) de ações
        } // Fim do while(1) do jogo
    } // Fim do if(comando == 1)
    else if(comando == 2)
    {
        // PARTE ADICIONADA: Lógica do Tutorial
        printf("\n=================== TUTORIAL ===================\n");
        printf("Controles:\n");
        printf("  w, a, s, d -> Movimentar o personagem (^, <, v, >)\n");
        printf("  i          -> Interagir com NPCs / Abrir portas / Descer escadas\n");
        printf("  m          -> Subir escadas\n");
        printf("  o          -> Atacar com a arma equipada\n");
        printf("  q          -> Sair do jogo\n");
        printf("\nElementos do mapa:\n");
        printf("  * -> Parede (intransponivel)\n");
        printf("  # -> Espinhos (causam dano)\n");
        printf("  D -> Porta trancada\n");
        printf("  @ -> Chave\n");
        printf("  L -> Escada\n");
        printf("  X -> Monstro\n");
        printf("================================================\n\n");
        printf("Pressione ENTER para voltar ao menu principal...");
        getchar(); // Limpar buffer
        getchar(); // Esperar entrada
        main();    // Reinicia o menu principal
    }
    else if(comando == 3)
    {
        // PARTE ADICIONADA: Opção de Sair
        printf("\nSaindo do programa... Obrigado por jogar!\n");
    }

    return 0;
}
