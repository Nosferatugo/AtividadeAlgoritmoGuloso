#include <iostream>	
#include <stdlib.h>	
#include <time.h>	
#include <stdio.h>	    
#include <iomanip>		
#include <climits>
#include <limits.h>

#define INF 9999

#define V 9	


using namespace std;

void dijkstra(int graph[V][V], int src);


int minDistance(int dist[], bool sptSet[]){
	
	int min = INT_MAX, min_index;
	
	for (int v = 0; v < V; v++)
	if (sptSet[v] == false && dist[v] <= min)
	min = dist[v], min_index = v;
	
	return min_index;
}

void printSolution (int dist[]){
	cout << "Vertex \t Distance from Source" << endl;
	for (int i = 0; i < V; i++)
	cout << i << "\t\t"<< dist[i]<< endl;
}	

/*
void dijkstra(int graph[V][V], int src){
	int dist[V];
	
	bool sptSet[V];
	for (int i = 0; i < V; i++)
	dist[i] = INT_MAX, sptSet[i] = false;
	
	dist[src] = 0;

for (int count = 0; count < V - 1; count++){
	int u = minDistance(dist, sptSet);
	
	sptSet[u] = true;
	
	for (int v = 0; v < V; v++)
	
	if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v])
	dist[v] = dist[u] + graph[u][v];
	
	printSolution(dist);
}
	
}
*/

void dijkstra (int ** G, int ordem, int s, int Custo[], int Rota[]){
	
	int r = -1;
	
	bool Visitado [ordem];
	
	//Inicializa os Vetores
	for ( int i = 0;i< ordem; i++)
		Visitado[i]=false;
	for (int i = 0; i< ordem; i++){
	
		Custo[i] = INF;
		Rota[i] = -1;
		
	}
	Custo[s] = 0;
	Rota[s] = s;
	
	//Loop Principal do algoritmo
	while  (true){
		r = -1;
		
		for (int i = 0; i < ordem; i++){
			if (!Visitado[i] && (r < 0 || Custo[i] < Custo[r]))
			r = i;
		}
		
		if (r < 0) break;
		
		Visitado[r] = true;
		
		for (int i = 0; i < ordem; i++){
			if (G[r][i] && Custo[i] > Custo[r] + G[r][i]){
				Custo[i] = Custo[r] + G[r][i];
				Rota[i] = r;
			}	
		
		}
		
	}
	
}

// A estrutura do vértice consiste em seu id
// ID dos pais e um custo (para Prim)
struct vertex 
{
	int id;
	int cost; // isso é usado como altura no algoritmo kruskal
	int parentId;
};

// Estrutura de arestas para indicar os ids dos vértices
// que compartilham uma aresta e seu peso.
struct edge 
{
	int vertex_id_1;
	int vertex_id_2;
	int weight;
};

// Funções utilitárias para o MST do Prim
void sort(vertex priorityQueue[], int size);
bool _include(vertex priorityQueue[], int vertexToFind, int sizeOfQueue);
int getCost(vertex queue[], int vertexId, int size);
void setCost(vertex queue[], int vertexId, int weight, int size);
void setParentId(vertex queue[], int vertexId, int idToSet, int size);
void le_matriz_adj( int*** madj, int* num_nos, char* arquivo );
void mostra_matriz_adj( int** mat, int nnos );


// Funções utilitárias para o MST do Kruskal
int find3(vertex set[], int i);
void union3(vertex set[], int i, int j);
void sortEdges(edge edges[], int size);

// Ponto de partida do programa
int main (){
	
	//alterar para arquivo desejado dij10, dij20, dij40 ou dij50
	char arquivo[] = "dij10.txt";
	int** madj;
	int num_nos;
	
	int graph[V][V];
	
		le_matriz_adj( &madj, &num_nos, arquivo);	
	    mostra_matriz_adj( madj, num_nos );	
		
		int no = 0;
		int custos[num_nos];
		int rota[num_nos];
		
		dijkstra(madj, num_nos, no, custos, rota);
		
		
		
		cout <<"O Menor Caminho e: " << custos[num_nos - 1] << endl;		
	
		
	srand(time(NULL));
	cout << "\n Numero de Vertices e: " << num_nos << "\n";

	//int adjacencyMatrix[n][n];
	int i;

	// Para passar um array 2d de tamanho variável para uma função como parâmetro
	int * matrix[num_nos];
	for (i = 0; i < num_nos; ++i) 
	matrix[i] = madj[i];
	
	//dijkstra(madj);
	
	
	
}

