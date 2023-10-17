/***
 * This code includes the necessary header files for standard input/output,
 *  standard library functions, and getopt.h, which contains functions for parsing command-line options.
*/
#if 0 // Chatgpt example
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>  // Its a standard GNU library

int main(int argc, char *argv[]) {
    int option;
    int long_index = 0;

    // Define the long options
    struct option long_options[] = {
        {"input", required_argument, NULL, 'i'},
        {"output", required_argument, NULL, 'o'},
        {"verbose", no_argument, NULL, 'v'},
        {0, 0, 0, 0} // Termination option
    };

    // Initialize option variables
    char *input_file = NULL;
    char *output_file = NULL;
    int verbose_flag = 0;

    while ((option = getopt_long(argc, argv, "i:o:v", long_options, &long_index)) != -1) {
        switch (option) {
            case 'i':
                input_file = optarg;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'v':
                verbose_flag = 1;
                break;
            case '?':
                // Invalid option or missing argument
                printf("Unknown option: %c\n", optopt);
                break;
            default:
                // Unexpected
                abort();
        }
    }

    // Process the parsed options
    if (input_file != NULL)
        printf("Input file: %s\n", input_file);

    if (output_file != NULL)
        printf("Output file: %s\n", output_file);

    if (verbose_flag)
        printf("Verbose mode is enabled.\n");

    return 0;
}
#endif

#if 1 //Linux book example
#include <getopt.h> //standard GNU library for parsing options
#include <stdio.h>
#include <stdlib.h>

/* The name of this program. */
const char* program_name;
/* Prints usage information for this program to STREAM (typically
stdout or stderr), and exit the program with EXIT_CODE. Does not
return. */
void print_usage(FILE *stream, int exit_code)
{
    fprintf(stream, "Usage : %s options[inputfile...]\n", program_name);
    fprintf(stream,
            " - h-- help Display this usage information.\n"
            " - o-- output filename Write output to file.\n"
            " - v-- verbose Print verbose messages.\n");
    exit(exit_code);
}

/* Main program entry point. ARGC contains number of argument list
elements; ARGV is an array of pointers to them. */
int main(int argc, char *argv[])
{
    int next_option;
    /* A string listing valid short options letters. */
    const char *const short_options = "ho:v";
    /* An array describing valid long options. */
    const struct option long_options[] = {
        {"help", 0, NULL, 'h'},
        {"output", 1, NULL, 'o'},
        {"verbose", 0, NULL, 'v'},
        {NULL, 0, NULL, 0} /* Required at end of array. */
    };
    /* The name of the file to receive program output, or NULL for
    standard output. */
    const char *output_filename = NULL;
    /* Whether to display verbose messages. */
    int verbose = 0;
    /* Remember the name of the program, to incorporate in messages.
    The name is stored in argv[0]. */
    program_name = argv[0];
    do
    {
        next_option = getopt_long(argc, argv, short_options,
                                  long_options, NULL);
        switch (next_option)
        {
        case 'h': /* -h or --help */
            /* User has requested usage information. Print it to standard
            output, and exit with exit code zero (normal termination). */
            print_usage(stdout, 0);
        case 'o': /* -o or --output */
            /* This option takes an argument, the name of the output file. */
            output_filename = optarg;
            break;
        case 'v': /* -v or --verbose */
            verbose = 1;
            break;
        case '?': /* The user specified an invalid option. */
            /* Print usage information to standard error, and exit with exit
            code one (indicating abnormal termination). */
            print_usage(stderr, 1);
        case -1: /* Done with options. */
            break;
        default: /* Something else: unexpected. */
            abort();
        }
    } while (next_option != -1);
    /* Done with options. OPTIND points to first nonoption argument.
    For demonstration purposes, print them if the verbose option was
    specified. */
    if (verbose)
    {
        int i;
        for (i = optind; i < argc; ++i)
            printf("Argument : %s\n", argv[i]);
    }
    /* The main program goes here. */
    return 0;
}

#endif
