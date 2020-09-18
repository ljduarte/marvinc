/*############################################################################################
  This code was written in C language for the MarvinC software as a part of a  undergraduante
  research project sponsored by FAPESP(grant #2016/07411-4). Be free to modify it according to
  your need. The main lines and some of the variables were commented to make the understanding
  easier. Notice that this program only calculates the area under a curve, without any concern
  about overlapping, overtones and combination bands affecting the result. Plase, report bugs
  and send suggestions to leo.j.duarte@hotmail.com.br.

  This software uses the gnuplot_i library written by N.Devillard.
##############################################################################################*/

#include "config"
#include "func.h"

void main (int argc, char *argv[]){
  double Vx[2], Vy[2];
  double *X, *Y, x, y;
  double area;
  double major;
  int cont;			// Arguments Counter
  int m;
  int i, err, lines;
  int j;
  int a;
  int l;
  int minor_index;
  double minor;
  double threshold;
  double derivative;
  int upper_limit, lower_limit;
  int bands_int;
  int max_abs_index;
  int h;			// Integrated Bands Counter
  float initial_float, final_float;
  char initial[20], final[20];	// The program will read all inputs as strings, this is used to avoid some input errors
  char file_name[70];
  char list_name[70];
  char bands[150];
  char new_file[1];		// Assumes only Y or N values
  char mode[3];
  FILE *fp;			// File Pointer
  FILE *lp;			// List Pointer
// Some Initial Values
  i = 0;
  lines = 0;
  area = 0.0;
  h = 0;
  gnuplot_ctrl *z;
  a = 0;
// Conversion Factors
  float K = 974.0864;		// km.amu.(mol.e^2)^-1
  double q = 23025.8509299405;	// cm-2.atm-1
  double w = 5668.18081627168;	// km.mol-1
  double e = 566818081627.168;	// cm.mmol-1
  double r = 9.41089293752562e-10;	// cm.molecule-1

  printf ("-------------------------------------------------------------\n\n");
  printf ("                          WELCOME TO                          \n");
  printf ("                           MarvinC                            \n\n");
  printf ("-------------------------------------------------------------\n\n");

  fp = fopen ("output.txt", "w");
  fprintf (fp,"**************************************************************\n");
  fprintf (fp,"************                                     *************\n");
  fprintf (fp,"************         OUTPUT FROM MarvinC         *************\n");
  fprintf (fp,"************                                     *************\n");
  fprintf (fp,"**************************************************************\n\n");
  fprintf (fp,"      L.J. DUARTE*, A.F. SILVA, W. E. RICHTER, R.E BRUNS        \n");
  fprintf (fp,"    University of Campinas, Institute of Chemistry - 2016.    \n\n");
  fprintf (fp,"For details about the program and conversion factors,please\nsee Vib. Spectr. XX, XXXX (2016).\n\n");
  fprintf (fp,"Please note that the code just integrate the desired band \nwithout any concern about overlapping, overtones and\ncombination bands affecting the result. Be sure this issues\nwill not lead to any misinterpretation.");
  fclose (fp);

  if (argc == 1) {
      do {
	  do {
	      printf ("Please, insert the file name:  ");
	      scanf ("%s", file_name);
	      if (check_file (file_name)){
		  printf ("ERROR: File not found. Try again!.\n");}
	      else
		printf ("Ready to perform the integration.\n");}
	  while (check_file (file_name));
	  plot_file (file_name);
	  do  {
	      printf ("How many bands would you like to integrate?  ");
	      scanf ("%s", bands);
	      bands_int = atoi (bands);
	      if (bands_int == 0){
		  printf("ERROR: The number of bands must be a number greater than ZERO. Try again!\n");};
	    }while (bands_int == 0);
	  do {
	      printf("Please, select the integration method:\n ABS - Absolute Integration\n MAT - Mathematical Integration\n SET - Set negative values as zero\n");
	      scanf ("%s", mode);
	      m = mode_select (mode);
	      if (m == 0){
		  printf("Please type only one of the options: ABS, MAT or SET - Set negative values as zero\n Try again!\n");};
	    } while (m == 0);
	  fp = fopen ("output.txt", "a");
	  fprintf (fp, "\n\n\n");
	  fprintf (fp,"**************************************************************\n\n");
	  fprintf (fp, "            File name:        %s\n", file_name);
	  fprintf (fp, "            Number of bands:  %d\n", bands_int);
	  fprintf (fp, "            Int. Method:      %s\n\n", mode);
	  fprintf (fp, "**************************************************************\n\n");
	  fprintf (fp, "\n\n\n");
	  fclose (fp);
	  fp = fopen (file_name, "r");	// Used to count the number of lines in the input file
	  while ((err = fscanf (fp, "%le %le\n", &x, &y)) != EOF){
	      lines = lines + 1; };
	  fclose (fp);
	  X = (double *) malloc ((lines + 5) * sizeof (double));
	  Y = (double *) malloc ((lines + 5) * sizeof (double));
	  fp = fopen (file_name, "r");	// The following lines checks if the X value is in crescent or decreasing order
	  fscanf (fp, "%le %le\n %le %le\n", &Vx[0], &Vy[0], &Vx[1], &Vy[1]);
	  fclose (fp);
	  if ((Vx[1] - Vx[0]) < 0)  {
	      i = lines - 1;
	      fp = fopen (file_name, "r");
	      while ((err = fscanf (fp, "%lf %lf\n", &x, &y)) != EOF){
		 if (m == 1){
		      X[i] = x;
		      Y[i] = sqrt(y*y);};
		  if (m == 2){
		      X[i] = x;
		      Y[i] = y;};
		  if (m == 3){
		      X[i] = x;
		      if (y <= 0){
			  Y[i] = 0;};
		      if (y > 0){
			  Y[i] = y;}; };
		  i=i-1;};
	      fclose (fp); };
	  if ((Vx[1] - Vx[0]) > 0) {
	      fp = fopen (file_name, "r");
	      while ((err = fscanf (fp, "%lf %lf\n", &x, &y)) != EOF){
		  if (m == 1){
		      X[i] = x;
		      Y[i] = sqrt (y * y); };
		  if (m == 2){
		      X[i] = x;
		      Y[i] = y; };
		  if (m == 3) {
		      X[i] = x;
		      if (y <= 0){
			  Y[i] = 0;};
		      if (y > 0){
			  Y[i] = y;};};
		  i=i+1;};
	      fclose (fp);};
	  do {
	      area = 0.0;
	      h = h + 1;
	      do {
		  do {
		      printf ("Please, insert the initial value of X for band #%d:", h);
		      scanf ("%s", initial);
		      initial_float = atof (initial);
		      if (initial_float == 0){
			  printf("ERROR: The inicial value must be a number greater than ZERO. Try again!\n");};
		    } while (initial_float == 0);
		  do {
		      printf("Please, insert the final value of X for band #%d:",h);
		      scanf ("%s", final);
		      final_float = atof (final);
		      if (final_float == 0){
			  printf("ERROR: The final value must be a number greater than ZERO. Try again!\n");};
		    } while (initial_float == 0);	// #8 Close
		  final_float = final_float + 1;
		  for (i = 0; i < lines; i = i + 1) {
		      if (initial_float <= X[i]){
			  lower_limit = i;
			  break;}; };
		  for (i = 0; i < lines; i = i + 1){
		      if (final_float <= X[i]){
			  upper_limit = i - 1;
			  break;}
		      else
			upper_limit = i;};
		  if (X[upper_limit] <= X[lower_limit]) {
		      printf("ERROR: The final value must be greater than the initial value. Try again!\n"); };
		}while (X[upper_limit] <= X[lower_limit]);	// #6 Close
	      major = Y[lower_limit];
	      for (i = lower_limit; i < upper_limit + 1; i = i + 1){
		  if (Y[i] >= major){
		      major = Y[i];
		      max_abs_index = i; }; };
	      for (i = lower_limit; i < upper_limit; i = i + 1){
		  area=area+((X[i+1]-X[i])*(Y[i]+Y[i+1])/2);};
	      fp = fopen ("output.txt", "a");
	      fprintf (fp, "---------------\n");
	      fprintf (fp, "    BAND #%d\n", h);
	      fprintf (fp, "---------------\n\n");
	      fprintf (fp, "Upper band limit:         %.2lf\n", X[upper_limit]);
	      fprintf (fp, "Lower band limit:         %.2lf\n", X[lower_limit]);
	      fprintf (fp, "Number of points:         %d\n", upper_limit - lower_limit + 1);
	      fprintf (fp, "Maximum absorbance at:    %.2lf cm⁻¹\n", X[max_abs_index]);
	      fprintf (fp, "Maximum absorbance:       %.4le\n", Y[max_abs_index]);
	      fprintf (fp, "Integral:                 %.10lf\n\n", area);
	      fprintf (fp, "Integrated intensity (band strength):\n\n");
	      fprintf (fp, "km.mol-¹                  %.2lf \n", w * area);
	      fprintf (fp, "cm-².atm-¹                %.2lf \n", q * area);
	      fprintf (fp, "cm.mmol-¹                 %.2le \n", e * area);
	      fprintf (fp, "cm.molecule-¹             %.2le \n\n", r * area);
	      fprintf (fp, "Dipole moment derivative:\n");
	      fprintf (fp, "(dP/dQ)                  |%.4lf| e/sqrt(amu)\n\n",sqrt (w * area / K));
	      fprintf (fp, "\n\n\n");
	      fclose (fp);
 	 } while (h != bands_int);
	  free (X);
	  free (Y);
	  file_name[0] = '\0';
	  lines = 0;
	  h = 0;
	  new_file[0] = '\0';
	  mode[0] = '\0';
	  do{
	      printf ("Do you want to load another file? (Y/N):  ");
	      scanf ("%s", new_file);
	      if (strcmp (new_file, "Y") != 0 && strcmp (new_file, "N") != 0){
		  printf ("ERROR: Insert only N or Y. Try again!\n");};
	    } while (strcmp (new_file, "Y") != 0 && strcmp (new_file, "N") != 0);
	}while (strcmp (new_file, "Y") == 0);
    };

if(argc >= 2){
        if (argc==2 && strcmp(argv[1],"l")!=0){
		printf("ERROR: Invalid argument, use 'l' if you want to perform an automatic integration of a list os spectra, or use none to perform a manual integration of a file.\n If you are using a list as input, you can highlight the bands by typing 'MarivnC l hl'\n");
	exit(0); };
	if (argc == 3 && strcmp(argv[1],"l") != 0 || strcmp(argv[2],"hl") != 0){
		printf("ERROR: Invalid argument, use 'l' if you want to perform an automatic integration of a list os spectra, or use none to perform a manual integration of a file.\n If you are using a list as input, you can highlight the bands by typing 'MarivnC l hl'\n");
	exit(0); };
	do{
		printf("Please, insert the list name: ");
		scanf("%s", list_name);
		if(check_file(list_name)){
			printf("ERROR: File not found. Try again!.\n");}
		else
			printf("Ready to perform the integration.\n");
	}while(check_file(list_name));
	do{
		printf("Please, select the integration method:\n ABS - Absolute Integration\n MAT - Mathematical Integration\n SET - Set negative values as zero\n");
		scanf("%s", mode);
		m=mode_select(mode);
		if (m==0){
			printf("Please type only one of the options: ABS, MAT or SET - Set negative values as zero\n Try again!\n"); };
	}while(m == 0);

lp=fopen(list_name,"r");
while ((err = fscanf(lp,"%s\n", &file_name)) !=EOF){
	fp=fopen("output.txt", "a");
	fprintf(fp,"\n\n\n");
	fprintf(fp,"**************************************************************\n\n");
	fprintf(fp,"			File name:	%s\n", file_name);
	fprintf(fp,"			Int. Method:	%s\n\n", mode);
	fprintf(fp," This is an automatic integration routine, keep in mind that\n");
	fprintf(fp,"    the band limits can be greater or lower than expected.\n\n");
	fprintf(fp,"**************************************************************\n\n");
	fprintf(fp,"\n\n\n");
	fclose(fp);
	fp = fopen(file_name,"r");
	while ((err = fscanf(fp,"%le %le\n", &x, &y)) !=EOF){
		lines=lines+1; };
	fclose(fp);
	X=(double*)malloc((lines+5)*sizeof(double));
	Y=(double*)malloc((lines+5)*sizeof(double));
	fp =fopen(file_name,"r");
	fscanf(fp, "%le %le\n %le %le\n", &Vx[0], &Vy[0], &Vx[1], &Vy[1]);
	fclose(fp);
	if((Vx[1]-Vx[0])<0){
		i=lines-1;
		fp =fopen(file_name,"r");
		while ((err = fscanf(fp, "%lf %lf\n", &x, &y)) !=EOF){
			if (m==1){
				X[i]=x;
				Y[i]=sqrt(y*y); };
			if (m==2){
				X[i]=x;
				Y[i]=y; };
			if (m==3){
				X[i]=x;
				if (y<=0){
					Y[i]=0; };
				if (y>0){
					Y[i]=y; }; };
		i=i-1; };
		fclose(fp); };

	if((Vx[1]-Vx[0])>0){
		fp =fopen(file_name,"r");
		while ((err = fscanf(fp, "%lf %lf\n", &x, &y)) !=EOF){
		if (m==1){
			X[i]=x;
			Y[i]=sqrt(y*y); };
		if (m==2){
			X[i]=x;
			Y[i]=y; };
		if (m==3){
			X[i]=x;
			if (y<=0){
				Y[i]=0; };
			if (y>0){
				Y[i]=y; }; };
		i=i+1; };
		fclose(fp); };
	i=0;
	j=0;
	minor_index=0;
	minor = 1000;
	h=0;

	for (i=0; i<=lines; i=i+1) {
		if(Y[i]<=minor && Y[i]>0){
			minor = Y[i];
			minor_index=i; }; };
	for (i=0; i<=lines; i=i+1) {
		if (Y[i]>=noise_threshold*minor) {
			lower_limit=i;
			area=0;
			for (j=i; j<=lines; j=j+1) {
		 		area=area+((X[j+1]-X[j])*(Y[j]+Y[j+1])/2);
				if ( ((X[j+1]-X[j])*(Y[j]+Y[j+1])/2) < integration_threshold && X[j]-X[i] >= min_band_width ){
					area=area+((X[j+1]-X[j])*(Y[j]+Y[j+1])/2);
					if (area <= area_threshold){
						break;};
					upper_limit=j;
					h=h+1;
					major = Y[lower_limit];
					for (i=lower_limit; i<=upper_limit; i=i+1){ //6
						if (Y[i]>=major){// 7
							major = Y[i];
							max_abs_index=i; }; }; // fecha 6 e 7
					fp = fopen("output.txt", "a");
					fprintf(fp,"---------------\n");
					fprintf(fp,"    BAND #%d\n",h);
					fprintf(fp,"---------------\n\n");
					fprintf(fp,"Upper band limit:		%.2lf\n", X[upper_limit]);
					fprintf(fp,"Lower band limit:		%.2lf\n", X[lower_limit]);
					fprintf(fp,"Number of points:		%d\n", upper_limit-lower_limit+1);
					fprintf(fp,"Maximum absorbance at:	%.2lfcm−1\n", X[max_abs_index]);
					fprintf(fp,"Maximum absorbance:		%.4le\n", Y[max_abs_index]);
					fprintf(fp,"Integral:			%.10lf\n\n", area);
					fprintf(fp,"Integrated intensity (band strength):\n\n");
					fprintf(fp,"km.mol-1	%.2lf\n", w*area);
					fprintf(fp,"cm-2.atm-1	%.2lf\n", q*area);
					fprintf(fp,"cm.mmol-1	%.2le\n", e*area);
					fprintf(fp,"cm.molecule-1	%.2le \n\n", r*area);
					fprintf(fp,"Dipole moment derivative:\n");
					fprintf(fp,"(dP/dQ)	|%.4lf| e/sqrt(amu)\n\n", sqrt(w*area/K));
					fprintf(fp,"\n\n\n");
					fclose(fp);
					if (argc == 3) {
						fp = fopen ("bands", "a");
						for (a=lower_limit; a<=upper_limit; a++){
							fprintf (fp, "%le %le\n", X[a], Y[a]);};
					fclose (fp);};
					i =j+1;
					a=0;
					break;};};};};
	if (argc == 3){
		z = gnuplot_init ();
		gnuplot_set_xlabel (z, "Wavenumber(cm-1)");
		gnuplot_set_ylabel (z, "Absorbance");
		gnuplot_cmd (z, "set term png");
		gnuplot_cmd (z, "set output \"%s.png\"", file_name);
		gnuplot_cmd (z,"plot '%s' with lines lt rgb 'red', 'bands' with points lt rgb 'blue' ",file_name);
		gnuplot_close (z);};
	free (X);
	free (Y);
	file_name[0] = '\0';
	lines = 0;
	remove ("bands");};
	fclose (lp);};
printf("\n\n");
printf("-------------------------------------------------------------\n\n");
printf(" Thanks for using MarvinC, don't forget to check output.txt!\n\n");
printf("-------------------------------------------------------------\n\n");
};

/*END OF PROGRAM*/