// param 1: Matriz de Adjacencia
// param 2: tamanho de uma única dimensão
// Execute o algoritmo MST de Prim
void prim(int ** matrix, int size)
{
	int i, edge_weight, queueSize;
	vertex priorityQueue[size];

	// Cria vértices e preenche priorityQueus
	for (i = 0; i < size; i++)
	{
		vertex v;
		v.parentId = -1;
		v.id = i;
		v.cost = INT_MAX;
		priorityQueue[i] = v;
	}

	queueSize = size;
	priorityQueue[size - 1].cost = 0;

	// Faz um loop até que a fila prioritária esteja vazia
	while (queueSize > 0)
	{
		// Pega o elemento mínimo e o remove
		vertex v = priorityQueue[queueSize - 1];
		queueSize--;

		// Percorrer todas as suas arestas
		for (i = 0; i < size; i++)
		{
			// Não percorra a si mesmo
			if (i != v.id)
			{
				edge_weight = matrix[i][v.id];

				// Se tivermos este vértice e seu peso for maior que o último peso calculado
				// Em seguida, defina o novo peso para ele, altere seu pai e mantenha a estrutura de prioridade
				// classificando novamente
				if (_include(priorityQueue, i, queueSize) &&
					getCost(priorityQueue, i, queueSize) > edge_weight)
				{
					setCost(priorityQueue, i, edge_weight, queueSize);
					setParentId(priorityQueue, i, v.id, queueSize);
					sort(priorityQueue, queueSize);
				}
			}
		}
	}

	cout << "\nTodas as arestas no MST por Prim:\n\n";
	int custo = 0;
	for (i = 0; i < size - 1; i++)
	{
		cout << " V(";
		cout << priorityQueue[i].id + 1 << ") --- V(";
		cout << priorityQueue[i].parentId + 1 << ")  Custa: ";
		cout << priorityQueue[i].cost << "\n";
		custo += priorityQueue[i].cost;
	}

	cout << endl;
	cout << "Resultado=" << custo;

	cout << "\n";
}

// param 1: Matriz de Adjacencia
// param 2: tamanho de uma única dimensão
// Execute o algoritmo MST de Kruskal
void kruskal(int ** matrix, int size)
{
	int i, j, u, v;
	int mst_counter = 0;
	int edge_counter = 0;
	int totalEdges = size * (size - 1) / 2;

	vertex set[size];
	edge edges[totalEdges];
	edge edges_for_MST[size - 1];

	// Preenche o array set que contém todos os vértices
	for (i = 0; i < size; i++)
	{
		vertex v;
		v.id = i;
		v.cost = 0;
		v.parentId = i;
		set[i] = v;
	}

	// Preenche o array de arestas que contém todas as arestas
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (j > i)
			{
				edge e;
				e.vertex_id_1 = i;
				e.vertex_id_2 = j;
				e.weight = matrix[i][j];

				edges[edge_counter] = e;
				edge_counter++;
			}
		}
	}

	// Bordas classificadas com peso crescente
	sortEdges(edges, totalEdges);

	// Loops até que todas as arestas sejam atravessadas
	for (i = 0; i < totalEdges; i++)
	{
		// Obtenha a borda mais curta
		edge shortest_edge = edges[i];

		// Encontre as raízes de ambos os lados da aresta
		u = find3(set, shortest_edge.vertex_id_1);
		v = find3(set, shortest_edge.vertex_id_2); 

		// Se as raízes não forem iguais, adicione-o ao resultado e
		// mesclar 2 conjuntos disjuntos diferentes
		if (u != v)
		{
			edges_for_MST[mst_counter] = shortest_edge;
			mst_counter++;
			union3(set, u, v);
		} 
	}

	cout << "\n Todas as arestas no MST por Kruskal:\n\n";	
	int peso = 0;
	for (i = 0; i < size - 1; i++)
	{
		cout << " V(";
		cout << edges_for_MST[i].vertex_id_1 + 1 << ") --- V(";
		cout << edges_for_MST[i].vertex_id_2 + 1 << ")  Custa: ";
		cout << edges_for_MST[i].weight << "\n";
		peso += edges_for_MST[i].weight;
	}	
		
	cout << endl;
	cout << "Resultado=" << peso;
	
	cout << "\n";
}

// Dada uma matriz que contém arestas, ordena-as 
// de acordo com o aumento do peso
void sortEdges(edge edges[], int size)
{
	int i, j;

	// Insertion sort
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (edges[i].weight < edges[j].weight)
			{
				edge e = edges[i];
				edges[i] = edges[j];
				edges[j] = e;
			}
		}
	}
}

