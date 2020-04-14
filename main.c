#define _POSIX_C_SOURCE 199309L
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <argp.h>

const char *argp_program_version =
        "benchmark  0.1";
/* Program documentation. */
static char doc[] =
        "I wonder for what this is";

static struct argp_option options[] = {
        {"verbose",  'v', 0,      0,  "Produce verbose output" },
        {"quiet",    'q', 0,      0,  "Don't produce any output" },
        {"iterations",   'i', "NUMBER", 0, "Number of iterations" },
        {"output",   'o', "file", 0, "output to file instead of stdout" },
        {"reps",   'r', "NUMBER", 0, "number of repetions in each iteration" },
        {"operation",   'O', "name", 0, "operation e.g. mul, add. ..." },
        {"numThreads",   'n', "NUMBER", 0, "specifies number of threads to be used. Default is num of logical cpus" },
        { 0 }
};
struct arguments
{
    int silent, verbose, numThreads;
    int use_output_file;
    char *output_file;
    char *operation;
    size_t iterations, reps_per_iteration;
};
/* parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    /* get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'q': case 's':
            arguments->silent = 1;
            break;
        case 'v':
            arguments->verbose = 1;
            break;
        case 'o':
            arguments->output_file = arg;
            arguments->use_output_file = 1;
            break;
        case 'O':
            arguments->operation = arg;
            break;
        case 'n':
            arguments->numThreads = strtol(arg,NULL,10);
            break;
	case 'i':
	    arguments->iterations = strtol(arg,NULL,10);//todo error handling
	    break;
	case 'r': 
	    arguments->reps_per_iteration = strtol(arg,NULL,10);//todo error handling
	    break;
        case ARGP_KEY_ARG:
            argp_usage (state);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* our argp parser. */
static struct argp argp = { options, parse_opt, 0 , doc };
//parsing args set up end---------------------------------------------------------------

clock_t ticks, new_ticks;
struct timespec t1, t2;

void timespec_diff(struct timespec *start, struct timespec *stop,
                   struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}


int main(int argc, char **argv) {
    //parsing args----------------------------------------------------------------
    struct arguments arguments;

    /* default values. */
    arguments.silent = 0;
    arguments.verbose = 0;
    arguments.reps_per_iteration = 100;
    arguments.iterations = 100;
    arguments.output_file = "-";
    arguments.operation = "mul";
    arguments.numThreads = 1;

    argp_parse (&argp, argc, argv, 0, 0, &arguments);
    //parsing args end---------------------------------------------------------------
    struct timespec * t_times = malloc(arguments.iterations  * sizeof(struct timespec));
    uint64_t * nanos = malloc(arguments.iterations * sizeof(uint64_t));
    float f2 = (float) arguments.iterations + 1.1; //avoid compiler optimization, because iterations is unknown for compiler
    float f1 = 1.1f;
    //iterate
    for (size_t j = 0; j < arguments.iterations ; j++){
        f2 = (float) arguments.iterations + 0.1 + j;
        clock_gettime(CLOCK_MONOTONIC, &t1);
        //printf("time: %i\t", t1.tv_nsec);
        for (size_t i = 0 ; i < arguments.reps_per_iteration; i++){
            f1 = f2 * 1.1f;
        }
        clock_gettime(CLOCK_MONOTONIC, &t2);
        struct timespec c;
        timespec_diff(&t1,&t2,&c);
        t_times[j] = c;
        nanos[j] = c.tv_sec * 1000000000 + c.tv_nsec;
    }

    //calculate mean and variance
    uint64_t mean = 0;
    for ( size_t i = 0 ; i < arguments.iterations; i++){
        mean += nanos[i];
        printf("dif=%ld sec %ld nsec \t nanos= %ld\n", t_times[i].tv_sec , t_times[i].tv_nsec, nanos[i]);

    }
    mean = mean / arguments.iterations;

    //variance
    uint64_t variance = 0;
    for ( size_t i = 0 ; i < arguments.iterations; i++){
        variance += (mean - nanos[i]) * (mean - nanos[i]);
        //printf("%ld ** 2 = %ld\t",(mean - nanos[i]),(mean - nanos[i]) * (mean - nanos[i]));
    }
    variance = variance / arguments.iterations;
    double std_derivation = sqrt(variance);
    double relvar = (double) variance / (double) mean ;
    double gflop =( double ) arguments.reps_per_iteration / (double) mean  ; // flops per nanosecond = Gflops
    double vgfkop = (double) relvar * gflop;
    double std_variation_gflop = sqrt(vgfkop);
    printf("-----------------------------\n");
    printf("raw:\n");
    printf("mean: %ld\tstd_variation: %f\tvar: %ld\trel var: %f\n", mean,std_derivation, variance, relvar);
    printf("\ngflops:\n");
    printf("mean: %f\tstd_variation: %f\tvar: %f\trel var: %f\n", gflop,std_variation_gflop, vgfkop, relvar);


}

