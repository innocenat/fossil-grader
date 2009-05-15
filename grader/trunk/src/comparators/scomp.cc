#include <stdio.h>
#include <ctype.h>

// scomp: evaluator based on string compare---negleting all blanks
// parameter: scomp [input] [output] [solution]
// ---read output, check with solution
// ---if they are identical, output 1

main(int argc, char *argv[])
{
  FILE *fo, *fsol;

  if(argc!=4)
    return 0;
  fo = fopen(argv[2],"r");
  fsol = fopen(argv[3],"r");
  if((fo==NULL) || (fsol==NULL)) {
    printf("0\n-");
    return 0;
  }
  while((!feof(fo)) || (!feof(fsol))) {
    int co, csol;

    co=csol=' ';
    while((!feof(fo)) && (isspace(co=getc(fo))))
      ;
    while((!feof(fsol)) && (isspace(csol=getc(fsol))))
      ;
    if((feof(fo) && (!feof(fsol))) ||
       (feof(fsol) && (!feof(fo)))) {
      printf("0\n-");
      fclose(fo);
      fclose(fsol);
//  fprintf(stderr,"done\n");
      return 0;
    }
    if(feof(fo) && feof(fsol))
      break;
    if(co!=csol) {
      printf("0\n-");
      fclose(fo);
      fclose(fsol);
//  fprintf(stderr,"done\n");
      return 0;
    }      
  }
  printf("1\nP");
  fclose(fo);
  fclose(fsol);
//  fprintf(stderr,"done\n");
}
