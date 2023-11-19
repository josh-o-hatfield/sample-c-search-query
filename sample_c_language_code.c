// program that acts as a search engine to query the best matching documents given a set of documents

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define SIZE 50
#define N 4

// function prototype
char * searchEngine(char ** documents, char * query, int S);

int main(void) {
	
	// A simple search engine
	// For a given multi word query, returns the first S characters from the top-ranking document
	
	// N is the number of documents
	char * document[N] = {"Located in Bloomington, Indiana, the campus of IU Bloomington is home to an extensive student life program. The university campus is 1937 acres, the largest of any university in Indiana. The university has a student count of over 45000. The campus is where a student takes classes, just like any other campus.",
						  "The campus rests on a bed of Indiana limestone, along with the rest of Bloomington and southern Indiana. The Bryan house is the campus home of the university president. To a student and a faculty member alike, Indiana University has a campus that is rich with green space and historic buildings. Bloomington, Indiana is located in Southern Indiana.",
						  "The Indiana Memorial Union in Bloomington is the second largest student union in the world. It features a student activities tower, which houses a variety of student organizations. The University campus is centered around this student union. The Campus has an extensive bus system.",
						  "The Luddy School of Informatics, Computing, and Engineering is a school within Indiana University. A student can major in fields such as Informatics and Computer Science. The student count has grown greatly in the past few years, and Luddy Hall was built on Indiana University campus in 2018. Indiana University does not have a mascot. The university is known for its Sample Gates."};
	
	printf("-------------------------\n");
	
	// q is the number of terms in the query
	char q[SIZE] = "";
	
	// the number of characters (from the beginning) from the resulting document to return
	int number_of_chars = 0;
	
	// ------------------------------------------------------------------------
	
	// prompt
	printf("Query: ");
	// fgets function for scanning user input of needle array
	fgets(q, SIZE, stdin);
	
	for (int i = 0; i < SIZE; i++) {
		if (q[i] == '\n') {
			q[i] = '\0';
			break;
		}
	}
	
	// prompt
	printf("Input number of characters to return: ");
	// scan in index location user input
	scanf("%d", &number_of_chars);
	
	printf("-------------------------\n");
	
	// call to function and print
	searchEngine(document, q, number_of_chars);
}

char * searchEngine(char ** documents, char * query, int S) {
	// size of string input
	size_t str_len = strlen(query);

	// automatically return NULL if string input is empty
    if (str_len == 0)
        return NULL;
	
    char** tokens = malloc(sizeof(char*) * 2);
    int token_size = 2;

    char word[SIZE] = "\0";
    int word_count = 0;
    for (register int i = 0, j = 0; i < str_len + 1; i++) {
        if (query[i] == ' ' || query[i] == '\0' || i == str_len) {
            size_t word_len = strlen(word);

            // Add a null character at the end
            if (word_len >= SIZE)
                word[j - 1] = '\0';
            else if (word_len > 0)
                word[j] = '\0';
            // Empty word
			else { 
                j = 0;
                continue;
            }             

            // Grow size
            if (word_count >= token_size - 1) {
                token_size *= 2;
                tokens = realloc(tokens, sizeof(char*) * token_size);
            }

            tokens[word_count] = malloc(sizeof(char) * word_len + 1);
            strcpy(tokens[word_count++], word);

            j = 0;
            word[0] = '\0';
            continue;
        }

        if (j < SIZE - 1)
            word[j++] = query[i];
    }

    if (word_count == 0)
        tokens[1] = NULL;

    tokens[word_count] = NULL;
	
	// counts number of words within query
	int query_counter = 0;
	for (int k = 0; k < word_count; k++) {
		query_counter++;
	}
	
	// variable for setting two-dimensional scores based on number of documents and number of query words
	int scores[query_counter][N];
	// variable for computing intersection once two-dimensional array has been finalized
	int intersection[N];
	
	// variable for counting the frequency of query words in a document
	int frequency = 0;
	
	// loop through every query word
	for (int l = 0; l < query_counter; l++) {
		// loop through every document
		for (int m = 0; m < N; m++) {
			// variable for iterating through every character in a document
			int doc_len = strlen(documents[m]);
			// variable for iterating through every character in a query word
			int word_len = strlen(tokens[l]);
			
			// loop through document, excluding query word
			for (int n = 0; n <= doc_len - word_len; n++) {
				int flag = 1;
				
				// loop through every character in query word
				for (int o = 0; o < word_len; o++) {
					// determines match / non-match between document word and query word
					if (tolower(documents[m][n + o]) != tolower(tokens[l][o])) {
						flag = 0;
						break;
					}
				}
				
				// increments match if words match
				if (flag == 1) {
					frequency++;
				}
			}
			
			// sets location of query word in two-dimensional array to frequency calculated
			scores[l][m] = frequency;
			
			// need to set frequency back to zero for every query word
			frequency = 0;
		}
	}
	
	// variable for calculating sum between row values in the same column
	int sum = 0;
	
	// loop through every document as a column
	for (int p = 0; p < N; p++) {
		// loop through every query word as a row
		for (int q = 0; q < query_counter; q++) {
			// sum row values within the same column
			sum += scores[q][p];
			
			// set intersection to sum
			intersection[p] = sum;
		}
		// need to set sum back to zero for every column
		sum = 0;
	}
	
	// variable for determining index for highest frequency
	int max_doc_counter = 0;
	
	// loop through every document / column
	for (int r = 0; r < N; r++) {
		// set max value to initial intersection value
		int max = intersection[0];
		
		// if next column value is higher than max value, set the counter variable to the index of the documents array
		if (intersection[r] > max) {
			max_doc_counter = r;
		}
	}
	
	/*
	for (int t = 0; t < N; t++) {
		printf("%d ", intersection[t]);
	}
	printf("\n");
	*/
	
	int result_size = 0;
	result_size = S;
	// variable for copying number of characters specified based on document with highest frequency
	char result[S];
	strncpy(result, documents[max_doc_counter], S);
	
	// for some reason, had to manually offset by N for the terminating null character since there were additional characters in the result
	result[strlen(result) - N] = '\0';
	
	printf("%s\n", result);
	
	// return result;
}