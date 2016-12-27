#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "gnuplot_i.h"
// Function 1: Used to check if the input file exists

int check_file (char *filename)
{
  FILE *file;
  if ((file = fopen (filename, "r")))
    {
      fclose (file);
      return 0;
    };                          // if file exists
  return 1;
};                              // if file does not exists

// Function 2: Used to determinate the integration method

int mode_select (char mode[3])
{
  if (strcmp (mode, "ABS") == 0)
    {                           // for absolute integration
      return 1;
    };
  if (strcmp (mode, "MAT") == 0)
    {                           // for mathematical integration
      return 2;
    };
  if (strcmp (mode, "SET") == 0)
    {                           // to ignore the negative values
      return 3;
    }
  else
    return 0;
};
// Function 3: Used to call gnuplot to plot the input file data

void plot_file (char filename[40])
{
  gnuplot_ctrl *z;
  z = gnuplot_init ();
  gnuplot_set_xlabel (z, "Wavenumber(cm-¹)");
  gnuplot_set_ylabel (z, "Absorbance");
  gnuplot_cmd (z, "plot '%s' with lines lt rgb 'red'", filename);
};