// Dado um conjunto e índice, encontra a raiz e compacta até a raiz
int find3(vertex set[], int i)
{
	int root = set[i].parentId;
	int target = i, pId;

	// Encontrar raiz da floresta
	while (root != set[i].id)
	{
		root = set[root].parentId; 
		i--;
	}

	// Comprimir o caminho de i para a raiz
	while (target != root)
	{
		pId = set[target].parentId;
		set[target].parentId = root;
		target = pId;
	}

	return root;
}

// Mescla 2 conjuntos disjuntos usando a propriedade height dos vértices
void union3(vertex set[], int i, int j)
{
	int root_id_x = find3(set, i);
	int root_id_y = find3(set, j);

	// Compare os níveis de definir o pai do menor para o maior
	// Se ambos forem iguais arbitrariamente, defina um para o pai do outro e aumente o nível
	if (set[root_id_x].cost < set[root_id_y].cost) set[root_id_x].parentId = root_id_y;
	else if (set[root_id_x].cost > set[root_id_y].cost) set[root_id_y].parentId = root_id_x;
	else
	{
		set[root_id_y].parentId = root_id_x;
		set[root_id_x].cost++;
	}
}

// Função getter para o campo de custo da estrutura de vértices
int getCost(vertex queue[], int vertexId, int size)
{
	for (int i = 0; i < size; i++) if (queue[i].id == vertexId) return queue[i].cost;

	return -1;
}

// Função setter para o campo de custo da estrutura de vértices
void setCost(vertex queue[], int vertexId, int weight, int size)
{
	for (int i = 0; i < size; i++) if (queue[i].id == vertexId) queue[i].cost = weight;
}

// Função setter para o campo parentId da estrutura do vértice
void setParentId(vertex queue[], int vertexId, int idToSet, int size)
{
	for (int i = 0; i < size; i++) if (queue[i].id == vertexId) queue[i].parentId = idToSet;
}

// Dada uma fila de prioridade retorna true se o vértice de destino for encontrado
bool _include(vertex priorityQueue[], int vertexToFind, int sizeOfQueue)
{
	for (int i = 0; i < sizeOfQueue; i++) if (priorityQueue[i].id == vertexToFind) return true;

	return false;
}

// Mantenha a estrutura da fila de prioridade classificando os vértices de acordo
// para diminuir os valores de custo
void sort(vertex priorityQueue[], int size)
{
	int i, j;

	// Insertion sort
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (priorityQueue[i].cost > priorityQueue[j].cost)
			{
				vertex v = priorityQueue[i];
				priorityQueue[i] = priorityQueue[j];
				priorityQueue[j] = v;
			}
		}
	}
}

// retorno: 1 para Prim, 2 para Kruskal
// Solicita que o usuário faça a entrada
int promptUser()
{
	int answer;
	cout << "\n Digite 1 para Prim, 2 para Kruskal: ";
	cin >> answer;

	if (answer != 1 && answer != 2) cout << " O algoritmo selecionado não foi encontrado.\n\n";
	return answer;
}

//cria matriz de adjacencia com arquivo
void le_matriz_adj( int*** madj, int* num_nos, char* arquivo ) {	
	FILE *f;
	int i, j, dist;
	char linha[ 1024 ];
				
	f = fopen( arquivo, "r" );
	
	if ( f == NULL ) {
		printf( "Erro na leitura do arquivo" );
		exit( 1 );
	}
	
	fgets( linha, sizeof( linha ), f ); 
	
	*num_nos = atoi( linha );
		
	*madj = (int**) malloc( (*num_nos) * sizeof( int* ) );		
	for( i = 0; i < *num_nos; i++ )
		(*madj)[i] = (int*)malloc( (*num_nos) * sizeof( int ) );	
	
	for( i = 0; i < *num_nos; i++ ) {
		for( j = 0; j < i; j++ ) 
			(*madj)[i][j] = (*madj)[j][i];
		for( j = i; j < *num_nos; j++ ) {
			if ( i == j ) {
				(*madj)[i][j] = 0;
				continue;
			}
			
			fscanf( f, "%d ", &dist );
						
			(*madj)[i][j] = dist;
		}
	}	
	
	fclose( f );	
}

// exibir matriz de adjacencia
void mostra_matriz_adj( int** mat, int nnos ) {
	int i, j;
	
	printf( "Matriz de adjacencias" );
	printf( "\n" );
	for( i = 0; i < nnos; i++ ) {
		for( j = 0; j < nnos; j++ ) {
			printf( "%10d", mat[i][j] );	
		}
		printf( "\n" );
	}
}


