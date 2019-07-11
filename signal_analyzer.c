#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "signal_analyzer.h"

/*typedef struct PeakList
{
	int index;
	double value;
	struct PeakList *next;
} PeakList;
*/
void destroy_peaklist(PeakList *peaklist)
{
	PeakList *pp = peaklist;
	while (pp != NULL)
	{
		peaklist = pp->next;
		free(pp);
		pp = peaklist;
	}
}

void add_peak_to_peaklist(PeakList **p_peaklist, PeakList *peak)
{
	peak->next = NULL;
	if (NULL == (*p_peaklist))
	{
		*p_peaklist = peak;
	}
	else
	{
		PeakList *pp = (*p_peaklist);
		while(NULL != pp->next)
		{
			pp = pp->next;
		}
		pp->next = peak;
	}
}

PeakList *create_peak(int index, double value)
{
	PeakList *new_peak = (PeakList*)calloc(1, sizeof(*new_peak));
	new_peak->index = index;
	new_peak->value = value;

	return new_peak;
}

void find_peaks_above_threshold(double *signal, int signal_length, double threshold, 
		PeakList **p_peaklist)
{
	int i;
	bool above_threshold = false;
	bool below_threshold = false;

	double maxval = 0;
	int maxindex = 0;

	for (i = 0; i < signal_length; i++)
	{
		if (signal[i] < threshold)
		{
			if (above_threshold) // Detect transition
			{
				PeakList *new_peak = create_peak(maxindex, maxval);
				add_peak_to_peaklist(p_peaklist, new_peak);
			}
			above_threshold = false;
			below_threshold = true;
		}
		else if (signal[i] >= threshold)
		{
			if (below_threshold) // Detect transition
			{
				// 
				maxval = signal[i];
				maxindex = i;
			}
			if (signal[i] > maxval)
			{
				maxval = signal[i];
				maxindex = i;
			}

			above_threshold = true;
			below_threshold = false;
		}
	}	
}

double calculate_average(double *signal, int signal_length)
{
	double signal_sum = 0;
	int i;

	for (i = 0; i < signal_length; i++)
	{
		signal_sum += signal[i];
	}

	/* May need to consider doing this division on every sample to avoid overflow */
	signal_sum /= (double)signal_length;

	return signal_sum;
}

void print_peaks(PeakList *peaklist)
{
	int peak_count = 0;
	PeakList *pp = peaklist;
	while (NULL != pp)
	{
		peak_count++;
		printf("Found peak %f at index %d\n", pp->value, pp->index);
		pp = pp->next;
	}
	printf("%d peaks found.\n", peak_count);
}

void test_peak_finder_case_1(void)
{
	double signal[13] = { 1, 2, 3, 4, 3, 2, 1, 2, 3, 4, 3, 2, 1 };
	int signal_length = 13;
	double threshold = 2.0;
	PeakList *peaklist = NULL;
	PeakList *pp = NULL;
	int peak_count = 0;

	find_peaks_above_threshold(signal, signal_length, threshold, &peaklist);

	print_peaks(peaklist);
	destroy_peaklist(peaklist);
}

void test_peak_finder_case_2(void)
{
	double signal[13] = { 1, 2, 3, 4, 3, 2, 1, 2, 3, 4, 3, 2, 1 };
	int signal_length = 13;
	double threshold = calculate_average(signal, signal_length);
	PeakList *peaklist = NULL;
	PeakList *pp = NULL;
	int peak_count = 0;

	printf("Using threshold: average value of %f\n", threshold);
	find_peaks_above_threshold(signal, signal_length, threshold, &peaklist);

	print_peaks(peaklist);
	destroy_peaklist(peaklist);
}

int main()
{
	test_peak_finder_case_1();
	printf("CASE 2:\n");
	test_peak_finder_case_2();

	return 0;
}
