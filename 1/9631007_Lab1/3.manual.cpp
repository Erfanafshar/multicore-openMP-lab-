
#include <stdio.h>
#include <math.h>
#include <omp.h>

const long int VERYBIG = 50000;
const int m_thread = 16;
// ***********************************************************************
int main(void)
{
#ifndef _OPENMP
	printf("OpenMP is not supported, sorry!\n");
	getchar();
	return 0;
#endif
	int i;
	long int j, k;
	double sumx, sumy;
	double starttime, elapsedtime;

	int sum[m_thread];
	double total[m_thread];
	long int shared_sum;
	double shared_total;
	// -----------------------------------------------------------------------
	// Output a start message
	printf("Serial Timings for %d iterations\n\n", VERYBIG);
	// repeat experiment several times
	for (i = 0; i<10; i++)
	{
		// get starting time56 x CHAPTER 3 PARALLEL STUDIO XE FOR THE IMPATIENT
		starttime = omp_get_wtime();
		// reset check sum & running total
		for (j = 0; j < m_thread; j++)
		{
			sum[j] = 0;
			total[j] = 0.0;
		}

		shared_sum = 0;
		shared_total = 0.0;
	
		// Work Loop, do some work by looping VERYBIG times
#pragma omp parallel for private(k, sumx, sumy) 
		for (j = 0; j < VERYBIG; j++)
		{
			// increment check sum
			int id = omp_get_thread_num();

			sum[id] += 1;

			// Calculate first arithmetic series
			sumx = 0.0;
			for (k = 0; k<j; k++)
				sumx = sumx + (double)k;
			// Calculate second arithmetic series
			sumy = 0.0;
			for (k = j; k>0; k--)
				sumy = sumy + (double)k;

			if (sumx > 0.0)total[id] = total[id] + 1.0 / sqrt(sumx);

			if (sumy > 0.0)total[id] = total[id] + 1.0 / sqrt(sumy);
		}

		for (j = 0; j < m_thread; j++)
		{
			shared_sum += sum[j];
			shared_total += total[j];
		}

		// get ending time and use it to determine elapsed time
		elapsedtime = omp_get_wtime() - starttime;
		// report elapsed time
		printf("Time Elapsed: %f Secs, Total = %lf, Check Sum = %ld\n",
			elapsedtime, shared_total, shared_sum);
	}
	// return integer as required by function header
	getchar();
	return 0;
}
