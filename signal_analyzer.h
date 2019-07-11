
typedef struct PeakList
{
	int index;
	double value;
	struct PeakList *next;
} PeakList;

void destroy_peaklist(PeakList *peaklist);

void print_peaks(PeakList *peaklist);

double calculate_average(double *signal, int signal_length);

void find_peaks_above_threshold(double *signal, int signal_length, double threshold, PeakList **p_peaklist);

