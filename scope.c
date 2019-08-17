#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
	FILE* fp_scope;
	char buffer[1024];
	
	// open device
	fp_scope = fopen("/dev/usbtmc0", "w+");
	if (fp_scope == NULL) {
		printf("could not open device\n");
		return 1;
	}

	// ask for identification
	fprintf(fp_scope, "*IDN?\n");
	fgets(buffer, 1024, fp_scope);	
	printf("%s", buffer);

	// send screenshot download command
	fprintf(fp_scope, "HARDCOPY:INKSAVER OFF\n");
	fprintf(fp_scope, "HARDCOPY START\n");

	// read first 6 bytes to figure out the size
	fread(buffer, 1, 6, fp_scope);
	int size = buffer[5] << 24 | buffer[4] << 16 | buffer[3] << 8 | buffer [2];
	printf("size: %d bytes\n", size);
	
	// read the rest into a large allocated buffer
	char* image_buffer;
	image_buffer = malloc(size);
	memcpy(image_buffer, buffer, 6);
	fread(image_buffer+6, 1, size-6, fp_scope);

	// write it into a file
	FILE* fp_image;
	fp_image = fopen("test.bmp", "w");
	fwrite(image_buffer, 1, size, fp_image); 
	
	// clean up
	fclose(fp_image);
	free(image_buffer);
	fclose(fp_scope);

	return 0;
}
