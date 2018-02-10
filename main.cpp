#include "practical_hg.h"
#include "fasta.h"
#include <string>
#include <iostream>


int main(int argc, char* argv[]) {
    int ch = -1;
    int back_size = 0;
    int rank = 0;
    int step = 0;
    char hg_file[200];
    char pos_file[200];

    while((ch = getopt(argc, argv, "t:s:r:p:g:")) != -1) {
        switch(ch) {
            case 's': {
                sscanf(optarg, "%d", &step);
                break;
            } case 't': {
                sscanf(optarg, "%d", &back_size);
                break;
            } case 'r': {
                sscanf(optarg, "%d", &rank);
                break;
            } case 'p': {
                sscanf(optarg, "%s", pos_file);
                break;
            } case 'g': {
                sscanf(optarg, "%s", hg_file);
                break;
            }
        }
    }
    blue::Fasta fs;
    fs.Open(hg_file);
    blue::PracticalHG phg(rank, back_size, fs);
    phg.AddPosFile(pos_file);
    phg.FindBackSeqs();
}
