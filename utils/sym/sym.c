#include <log.h>
#include <symfile.h>

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(void)
{
	printf("Usage: sym [OPTIONS]\n"
	       "\n"
	       "  -A			   print all symbols\n"
	       "  -a address               symbol address to resolve\n"
	       "  -f symfile               path to .sym file\n"
	       "  -h                       print this message and exit\n"
	       "  -s symbol                symbol name to resolve\n"
	       "  -v                       increments the current Verbose level by one.\n"
	       "                           Multiple -v options are allowed\n"
	      );
}

int main(int argc, char *argv[])
{
	int c;
	int all = 0;
	void *address = NULL;
	char *symfile = NULL;
	char *symbol = NULL;

	while ((c = getopt(argc, argv, "Aa:f:hs:v")) != -1) {
		switch (c) {
		case 'A':
			all = 1;

			break;
		case 'a': {
			char *endptr = NULL;

			address = (void *)strtoul(optarg, &endptr, 16);
			if (*endptr != '\0' || address == (void *)ULONG_MAX) {
				say_error("bad value of address argument: %s", optarg);

				usage();

				exit(EXIT_FAILURE);
			}

			break;
		}
		case 'f':
			symfile = strdup(optarg);

			break;
		case 'h':
			usage();

			exit(EXIT_SUCCESS);
		case 's':
			symbol = strdup(optarg);

			break;
		case 'v':
			++verbose;

			break;
		case '?':
			usage();

			exit(EXIT_FAILURE);
		default:
			say_error("unmatched option: -%c", c);

			exit(EXIT_FAILURE);
		}
	}

	if (symfile == NULL) {
		say_error(".sym file must be specified");

		exit(EXIT_FAILURE);
	}

	if (symfile_load(symfile) != 0) {
		say_error("can't load `%s'", symfile);

		exit(EXIT_FAILURE);
	}

	if (all == 1) {
		for (unsigned i = 0; i < sym_table.n_symbols; ++i) {
			char *sym_name = sym_table.sym_name + sym_table.sym_entry[i].sym_name_off;
			void *addr = sym_table.sym_entry[i].addr;

			printf("%s 0x%x\n", sym_name, addr);
		}

		return 0;
	} else if (address != 0)
		printf("%s\n", symfile_name_by_addr(address));
	else if (symbol != NULL)
		printf("0x%x\n", symfile_addr_by_name(symbol));

	return 0;
}
