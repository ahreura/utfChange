#include "debug.h"
#include "utf.h"
#include "wrappers.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int opterr;
int optopt;
int optind;
char *optarg;

state_t *program_state;

void
parse_args(int argc, char *argv[])
{
  //bool infileBool = 0;
  bool eBool = 0;
  int i;
  char option;
  char *joined_argv;
  int index = 0;
  joined_argv= join_string_array(argc, argv);
  info("argc: %d argv: %s", argc, joined_argv);
  free(joined_argv);
  program_state = Calloc(1, sizeof(state_t));
  for (i = 0; index < argc; ++i) {
    index++;
    debug("%d opterr: %d", i, opterr);
    debug("%d optind: %d", i, optind);
    debug("%d optopt: %d", i, optopt);
    debug("%d argv[optind]: %s", i, argv[optind]);
     if (eBool == 0 && (option = getopt(argc, argv, "+e:")) != -1) {
      switch (option) {
        case 'e': {
          eBool = 1;
          info("Encoding Argument: %s", optarg);

          if ((program_state->encoding_to = determine_format(optarg)) == 0)
                print_state();
              break;
        }
        case '?': {
          if (optopt != 'h')
            fprintf(stderr, KRED "-%c is not a supported argument\n" KNRM,
                    optopt);
          USAGE(argv[0]);
          exit(0);
        }
        default: {
          break;
        }
      }
    }
    else if(argv[optind] != NULL)
    {
       if(optind == argc -1){
          if (eBool == 1 && program_state->in_file == NULL) {
            //infileBool =1;
            program_state->in_file = argv[argc-4];
          }
          else if(eBool == 1 && program_state->out_file == NULL)
          {
            program_state->out_file = argv[argc -1];
            index++;
            if(program_state->in_file == program_state-> out_file){
              exit(EXIT_FAILURE);
              // || program_state-> out_file == NULL
            }
            break;
          }
       }
       else if(optind == argc -2){
          if (eBool == 1 && program_state->in_file == NULL) {
            //infileBool =1;
            program_state->in_file = argv[argc-2];
          }
          else if(eBool == 1 && program_state->out_file == NULL)
          {
            program_state->out_file = argv[argc-1];
            index++;
            if(program_state->in_file == program_state-> out_file){
              exit(EXIT_FAILURE);
            }
            break;
          }
       }
    }
    else if(argv[optind] == NULL){
      if (eBool == 1 && program_state->in_file == NULL) {
            //infileBool =1;
            program_state->in_file = argv[argc-4];
          }
          else if(eBool == 1 && program_state->out_file == NULL)
          {
            program_state->out_file = argv[argc-3];
            optind++;
            if(program_state->in_file == program_state-> out_file){
              exit(EXIT_FAILURE);
            }
            break;
          }
    }
    if(eBool == 0){
      optind++;
    }
  }
}

format_t
determine_format(char *argument)
{

  if (strcmp(argument, "UTF16LE") == 0)
    return UTF16LE;
  if (strcmp(argument, "UTF16BE") == 0)
    return UTF16BE;
  if (strcmp(argument, "UTF8") == 0)
    return UTF8;
  return 0;
}

char*
bom_to_string(format_t bom){
  switch(bom){
    case UTF8: return STR_UTF8;
    case UTF16BE: return STR_UTF16BE;
    case UTF16LE: return STR_UTF16LE;
  }
  return "UNKNOWN";
}

char*
join_string_array(int count, char *array[])
{
  char *ret;
  char charArray[count];
  int i;
  int s = count;
  int len = 0;
  int str_len = 0;
  int cur_str_len =0;
  str_len = array_size(count, array);
  ret = &charArray[0];
  void * cccc = Calloc(str_len, 1);
  ret = (char*) cccc;
  for (i = 0; i < s; ++i) {
    cur_str_len = strlen(array[i]);
    memecpy(ret + len, array[i], cur_str_len);
    len += cur_str_len;
    memecpy(ret + len, " ", 1);
    len += 1;
  }

  return cccc;

}

int
array_size(int count, char *array[])
{
  int i, sum = 1; /* NULL terminator */
  for (i = 0; i < count; ++i) {
    sum += strlen(array[i]);
    ++sum; /* For the spaces */
  }
  return sum+1;
}

void
print_state()
{
//errorcase:
  if (program_state == NULL) {
    error("program_state is %p", (void*)program_state);
    exit(EXIT_FAILURE);
  }
  info("program_state {\n"
         "  format_t encoding_to = 0x%X;\n"
         "  format_t encoding_from = 0x%X;\n"
         "  char *in_file = '%s';\n"
         "  char *out_file = '%s';\n"
         "};\n",
         program_state->encoding_to, program_state->encoding_from,
         program_state->in_file, program_state->out_file);
}
