#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int n = 0, m = 0;
int visitedCells = 0; // Should be n*m by the end of maze generation

typedef struct cell *cellptr;

typedef struct cell{
    int x_coord, y_coord;
    cellptr neighbours[4]; // [0] for left, [1] for right, [2] for top, [3] for bottom
    cellptr next;
    cellptr prev;
    int visited; // 1 if visited, 0 if not visited
}cell;

// Defining a stack containing the visited cells (will be used later for backtracking)
cellptr stack[10000];
int top = 0;

void push(cellptr element) {stack[top++] = element;}
cellptr pop(){
    top--;
    if(top > 0) return stack[top];
    else return NULL;
}
// Definitions for stack ended

typedef struct queue *queueptr;

// Defining a queue (will be used later in BFS)
typedef struct queue {
    cellptr front;
    cellptr rear;
} queue;

void enqueue(queueptr q, cellptr element) {
    if(q->front == NULL){
        q->front = element;
        q->rear = element;
        element->next = NULL;
    }
    else{
        q->rear->next = element;
        q->rear = element;
        element->next = NULL;
    }
}

cellptr dequeue(queueptr q){
    if(q->front == NULL) return NULL;
    cellptr front = q->front;
    q->front = q->front->next;
    return front;
}
// Definitions of queue ended

void initialiseMaze(cellptr maze);
void connectCells(cellptr maze);
void displayMaze(cellptr maze);
int getRandom(int min, int max);
void solveMaze(cellptr maze, cellptr start, cellptr end);

int main(){
    printf("Please provide the dimensions of the maze you desire to create (in nxm format) : "); // n is the height and m is the width
    char str[10];
    scanf("%s", str);
    int count = 0;
    int mid;
    for(int i = strlen(str)-1; i >= 0; i--){
        if(str[i] == 'x'){
            mid = i;
            count++;
            continue;
        }
        if(count == 0){
            int pow = 1;
            for(int j = 0; j < strlen(str)-i-1; j++) pow *= 10;
            m += pow*(str[i]-'0');
        }
        else if(count == 1){
            int pow = 1;
            for(int j = 0; j < mid-i-1; j++) pow *= 10;
            n += pow*(str[i]-'0');
        }
    }
    srand(time(NULL));
    cell *maze = malloc(n*m*sizeof(cell));
    initialiseMaze(maze);
    connectCells(maze);
    displayMaze(maze);
    printf("\n-------------------------------------------------------------\n");
    printf("To find the solution to the above maze, type 1: ");
    int alpha;
    scanf("%d", &alpha);
    if(alpha == 1){
        printf("Solving the maze assuming the source and destination to be first and last cell respectively:\n");
        cellptr start = maze;
        cellptr end = maze + n * m - 1;
        solveMaze(maze, start, end);
        cellptr current = start;
        while (current != end) {
            printf("(%d, %d) ", current->x_coord+1, current->y_coord+1);
            current = current->next;
        }
        printf("(%d, %d) ", current->x_coord+1, current->y_coord+1);
        printf("\n");
    }
    free(maze);
    return 0;
}

void initialiseMaze(cellptr maze){
    cellptr curr = maze;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            curr->x_coord = j;
            curr->y_coord = i;
            curr->visited = 0;
            curr->next = NULL;
            curr->prev = NULL;
            curr++;
        }
    }
    curr = maze;
    for(int i = 0; i < n*m; i++){
        // Defining all neighbours
        if(curr->x_coord > 0) curr->neighbours[0] = curr-1;
        else curr->neighbours[0] = NULL;
        if(curr->x_coord < m-1) curr->neighbours[1] = curr+1;
        else curr->neighbours[1] = NULL;
        if(curr->y_coord > 0) curr->neighbours[2] = curr-m;
        else curr->neighbours[2] = NULL;
        if(curr->y_coord < n-1) curr->neighbours[3] = curr+m;
        else curr->neighbours[3] = NULL;
        curr++;
    }
}

void connectCells(cellptr maze){
    cellptr curr = maze;
    if(curr->visited == 0) curr->visited = 1;
    push(curr);
    visitedCells++;

    while(visitedCells != n*m){

        // Connecting with any unvisited neighbour
        cellptr unvisited[4];
        int unvisitedCount = 0;
        for(int i = 0; i < 4; i++) unvisited[i] = NULL;
        for(int i = 0; i < 4; i++){
            if(curr->neighbours[i] != NULL && curr->neighbours[i]->visited == 0){
                unvisited[unvisitedCount] = curr->neighbours[i];
                unvisitedCount++;
            }
        }
        if(unvisitedCount != 0){
            int connect = getRandom(0, unvisitedCount-1);
            curr->next = unvisited[connect];
            curr->next->prev = curr;
            curr = curr->next;
            if(curr->visited == 0) curr->visited = 1;
            push(curr);
            visitedCells++;
        }
        else{
            cellptr current = curr;
            curr=pop();
            if(curr == current) pop();
        }
    }
}

void displayMaze(cellptr maze){
    cellptr curr = maze;
    for(int i = 0; i < 2*m+1; i++) printf("#");
    printf("\n");
    for(int i = 1; i < 2*n; i++){
        if(i%2 == 1){
            cellptr current = curr;
            for(int j = 0; j < 2*m+1; j++){
                if(j == 0 || j == 2*m){
                    printf("#");
                    if(j == 0) curr++;
                }
                else{
                    if(j%2 == 1) printf(" ");
                    else{
                        if((curr->neighbours[0]->next == curr || curr->next == curr->neighbours[0]) || (curr->neighbours[0]->prev == curr || curr->prev == curr->neighbours[0])) printf(" ");
                        else printf("#");
                        curr++;
                    }
                }
            }
            if(i != 2*n-1) curr = current;
        }
        else{
            for(int j = 0; j < 2*m+1; j++){
                if(j%2 == 0) printf("#");
                else{
                    if((curr->neighbours[3]->next == curr || curr->next == curr->neighbours[3]) || (curr->neighbours[3]->prev == curr || curr->prev == curr->neighbours[3])) printf(" ");
                    else printf("#");
                    curr++;
                }
            }
        }
        printf("\n");
    }
    for(int i = 0; i < 2*m+1; i++) printf("#");
}

int getRandom(int min, int max) {return (rand()%(max-min+1)+min);}

void solveMaze(cellptr maze, cellptr start, cellptr end){
    queue q;
    q.front = NULL;
    q.rear = NULL;

    cellptr current = maze;
    for(int i = 0; i < n * m; i++) maze[i].visited = 0;

    enqueue(&q, start);
    start->visited = 1;

    while(q.front != NULL){
        current = dequeue(&q);
        if (current == end) return;
        for (int i = 0; i < 4; i++){
            if(
                current->neighbours[i] != NULL && current->neighbours[i]->visited == 0 &&
                (current->next == current->neighbours[i] || current->neighbours[i]->next == current ||
                current->prev == current->neighbours[i] || current->neighbours[i]->prev == current)
            ){
                enqueue(&q, current->neighbours[i]);
                current->neighbours[i]->visited = 1;
                current->neighbours[i]->prev = current;
                current->next = current->neighbours[i];
            }
        }
    }
}
