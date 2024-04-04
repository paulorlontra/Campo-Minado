#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#ifdef WIN32
#include <windows.h>
#endif

#define ANSI_COLOR_RED     	"\x1b[31m" 
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_PURPLE  "\x1b[35m"
#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_CYAN  "\x1b[36m"

typedef struct{
    int mina, minasvizinhas, estado, flag;
} info;

info tabuleiro[18][32];

int linhas, colunas, abertos;

int valido(int x, int y) {
    if (x > linhas || y > colunas || x < 0 || y < 0) {
        printf("\nCoordenada inválida, a linha deve estar entre 1 e %d, e a coluna deve estar entre 1 e %d!\n", linhas, colunas);
        return 0;
    }

    if (tabuleiro[x][y].estado == 1) {
        printf("\nPosição já jogada, selecione outra posição!\n");
        return 0;
    }

    return 1;

}

void textoGanhou(){
	FILE *arq;
	char ganhou[128];
	
	if((arq=fopen("ganhou.txt", "r"))==0){
		puts("Erro");
	}else{
		while(fgets(ganhou, sizeof(ganhou), arq)!=NULL){
			printf("%s", ganhou);
		}
	}
}

void textoPerdeu(){
	FILE *arq;
	char perdeu[128];
	
	if((arq=fopen("perdeu.txt", "r"))==0){
		puts("Erro");
	}else{
		while(fgets(perdeu, sizeof(perdeu), arq)!=NULL){
			printf("%s", perdeu);
		}
	}
}


void textoTitulo(){
	FILE *arq;
	char titulo[128];
	
	if((arq=fopen("titulo.txt", "r"))==0){
		puts("Erro");
	}else{
		while(fgets(titulo, sizeof(titulo), arq)!=NULL){
			printf("%s", titulo);
        }
	}
}


void iniciaMinas(int cont){
   int x, y;
   srand(time(NULL));

   while(1){
        if(cont<1) break;
        x=rand()%linhas;
        y=rand()%colunas;
        x++;
        y++;
        if(!(tabuleiro[x][y].mina)){
            tabuleiro[x][y].mina=1;
            cont--;
        }
    }
}

void contaMinas(int row, int col){
    int i, j;

    for(i=-1; i<=1; i++){
        for(j=-1; j<=1; j++){
            int x=row+i;
            int y=col+j;
            if(x>=1 && x<=linhas && y>=1 && y<=colunas && tabuleiro[x][y].mina){
                tabuleiro[row][col].minasvizinhas++;
            }
        }
    }
}

void iniciaTabuleiro(int cont){
    int i, j;

    iniciaMinas(cont);

    for(i=1; i<=linhas; i++){
        for(j=1; j<=colunas; j++){
            if(!(tabuleiro[i][j].mina)){
                contaMinas(i, j);
            }
        }
    }
}

void limpaTabuleiro(){
    int i, j;

    for(i = 1; i <=linhas; i++){
        for(j = 1; j <=colunas; j++){
            tabuleiro[i][j].mina = 0;
            tabuleiro[i][j].estado = 0;
            tabuleiro[i][j].minasvizinhas = 0;
            tabuleiro[i][j].flag = 0;
        }
    }


}

void percorreTabuleiro(int row, int col){
    tabuleiro[row][col].estado=1;
    abertos++;

    if(!(tabuleiro[row][col].minasvizinhas)){
        int i, j;
        for(i=-1; i<=1; i++){
            for(j=-1; j<=1; j++){
                int x=row+i;
                int y=col+j;
                if(x>=1 && x<=linhas && y>=1 && y<=colunas && !(tabuleiro[x][y].mina) && !(tabuleiro[x][y].estado)){
                    percorreTabuleiro(x, y);
                }
            }
        }
    }
}

