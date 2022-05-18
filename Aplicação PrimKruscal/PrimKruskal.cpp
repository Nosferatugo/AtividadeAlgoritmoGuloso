#include <iostream>	
#include <stdlib.h>	
#include <time.h>	
#include <stdio.h>	    
#include <iomanip>		
#include <climits>		

using namespace std;

// A estrutura do v�rtice consiste em seu id
// ID dos pais e um custo (para Prim)
struct vertex 
{
	int id;
	int cost; // isso � usado como altura no algoritmo kruskal
	int parentId;
};

// Estrutura de arestas para indicar os ids dos v�rtices
// que compartilham uma aresta e seu peso.
struct edge 
{
	int vertex_id_1;
	int vertex_id_2;
	int weight;
};

// retorno: 1 para Prim, 2 para Kruskal
// Solicita que o usu�rio fa�a a entrada
int promptUser ();

// param 1: matriz de adjac�ncia
// param 2: tamanho de uma �nica dimens�o
// Execute o algoritmo MST de Prim
void prim (int ** matrix, int size);

// Fun��es utilit�rias para o MST do Prim
void sort(vertex priorityQueue[], int size);
bool _include(vertex priorityQueue[], int vertexToFind, int sizeOfQueue);
int getCost(vertex queue[], int vertexId, int size);
void setCost(vertex queue[], int vertexId, int weight, int size);
void setParentId(vertex queue[], int vertexId, int idToSet, int size);
void le_matriz_adj( int*** madj, int* num_nos, char* arquivo );
void mostra_matriz_adj( int** mat, int nnos );

// param 1: Matriz de Adjacencia
// param 2: tamanho de uma �nica dimens�o
// Execute o algoritmo MST de Kruskal
void kruskal (int ** matrix, int size);

// Fun��es utilit�rias para o MST do Kruskal
int find3(vertex set[], int i);
void union3(vertex set[], int i, int j);
void sortEdges(edge edges[], int size);

// Ponto de partida do programa
int main (){
	
	//alterar para arquivo desejado dij10, dij20, dij40 ou dij50
	char arquivo[] = "dij10.txt";
	int** madj;
	int num_nos;
	
		le_matriz_adj( &madj, &num_nos, arquivo);	
	    mostra_matriz_adj( madj, num_nos );
	
	
		
	srand(time(NULL));
//	num_nos = (rand() % 6) + 5;
	cout << "\n Numero de Vertices e: " << num_nos << "\n";

	//int adjacencyMatrix[n][n];
	int i, j;



	// Para passar um array 2d de tamanho vari�vel para uma fun��o como par�metro
	int * matrix[num_nos];
	for (i = 0; i < num_nos; ++i) matrix[i] = madj[i];

	// O usu�rio � solicitado a escolher um algoritmo MST
	int algo = promptUser();

	if (algo == 1) prim(matrix, num_nos);
	else if (algo == 2) kruskal(matrix, num_nos);
}

// param 1: Matriz de Adjacencia
// param 2: tamanho de uma �nica dimens�o
// Execute o algoritmo MST de Prim
void prim(int ** matrix, int size)
{
	int i, edge_weight, queueSize;
	vertex priorityQueue[size];

	// Cria v�rtices e preenche priorityQueus
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

	// Faz um loop at� que a fila priorit�ria esteja vazia
	while (queueSize > 0)
	{
		// Pega o elemento m�nimo e o remove
		vertex v = priorityQueue[queueSize - 1];
		queueSize--;

		// Percorrer todas as suas arestas
		for (i = 0; i < size; i++)
		{
			// N�o percorra a si mesmo
			if (i != v.id)
			{
				edge_weight = matrix[i][v.id];

				// Se tivermos este v�rtice e seu peso for maior que o �ltimo peso calculado
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
// param 2: tamanho de uma �nica dimens�o
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

	// Preenche o array set que cont�m todos os v�rtices
	for (i = 0; i < size; i++)
	{
		vertex v;
		v.id = i;
		v.cost = 0;
		v.parentId = i;
		set[i] = v;
	}

	// Preenche o array de arestas que cont�m todas as arestas
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

	// Loops at� que todas as arestas sejam atravessadas
	for (i = 0; i < totalEdges; i++)
	{
		// Obtenha a borda mais curta
		edge shortest_edge = edges[i];

		// Encontre as ra�zes de ambos os lados da aresta
		u = find3(set, shortest_edge.vertex_id_1);
		v = find3(set, shortest_edge.vertex_id_2); 

		// Se as ra�zes n�o forem iguais, adicione-o ao resultado e
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

// Dada uma matriz que cont�m arestas, ordena-as 
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

// Dado um conjunto e �ndice, encontra a raiz e compacta at� a raiz
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

// Mescla 2 conjuntos disjuntos usando a propriedade height dos v�rtices
void union3(vertex set[], int i, int j)
{
	int root_id_x = find3(set, i);
	int root_id_y = find3(set, j);

	// Compare os n�veis de definir o pai do menor para o maior
	// Se ambos forem iguais arbitrariamente, defina um para o pai do outro e aumente o n�vel
	if (set[root_id_x].cost < set[root_id_y].cost) set[root_id_x].parentId = root_id_y;
	else if (set[root_id_x].cost > set[root_id_y].cost) set[root_id_y].parentId = root_id_x;
	else
	{
		set[root_id_y].parentId = root_id_x;
		set[root_id_x].cost++;
	}
}

// Fun��o getter para o campo de custo da estrutura de v�rtices
int getCost(vertex queue[], int vertexId, int size)
{
	for (int i = 0; i < size; i++) if (queue[i].id == vertexId) return queue[i].cost;

	return -1;
}

// Fun��o setter para o campo de custo da estrutura de v�rtices
void setCost(vertex queue[], int vertexId, int weight, int size)
{
	for (int i = 0; i < size; i++) if (queue[i].id == vertexId) queue[i].cost = weight;
}

// Fun��o setter para o campo parentId da estrutura do v�rtice
void setParentId(vertex queue[], int vertexId, int idToSet, int size)
{
	for (int i = 0; i < size; i++) if (queue[i].id == vertexId) queue[i].parentId = idToSet;
}

// Dada uma fila de prioridade retorna true se o v�rtice de destino for encontrado
bool _include(vertex priorityQueue[], int vertexToFind, int sizeOfQueue)
{
	for (int i = 0; i < sizeOfQueue; i++) if (priorityQueue[i].id == vertexToFind) return true;

	return false;
}

// Mantenha a estrutura da fila de prioridade classificando os v�rtices de acordo
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
// Solicita que o usu�rio fa�a a entrada
int promptUser()
{
	int answer;
	cout << "\n Digite 1 para Prim, 2 para Kruskal: ";
	cin >> answer;

	if (answer != 1 && answer != 2) cout << " O algoritmo selecionado n�o foi encontrado.\n\n";
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


