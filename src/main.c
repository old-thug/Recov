
#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#include "recov/recov.h"

#define WINDOW_HEIGHT 1028
#define WINDOW_WIDTH  780

#define WINDOW_FLAGS (SDL_WINDOW_RESIZABLE)

#define _shift(argc, argv)			\
     ({						\
	  (*argc)--;				\
	  const char *arg = *(*argv)++;		\
	  arg;					\
     })

#define shift(argc, argv) _shift(&argc, &argv)

/* int setupWindow() */
/* { */
/*      SDL_Window *main_window = SDL_CreateWindow("Recov", WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS); */
/*      SDL_Renderer *main_renderer = SDL_CreateRenderer(main_window, NULL);     */
/* } */

enum
{
     MODE_Scan = 0,
     MODE_Recover = 1,
};

typedef struct CliOptions
{
     int mode;

     union
     {	  
	  struct { const char *drive_name; } scan;
	  struct {
	       const char *drive_name;
	       const char *target_name;
	  } recover;
     };
} CliOptions;

void errorExit(const char *program_name, const char *fmt, ...)
{
     fprintf(stderr, "%s: Error: ", program_name);
     va_list args;
     va_start(args, fmt);
     vfprintf(stderr, fmt, args);
     fprintf(stderr, "\n");
     exit(1);
}

void printHelpText(const char *program_name, FILE *out)
{
     fprintf(out, "Usage: %s <mode> args...\n", program_name);
     fprintf(out, "Mode:\n");
     fprintf(out, "   scan <drive>                     Perform a scan on a Drive.\n");
     fprintf(out, "   recover <drive> <target>         Try to recover a file from a Drive.\n");
     fprintf(out, "   help                             Print this message.\n");;
}

void parseScanArgs(const char *program_name, int *argc, char ***argv, CliOptions *opt)
{
     opt->mode = MODE_Scan;
     if (!*argc)
     {
	  errorExit(program_name, "Expected Drive Name");
     }
     opt->scan.drive_name = _shift(argc, argv);
}

void parseCliOptions(int argc, char **argv, CliOptions *opt)
{
     const char *program_name = shift(argc, argv);
     if (!argc)			/* No mode was provided. */
     {
	  errorExit(program_name, "No mode was provided. try `%s help`", program_name);
     }
     const char *run_mode = shift(argc, argv);
     if (!strcmp(run_mode, "help"))
     {
	  printHelpText(program_name, stdout);
	  exit(0);
     }
     else if (!strcmp(run_mode, "scan"))
     {
	  // TODO: Check for failure here.
	  parseScanArgs(program_name, &argc, &argv, opt);
     }
     else
     {
	  errorExit(program_name, "Invalid Mode: `%s`\n", run_mode);
     }
}

int main(int argc, char **argv)
{
     // setUpwindow();
     // I Will be developing cli interface for now.
     CliOptions opt;
     parseCliOptions(argc, argv, &opt);

     switch (opt.mode)
     {
     case MODE_Scan:
	  int err = scanDrive(opt.scan.drive_name);
	  if (err == RECOV_ERR_Ok) return 0;
	  if (err != RECOV_ERR_No_Such_Drive)
	  {
	       errorExit(argv[0], "No such drive, `%s`", opt.scan.drive_name);
	  }
	  else 
	  {
	       errorExit(argv[0], "Fatal error");
	  }
	  break;
     case MODE_Recover:
	  break;
     default: /* Unrechable */ break;
     }
     return 0;
}