void imprimirTabuleiro(int cont, int difi){
    int i, j;

    switch(difi){
        case 1:
            printf("==================== Campo Minado ====================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("               Dificuldade: Fácil\n\n\n");
            break;
        case 2:
            printf("=========================== Campo Minado ===========================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                             Dificuldade: Médio\n\n\n");
            break;
        case 3:
            printf("====================================================== Campo Minado =======================================================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                                                                                  Dificuldade: Difícil\n\n\n");
            break;
        default:
            break;
    }

    printf("  ");
    for(i=1; i<=colunas; i++){
        if(i<=9) printf("   %d", i);
        else printf("  %d", i);
        if(i==colunas) printf("\n");
    }
    printf("   |");

    for(i=1; i<=colunas; i++){
        if(i==colunas){
            printf("---|");
        }else{
            printf("---+");
        }
    }
    printf("\n");

    for(i=1; i<=linhas; i++){
        printf("%d ", i);
        for(j=1; j<=colunas; j++){
            if(i<=9 && j==1) printf(" ");
            if(tabuleiro[i][j].flag){
                printf("|");
                printf(ANSI_COLOR_RED " ! " ANSI_COLOR_RESET);
            }else if(tabuleiro[i][j].estado){
                if(tabuleiro[i][j].minasvizinhas==1){
                    printf("|");
                    printf(ANSI_COLOR_CYAN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas==2){
                    printf("|");
                    printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas>2){
                    printf("|");
                    printf(ANSI_COLOR_PURPLE " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else{
                    printf("|   ");
                }
            }else if(!tabuleiro[i][j].estado){
                printf("| - ");
            }
        }

        printf("|\n");
        printf("   |");
        for(j=1; j<=colunas; j++){
            if(j==colunas){
                printf("---|");
            }else{
                printf("---+");
            }
        }
        printf("\n");
    }
}

int ganhou(int cont){
    if(abertos==linhas*colunas-cont) return 1;
    return 0;
}

void perdeuTabuleiro(int cont, int difi, int x, int y){
    int i, j;

    switch(difi){
        case 1:
            printf("==================== Campo Minado ====================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("               Dificuldade: Fácil\n\n\n");
            break;
        case 2:
            printf("=========================== Campo Minado ===========================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                             Dificuldade: Médio\n\n\n");
            break;
        case 3:
            printf("====================================================== Campo Minado =======================================================\n\n");
            if(cont<=9) printf("Flags Disponíveis: %d ", cont);
            else printf("Flags Disponíveis: %d", cont);
            printf("                                                                                  Dificuldade: Difícil\n\n\n");
            break;
        default:
            break;
    }

    printf("  ");
    for(i=1; i<=colunas; i++){
        if(i<=9) printf("   %d", i);
        else printf("  %d", i);
        if(i==colunas) printf("\n");
    }
    printf("   |");

    for(i=1; i<=colunas; i++){
        if(i==colunas){
            printf("---|");
        }else{
            printf("---+");
        }
    }
    printf("\n");

    for(i=1; i<=linhas; i++){
        printf("%d ", i);
        for(j=1; j<=colunas; j++){
            if(i<=9 && j==1) printf(" ");
            if(tabuleiro[i][j].mina){
                printf("|");
                if(i==x && j==y){
                    printf(ANSI_COLOR_YELLOW " * " ANSI_COLOR_RESET);
                }else{
                    printf(ANSI_COLOR_RED " * " ANSI_COLOR_RESET);
                }
            }else if(tabuleiro[i][j].estado){
                if(tabuleiro[i][j].minasvizinhas==1){
                    printf("|");
                    printf(ANSI_COLOR_CYAN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas==2){
                    printf("|");
                    printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas>2){
                    printf("|");
                    printf(ANSI_COLOR_PURPLE " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else{
                    printf("|   ");
                }
            }else if(!tabuleiro[i][j].estado){
                printf("| - ");
            }
        }

        printf("|\n");
        printf("   |");
        for(j=1; j<=colunas; j++){
            if(j==colunas){
                printf("---|");
            }else{
                printf("---+");
            }
        }
        printf("\n");
    }
}

void ganhouTabuleiro(int difi){
    int i, j;

    switch(difi){
        case 1:
            printf("==================== Campo Minado ====================\n\n");
            printf("Flags Disponíveis: %d ", 0);
            printf("               Dificuldade: Fácil\n\n\n");
            break;
        case 2:
            printf("=========================== Campo Minado ===========================\n\n");
            printf("Flags Disponíveis: %d ", 0);
            printf("                             Dificuldade: Médio\n\n\n");
            break;
        case 3:
            printf("====================================================== Campo Minado =======================================================\n\n");
            printf("Flags Disponíveis: %d ", 0);
            printf("                                                                                  Dificuldade: Difícil\n\n\n");
            break;
        default:
            break;
    }

    printf("  ");
    for(i=1; i<=colunas; i++){
        if(i<=9) printf("   %d", i);
        else printf("  %d", i);
        if(i==colunas) printf("\n");
    }
    printf("   |");
    for(i=1; i<=colunas; i++){
        if(i==colunas){
            printf("---|");
        }else{
            printf("---+");
        }
    }
    printf("\n");

    for(i=1; i<=linhas; i++){
        printf("%d ", i);
        for(j=1; j<=colunas; j++){
            if(i<=9 && j==1) printf(" ");
            if(tabuleiro[i][j].mina){
                printf("|");
                printf(ANSI_COLOR_RED " * " ANSI_COLOR_RESET);
            }else if(tabuleiro[i][j].estado){
                if(tabuleiro[i][j].minasvizinhas==1){
                    printf("|");
                    printf(ANSI_COLOR_CYAN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas==2){
                    printf("|");
                    printf(ANSI_COLOR_GREEN " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else if(tabuleiro[i][j].minasvizinhas>2){
                    printf("|");
                    printf(ANSI_COLOR_PURPLE " %d " ANSI_COLOR_RESET, tabuleiro[i][j].minasvizinhas);
                }else{
                    printf("|   ");
                }
            }else if(!tabuleiro[i][j].estado){
                printf("| - ");
            }
        }

        printf("|\n");
        printf("   |");
        for(j=1; j<=colunas; j++){
            if(j==colunas){
                printf("---|");
            }else{
                printf("---+");
            }
        }
        printf("\n");
    }
}

void tempo(int x, int y){
    int min=x/60;
    int seg=x-(min*60);
    if(y){
        if(x<60){
            printf("\n\nVocê venceu o Campo Minado em %d segundo(s).\n", seg);
        }else{
            printf("\n\nVocê venceu o Campo Minado em %d minuto(s) e %d segundo(s).\n", min, seg);
        }
    }else{
        if(x<60){
            printf("\n\nO tempo da partida foi de %d segundo(s).\n", seg);
        }else{
            printf("\n\nO tempo da partida foi de %d minuto(s) e %d segundo(s).\n", min, seg);
        }
    }
}

int main(){

    setlocale(LC_ALL, "Portuguese");
    
    time_t comeco, fim;
    int x, y, sa;
    int dificuldade, jogada, primeira=1;
    int qntFlags, minas;
    char jogar = 's', ctz;
    
    while(jogar == 's' || jogar == 'S'){
        
        #ifdef _WIN32
        	system("cls");
    	#else 
        	system("clear");
    	#endif

        textoTitulo();
        printf("\nBem vindo ao famoso jogo \"Campo Minado\", as dificuldades disponíveis são:\n\n1 - Fácil (tabuleiro 9x9 / 10 minas)\n2 - Médio (tabuleiro 16x16 / 40 minas)\n3 - Difícil (tabuleiro 16x30 / 99 minas)\n\n0 - Sair o jogo\n\nInsira o comando desejado: ");

        while(scanf("%d", &dificuldade)==0 || dificuldade < 0 || dificuldade > 3){
            fflush(stdin);
            printf("\n\nComando inválido, selecione um número entre 0 e 3: ");
        }

        if(!dificuldade){
            return 0;
        }

        switch(dificuldade){
            case 1:
                linhas=9, colunas=9, minas=qntFlags=10;
                break;
            case 2:
                linhas=16, colunas=16, minas=qntFlags=40;
                break;
            case 3:
                linhas=16, colunas=30, minas=qntFlags=99;
                break;
            default:
                break;
        }

        primeira=1;
        abertos=0;

        limpaTabuleiro();
        iniciaTabuleiro(minas);

        while(1){
            
            #ifdef _WIN32
                system("cls");
            #else 
                system("clear");
            #endif

            imprimirTabuleiro(qntFlags, dificuldade);

            printf("\n\n");

            printf("\nJogadas:\n\n1 - Escavar\n2 - Inserir flag de mina\n3 - Remover flag de mina\n\n0 - Voltar ao Menu Principal\n\nInsira o comando desejado: ");

            while(scanf("%d", &jogada)==0 || jogada<0 || jogada>3){
                fflush(stdin);
                printf("\nComando inválido, selecione um número entre 0 e 3: ");
            }

            if(!jogada){
                getchar();
                printf("\nTem certeza que deseja voltar ao Menu Principal?(S/N): ");
                scanf("%c", &ctz);
                getchar();

                while(ctz!='S' && ctz!='s' && ctz!='N' && ctz!='n'){
                    printf("\nComando inválido, selecione as letras solicitadas: ");
                    scanf("%c", &ctz);
                    getchar();
                }

                if(ctz=='S' || ctz=='s') break;
                else continue;
            }

            printf("\nInsira a coordenada \"x y\" da sua jogada, caso queira cancelar a jogada insira a coordenada \"0 0\": ");
           
            while(!(sa=scanf("%d %d", &x, &y)) || !(valido(x, y))){
                fflush(stdin);

                if(!sa){
                    printf("\n\nCoordenada inválida, a linha deve estar entre 1 e %d, e a coluna deve estar entre 1 e %d!\n", linhas, colunas);
                }

                printf("\nInsira a coordenada \"x y\" da sua jogada, caso queira cancelar a jogada insira a coordenada \"0 0\": ");
            }

            if(x==0 && y==0) continue;

            if(jogada==1){
                if(primeira){
                    comeco = time(NULL);

                    while(tabuleiro[x][y].mina){
                        limpaTabuleiro();
                        iniciaTabuleiro(minas);
                    }

                    primeira=0;
                }
                if(tabuleiro[x][y].mina==1){
                    
                    #ifdef _WIN32
                    system("cls");
                    #else 
                        system("clear");
                    #endif

                    perdeuTabuleiro(qntFlags, dificuldade, x, y);

                    fim = time(NULL);

                    textoPerdeu();
                    tempo(fim-comeco, 0);
                    getchar();
                    printf("\nVocê deseja jogar novamente?(S/N): ");
                    scanf("%c", &jogar);
                    getchar();

                    while(jogar!='S' && jogar!='s' && jogar!='N' && jogar!='n'){
                        printf("\nComando inválido, selecione as letras solicitadas: ");
                        scanf("%c", &jogar);
                        getchar();
                    }

                    break;
                    
                }else{
                    percorreTabuleiro(x, y);
                }
            }

            if(jogada==2){
                if(qntFlags==0){
                    printf("\nNão há mais flags disponíveis, remova uma flag para acrescentar em outro lugar!\n\nEnter para continuar >>");
                    fflush(stdin);
                    getc(stdin);
                }else{
                    if((tabuleiro[x][y].flag)){
                        printf("\nJogada inválida, já existe uma flag nessa posição!\n\nEnter para continuar >>");
                        fflush(stdin);
                        getc(stdin);
                    }else{
                        tabuleiro[x][y].flag=1;
                        qntFlags--;
                    }
                }
            }

            if(jogada==3){
                if(qntFlags==minas){
                    printf("\nNão existem flags para serem removidas, acrescente uma flag primeiro, para depois retirá-la!\n\nEnter para continuar >>");
                    fflush(stdin);
                    getc(stdin);
                }else{
                    if(!(tabuleiro[x][y].flag)){
                        printf("\nNão existe uma flag nessa posição, acrescente uma flag primeiro, para depois retirá-la!\n\nEnter para continuar >>");
                        fflush(stdin);
                        getc(stdin);
                    }else{
                        tabuleiro[x][y].flag=0;
                        qntFlags++;
                    }
                }
            }
            if(ganhou(minas)){
                
                #ifdef _WIN32
                system("cls");
                #else 
                    system("clear");
                #endif

                ganhouTabuleiro(dificuldade);

                fim = time(NULL);

                textoGanhou();
                tempo(fim-comeco, 1);
                getchar();
                printf("\n\nVocê deseja jogar novamente?(S/N): ");
                scanf("%c", &jogar);
                getchar();

                while(jogar!='S' && jogar!='s' && jogar!='N' && jogar!='n'){
                    printf("\nComando inválido, selecione as letras solicitadas: ");
                    scanf("%c", &jogar);
                }
                break;
            }
        }
    }
    return 0;
}