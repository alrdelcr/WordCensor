#include "bf.h"
#include "bv.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define OPTIONS "ht:f:mi:c:b:"

int main(int argc, char **argv) {
  int opt = 0;
  uint32_t htsize = 10000; // set the default hash table size
  uint32_t bfsize = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 *
                    2 * 2 * 2 * 2; // set the default bloom filter size
  bool m = false;                  // set the default mtf to false
  FILE *bad = fopen("dummy.txt", "r");
  FILE *infile = stdin;
  FILE *censored = fopen("censored.txt", "w");
  uint32_t i = 0;
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h': // help argument
      fprintf(stderr,
              "Usage: ./censor [options]\n"
              "  ./banhammer will read in words from stdin, identify any "
              "badspeak or old speak and output an\n"
              "  appropriate punishment message. The badspeak and oldspeak "
              "(with the newspeak translation)\n"
              "  that caused the punishment will be printed after the message. "
              "If statistics are enabled\n"
              "  punishment messages are suppressed and only statistics will "
              "be printed.\n"
              "    -t <ht_size>: Hash table size set to <ht_size>. (default: "
              "10000)\n"
              "    -f <bf_size>: Bloom filter size set to <bf_size>. (default "
              "2^19)\n"
              "    -s          : Enables the printing of statistics.\n"
              "    -m          : Enables move-to-front rule.\n"
              "    -h          : Display program synopsis and usage.\n");
      return 0;
      break;
    case 't':
      htsize = strtoul(optarg, NULL, 10); // set the hash table size
      break;
    case 'f':
      bfsize = strtoul(optarg, NULL, 10); // set the bloom filter size
      break;
    case 'm':
      m = true; // set the mtf to true
      break;
    case 'i': // Changes the infile to the file with the list of banned word. MUST DO
      infile = fopen(optarg, "r");
      i = 1;
      break;
    case 'c': // Changes the outfile name to write the censored text to.
      censored = fopen(optarg, "w");
      break;
    case 'b': // Changes the outfile name to write the censored text to.
      bad = fopen(optarg, "r");
      break;
    default: // if none of the arguments are given, print the help message and
             // return a 1
      fprintf(stderr,
              "Usage: ./banhammer [options]\n"
              "  ./banhammer will read in words from stdin, identify any "
              "badspeak or old speak and output an\n"
              "  appropriate punishment message. The badspeak and oldspeak "
              "(with the newspeak translation)\n"
              "  that caused the punishment will be printed after the message. "
              "If statistics are enabled\n"
              "  punishment messages are suppressed and only statistics will "
              "be printed.\n"
              "    -t <ht_size>: Hash table size set to <ht_size>. (default: "
              "10000)\n"
              "    -f <bf_size>: Bloom filter size set to <bf_size>. (default "
              "2^19)\n"
              "    -s          : Enables the printing of statistics.\n"
              "    -m          : Enables move-to-front rule.\n"
              "    -h          : Display program synopsis and usage.\n");
      return 1;
      break;
    }
  }
  
  HashTable *ht = ht_create(htsize, m); // create the hash table
  BloomFilter *bf = bf_create(bfsize);  // create the bloom filter
  char *word1 = (char *)malloc(1001);   // create a word1
  Parser *p = parser_create(bad);         // create the parser
  while (next_word(p, word1)) {         // while there is still a next word
    bf_insert(bf, word1);               // insert the word into the bloom filter
    ht_insert(ht, word1);         // insert into the hash table
  }
  char line[1001];		// line buffer to read from
  uint32_t current = 0;		// current index variable
  char *word = (char *)malloc(1001);   // create a word
  char *lower = (char *)malloc(1001);   // create a word1
  fgets(line,1001,infile);
  while (line[current] != '\0') {
	if (isalnum(line[current]) || line[current] == '\\' || line[current] == '-') {
        	uint32_t counter = 0;
 		while (isalnum(line[current]) || line[current] == '\'' || line[current] == '-') {
        		word[counter] = line[current];
        		lower[counter] = tolower(line[current]);
        		current++;
        		counter++;
        	}
        	word[counter] = '\0';		// end the word with a null terminator
        	lower[counter] = '\0';		// end the word with a null terminator
        	Node *node;
        	if (bf_probe(bf, lower) == true) { // if the word is in the bloom filter
      			node = ht_lookup(ht, lower);     // lookup the owrd in the hashtable
      			if (node) { // if its in the hashtable (node pointer is not null)
      				strcpy(word,node->censored);		// switches the word to the censored version
          		}
          	} 
        fprintf(censored,"%s",word);
        } else if (line[current] == '\n') {
         	fputc(line[current],censored);
         	if(fgets(line,1001,infile) == NULL) {
         		break;
         	}	
         	current = 0;
        } else {
        	fputc(line[current],censored);
        	current++;
        }
  }
  ht_delete(&ht); // free everything you have allocated (NO MEMORY LEAKS!)
  bf_delete(&bf);
  free(word1);
  free(word);
  fclose(censored);
  if (i == 1) {
     fclose(infile);
  }
  fclose(bad);
  parser_delete(&p);
  return 0; // return 0
}
