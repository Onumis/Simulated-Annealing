
/*
	Room assign problem

	using simulated annealing - swap if "temperature" allows
*/

#include "main.h"


// n is the problem size (number of students to match)
int alg2(int n, int * D, int * a_solution, int process_rank)
{
	//printf("Room Assignment w/ Simulated annealing. N=%d\n", n);

	// copy the initial solution
	int * a = (int*) malloc( sizeof(int)*n );
	copyArray(a_solution, a, n);

	int tries = 5*n;
	int it=0;
	double temperature=1.0;
	int new_a, new_b, new_ap, new_bp;
	int * tmp = (int*) malloc( sizeof(int)*n );

	// attempt to get a better solution
	while(tries)
	{
		// get two random students to swap
		new_a = getRand(0,n);
		new_b = getRand(0,n);
		
		// save the matching partners
		new_ap = a[new_a];
		new_bp = a[new_b];

		// if they arent the same and they arent together already
		if( (new_a != new_b) && (new_ap != new_b) )
		{
			// d[a,b] has the compatibility number, d[b,a] has -1, just pick the highest
			// oldcost( <a, ap> + <b, bp> ) 
			int oldcost = 
				MAX(D[(new_a*n) + new_ap], D[(new_ap*n) + new_a]) +
				MAX(D[(new_b*n) + new_bp], D[(new_bp*n) + new_b]); 
			// newcost ( <b, ap> + <a, bp> )
			int newcost =
				MAX(D[(new_b*n) + new_ap], D[(new_ap*n) + new_b]) +
				MAX(D[(new_a*n) + new_bp], D[(new_bp*n) + new_a]);

			int delta = newcost-oldcost;
			
			double expo = exp( (double) (-delta)/temperature); 
			double u = ((double)rand()/(RAND_MAX));

			if (delta < 0 || expo >= u )
			{
				tries = 5*n;
				// swap the room mates
				a[new_a] = new_bp;
				a[new_b] = new_ap;
				a[new_ap] = new_b;
				a[new_bp] = new_a;

			}
			else
			{
				tries--;
			
			}

			//printf("2\t%d\t%d\t%d\t%.3f\t%.3f\n", it, n, cost(D, a, n ), temperature, expo );
			// a little hack to print only what i want
			//if(n==98)printf("iter\t%d\t%d\n", it, cost(D, a, n ) );

			// keep it cool
			temperature*=0.999;
			it++;

		}
	}

	//printMatrix(D, n);
	//printf("\n");
	//printArray(a, n);
	//printf("%d\t%d\t%d\t%d\n", process_rank, n, cost(D, a, n ), it );
	return cost(D, a, n );
}

